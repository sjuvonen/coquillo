#ifndef COQUILLO_TAGS_CONTAINER_H
#define COQUILLO_TAGS_CONTAINER_H

#include "tag.hpp"

namespace Coquillo {
    namespace Tags {
        class Container {
            public:
                Container(const QString & path);
                void addTag(const Tag & tag);
                void setPrimaryTag(const QString & id);
                QVariant value(const QString & field) const;

                inline QString path() const { return _path; }
                inline uint id() const { return _id; }

            private:
                const uint _id;
                QString _path;
                QString _primary;
                QHash<QString, Tag> _tags;
        };
    }
}
#endif
