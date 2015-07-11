
#include "metadata.hpp"
#include "mapper.hpp"

namespace Coquillo {
    namespace MetaData {

        QString Mapper::mapToSource(const QString & name) const {
            return _map.value(name, name);
        }

        QString Mapper::mapFromSource(const QString & name) const {
            return _map.key(name, name);
        }

        QVariant Mapper::value(const Tag & tag, const QString & field) const {
            return tag.value(mapToSource(field)).value(0);
        }

        QVariant Mapper::take(Tag & tag, const QString & field) const {
            QVariant val = value(tag, field);
            tag.remove(mapToSource(field));
            return val;
        }

        void Mapper::insert(Tag & tag, const QString & field, const QVariant & value) {
            tag[mapToSource(field)] = {value};
        }

        Id3v2Mapper::Id3v2Mapper() : Mapper() {
            _map = {
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
                {"year", "TDRL"},
            };
        }

        QVariant Id3v2Mapper::value(const Tag & tag, const QString & field) const {
            if (field == "number" or field == "total") {
                const QString trck = tag.value("TRCK").value(0, "0/0").toString();
                int pos = (field == "number" ? 0 : 1);
                return trck.section('/', pos, pos).toUInt();
            } else {
                const QString src = mapToSource(field);
                return tag.value(src).value(0);
            }
        }

        void Id3v2Mapper::insert(Tag & tag, const QString & field, const QVariant & value) {
            if (field == "number" or field == "total") {
                const QString trck = tag.value("TRCK").value(0, "0/0").toString();
                const QString number = field == "number" ? value.toString() : trck.section('/', 0, 0);
                const QString total = field == "total" ? value.toString() : trck.section('/', 1, 1);
                const QVariantList list = { QString("%1/%2").arg(number, total) };
                tag.insert("TRCK", list);
            } else {
                const QString src = mapToSource(field);
                const QVariantList values = QVariantList()
                    << value
                    << tag.value(src).mid(1);
                tag.insert(src, values);
            }
        }



        XiphMapper::XiphMapper() : Mapper() {
            _map = {
                {"album", "ALBUM"},
                {"album_artist", "ALBUM ARTIST"},
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
            };
        }

        QVariant XiphMapper::value(const Tag & tag, const QString & field) const {
            const QString src = mapToSource(field);
            return tag.value(src).value(0);
        }

        void XiphMapper::insert(Tag & tag, const QString & field, const QVariant & value) {
            const QString src = mapToSource(field);
            const QVariantList values = QVariantList()
                << value
                << tag.value(src).mid(1);
            tag.insert(src, values);
        }

        /*
         * NOTE: Initialize mappers for MetaData class
         */
        QHash<QString, Mapper*> MetaData::mappers = {
            {"unknown", new Mapper},
            {"id3v2", new Id3v2Mapper},
            {"xiph", new XiphMapper},
        };
    }
};
