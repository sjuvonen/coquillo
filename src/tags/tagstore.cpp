#include "crawler/types.hpp"
#include "tagstore.hpp"

#include <QDebug>

namespace Coquillo {
    namespace Tags {
        Store::Store() {
            _fallback = {
                {"album", "album"},
                {"artist", "artist"},
                {"comment", "comment"},
                {"genre", "genre"},
                {"number", "number"},
                {"title", "title"},
                {"year", "year"},
            };

            _mappings = {
                {"id3v2", {
                    {"album", "TALB"},
                    {"album_artist", "TPE2"},
                    {"artist", "TPE1"},
                    {"comment", "COMM"},
                    {"composer", "TCOM"},
                    {"disc", "TPOS"},
                    {"encoder", "TENC"},
                    {"genre", "TCON"},
                    {"number", "TRCK"},
                    {"total", "TRCK[b]"},
                    {"original_artist", "TOPE"},
                    {"title", "TIT2"},
                    {"url", "WXXX"},
                    {"year", "TDRC"},
                }},
                {"xiph", {
                    {"album", "ALBUM"},
                    {"album_artist", "ALBUMARTIST"},
                    {"artist", "ARTIST"},
                    {"comment", "DESCRIPTION"},
                    {"composer", "COMPOSER"},
                    {"disc", "DISCNUMBER"},
                    {"encoder", "ENCODED-BY"},
                    {"genre", "GENRE"},
                    {"number", "TRACKNUMBER"},
                    {"original_artist", "PERFORMER"},
                    {"title", "TITLE"},
                    {"total", "TRACKTOTAL"},
                    {"url", "CONTACT"},
                    {"year", "DATE"},
                }}
            };
        }

        void Store::add(const QVariantHash & file) {
            const QString path = file.value("path").toString();
            const QString primary = file.value("primary").toString();
            const QVariantHash taghash = file.value("tags").toHash();
            Container item(path);
            item.setPrimaryTag(primary);

            foreach (const QString type, taghash.keys()) {
                const Tag::Mapping mapping = _mappings.contains(type) ? _mappings.value(type) : _fallback;
                const QVariantHash values = taghash[type].toHash();
                const Tag tag(type, mapping, values);
                item.addTag(tag);
            }

            item.setImageCount(file.value("image_count").toInt());

            foreach (const QVariant data, file.value("images").value<ImageDataList>()) {
                // item.addImage(Image::fromValues(data.toHash()));
            }

            _items << item;
        }

        void Store::add(const QList<QVariantHash> & files) {
            foreach (const QVariantHash & file, files) {
                add(file);
            }
        }

        const Container Store::at(int pos) const {
            return _items[pos];
        }

        Container & Store::at(int pos) {
            return _items[pos];
        }

        bool Store::isModified(int pos) const {
            const Container & item = at(pos);
            return _backup.contains(item.id());
        }

        bool Store::isFieldModified(int pos, const QString & field) const {
            const Container & item = at(pos);
            if (_backup.contains(item.id())) {
                const Container & backup = _backup[item.id()];
                return item.value(field) != backup.value(field);
            }
            return false;
        }

        void Store::reset() {
            for (int i = 0; i < _items.size(); i++) {
                const auto file = _items.at(i);
                if (_backup.contains(file.id())) {
                    _items.replace(i, _backup[file.id()]);
                }
            }
            commit();
        }

        void Store::commit() {
            _backup.clear();
        }

        void Store::remove(int pos) {
            if (pos >= 0 && pos < size()) {
                const auto item = at(pos);
                _backup.remove(item.id());
                _items.removeAt(pos);
            }
        }

        bool Store::rename(int pos, const QString & new_path) {
            Container & item = at(pos);

            if (item.path() != new_path) {
                qDebug() << "rename" << item.path() << new_path;
                backup(Container(item));
                item.setPath(new_path);
                return true;
            }

            return false;
        }

        bool Store::setValue(int pos, const QString & field, const QVariant & value) {
            Container & item = at(pos);
            bool changed = false;

            foreach (const QString key, item.tagNames()) {
                /*
                 * NOTE: Using a const copy here to prevent the backup also changing upon writing
                 * into the field.
                 */
                const Tag tag = item.tag(key);

                if (!tag.equals(field, value)) {
                    const Container copy(item);
                    // qDebug() << "change" << tag.id() << field << tag.value(field) << value;

                    if (item.tag(key).insert(field, value)) {
                        backup(copy);
                        changed = true;
                    }
                }
            }

            return changed;
        }

        bool Store::setImages(int pos, const QList<Image> & images) {
            Container & item = at(pos);

            /*
             * Do not backup when images are being initialized.
             */
            if (item.images().size() > 0 || item.imageCount() == 0) {
                backup(item);
            }

            item.setImages(images);
            return true;
        }


        void Store::backup(int pos) {
            const Container item = at(pos);

            if (!_backup.contains(item.id())) {
                _backup.insert(item.id(), item);
            }
        }

        void Store::backup(const Container & item) {
            if (!_backup.contains(item.id())) {
                _backup.insert(item.id(), item);
            }
        }

        QList<Container> Store::changedItems() const {
            QList<Container> changed;

            foreach (const Container & item, _items) {
                if (_backup.contains(item.id())) {
                    changed << item;
                }
            }

            return changed;
        }
    }
}
