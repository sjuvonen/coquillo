#ifndef COQUILLO_TAGS_CONTAINER_H
#define COQUILLO_TAGS_CONTAINER_H

#include "tag.hpp"

namespace Coquillo {
    namespace Tags {
        class Container {
            public:
                Container(const QString & path);
                void addTag(const Tag & tag);
                QVariant value(const QString & field) const;

                const uint id;
                
            private:
                QString _path;
                QString _primary;
                QHash<QString, Tag> _tags;
        };
    }
}
#endif
