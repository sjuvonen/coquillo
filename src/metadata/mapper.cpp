
#include "metadata.h"
#include "mapper.h"

namespace Coquillo {
    namespace MetaData {

        QString Mapper::mapToSource(const QString & name) const {
            return _map.value(name, name);
        }

        QString Mapper::mapFromSource(const QString & name) const {
            return _map.key(name, name);
        }

        QVariant Mapper::value(const QVariantMap & tag, const QString & field) const {
            return tag.value(mapToSource(field));
        }

        void Mapper::insert(QVariantMap & tag, const QString & field, const QVariant & value) {
            tag.insert(mapToSource(field), value);
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
//                 {"total", "TRACKTOTAL"},
                {"url", "WXXX"},
                {"year", "TYER"},
            };
        }

        QVariant Id3v2Mapper::value(const QVariantMap & tag, const QString & field) const {
            if (field == "number" or field == "total") {
                const QString trck = tag.value("TRCK").toList().value(0, "0/0").toString();
                int pos = (field == "number" ? 0 : 1);
                return trck.section('/', pos, pos).toUInt();
            } else {
                const QString src = mapToSource(field);
                return tag.value(src).toList().value(0);
            }
        }

        void Id3v2Mapper::insert(QVariantMap & tag, const QString & field, const QVariant & value) {
            if (field == "number" or field == "total") {
                const QString trck = tag.value("TRCK").toList().value(0, "0/0").toString();
                QString number = field == "number" ? value.toString() : trck.section('/', 0, 0);
                QString total = field == "total" ? value.toString() : trck.section('/', 1, 1);
                QVariantList list = { QString("%1/%2").arg(number, total) };
                tag.insert("TRCK", list);
            } else {
                const QString src = mapToSource(field);
                QVariantList values = tag.value(src).toList();
                if (values.isEmpty()) {
                    values.append(value);
                } else {
                    values.replace(0, value);
                }
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
//                 {"year", "DATE"},
            };
        }

        QVariant XiphMapper::value(const QVariantMap & tag, const QString & field) const {
            return tag.value(mapToSource(field));
        }

        void XiphMapper::insert(QVariantMap & tag, const QString & field, const QVariant & value) {
            tag.insert(mapToSource(field), value);
        }

        /*
         * NOTE: Initialize mappers for MetaData class
         */
        QHash<QString, Mapper*> MetaData::mappers = {
            {"default", new Mapper},
            {"id3v2", new Id3v2Mapper},
            {"xiph", new XiphMapper},
        };
    }
};
