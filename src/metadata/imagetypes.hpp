#ifndef COQUILLO_METADATA_IMAGETYPES_H
#define COQUILLO_METADATA_IMAGETYPES_H

#include <QMap>
#include <QString>

namespace Coquillo {
    namespace MetaData {
        class ImageTypes {
            public:
                static QStringList types();

            private:
                static QMap<int, QString> _types;
        };
    }
}

#endif
