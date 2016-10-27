
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/urllinkframe.h>
#include "id3v2tag.hpp"

#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            QVariantHash Id3v2Tag::read(const TagLib::ID3v2::Tag * tag) const {
                QVariantHash data;
                const auto frames = tag->frameListMap();
                const QStringList ignored = {"APIC"};

                for (auto i = frames.begin(); i != frames.end(); i++) {
                    const QString field = i->first.data();

                    if (!ignored.contains(field)) {
                        // QVariantList values;

                        for (auto j = i->second.begin(); j != i->second.end();j++) {
                            // values << T2QString((*j)->toString());
                            if (field == "WXXX") {
                                const QRegExp prefix("^\\[\\] ");
                                data.insertMulti(field, T2QString((*j)->toString()).remove(prefix));
                            } else {
                                data.insertMulti(field, T2QString((*j)->toString()));
                            }
                        }

                        // data.insert(field, values);
                    }
                }

                if (data.contains("TYER")) {
                    data["TDRC"] = data.take("TYER");
                }

                // if (data.contains("WXXX")) {
                //     // Strip leading "[] "
                //     const QRegExp prefix("^\\[\\] ");
                //     data["WXXX"][0] = data["WXXX"].first().toString().remove(prefix);
                // }

                return data;
            }
        }
    }
}
