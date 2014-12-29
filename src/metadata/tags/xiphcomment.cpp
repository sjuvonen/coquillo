
#include <QVariantMap>
#include <taglib/xiphcomment.h>
#include "xiphcomment.h"

#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace MetaData {
        namespace Container {

            XiphComment::XiphComment(TagLib::Ogg::XiphComment * tag)
            : AbstractTag(tag) {

            }

            QVariantMap XiphComment::read() const {
                QVariantMap data = readCommon();
                const auto tag = dynamic_cast<TagLib::Ogg::XiphComment*>(_tag);
                const auto fields = tag->fieldListMap();

                if (tag->contains("COMPOSER")) {
                    data.insert("composer", T2QString(fields["COMPOSER"].front()));
                }

                if (tag->contains("PERFORMER")) {
                    data.insert("original_artist", T2QString(fields["PERFORMER"].front()));
                }

                if (tag->contains("LICENSE")) {
                    data.insert("url", T2QString(fields["LICENSE"].front()));
                }

                if (tag->contains("DISCNUMBER")) {
                    data.insert("disc", T2QString(fields["DISCNUMBER"].front()).toInt());
                }

                if (tag->contains("TRACKTOTAL")) {
                    data.insert("total", T2QString(fields["TRACKTOTAL"].front()).toInt());
                }

                if (tag->contains("ENCODED-BY")) {
                    data.insert("encoder", T2QString(fields["ENCODED-BY"].front()));
                }

                if (tag->contains("DESCRIPTION")) {
                    data.insert("comment", T2QString(fields["DESCRIPTION"].front()));
                }

                for (auto i = fields.begin(); i != fields.end(); i++) {
                    const QString field = T2QString(i->first);
                    QVariantList values;
                    
                    for (auto j = i->second.begin(); j != i->second.end(); j++) {
                        values << T2QString(*j);
                    }

                    data.insert(field, values);
                }

                return data;
            }

            void XiphComment::write(const QVariantMap & data) {

            }
        }
    }
}
