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
                {"number", "TRACnumberKNUMBER"},
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
                    {"url", "LICENSE"},
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

            _items << item;
        }

        void Store::add(const QList<QVariantHash> & files) {
            foreach (const QVariantHash & file, files) {
                add(file);
            }
        }

        const Container & Store::at(int pos) const {
            return _items.at(pos);
        }

        bool Store::isModified(int pos) const {
            const Container & item = at(pos);
            return _backup.contains(item.id());
        }

        bool Store::isFieldModified(int pos, const QString & field) const {
            const Container & item = at(pos);
            if (_backup.contains(item.id())) {
                const Container & backup = _backup[item.id()];
                return item.value(field) == backup.value(field);
            }
            return false;
        }

        void Store::remove(int pos) {
            if (pos >= 0 && pos < size()) {
                const auto item = at(pos);
                _backup.remove(item.id());
                _items.removeAt(pos);
            }
        }
    }
}
