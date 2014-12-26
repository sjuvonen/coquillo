
#include "metadata/metadata.h"
#include "parsediscnumber.h"

namespace Coquillo {
    namespace Filter {
        MetaData::MetaData ParseDiscNumber::filter(MetaData::MetaData data) const {
            if (data.value("disc").toInt() == 0) {
                const QStringList patterns = {
                    "(?:\\s*)\\((?:Disc|CD)(?:\\s*)(\\d+)\\)(?:\\s*)$",
                };
                QString album = data.value("album").toString();

                foreach (const QString pattern, patterns) {
                    QRegExp rx(pattern, Qt::CaseInsensitive);

                    if (album.contains(rx)) {
                        data.insert("album", album.remove(rx));
                        data.insert("disc", rx.cap(1).toUInt());
                        return data;
                    }
                }
            }
            return data;
        }
    }
}
