#ifndef COQUILLO_TAGS_CONTAINER_H
#define COQUILLO_TAGS_CONTAINER_H

#include "tag.hpp"

namespace Coquillo {
    namespace Tags {
        class Container {
            public:
                Container();
                Container(const QString & path);
                void addTag(const Tag & tag);
                void setPrimaryTag(const QString & id);
                QVariant value(const QString & field) const;

                inline QString primaryTag() const { return _primary; }
                inline QString path() const { return _path; }
                inline uint id() const { return _id; }
                inline bool isNull() const { return _path.isNull(); }

                const Tag tag(const QString & id) const;
                QList<Tag> tags() const;
                QStringList tagNames() const;

            private:
                const uint _id;
                QString _path;
                QString _primary;
                QHash<QString, Tag> _tags;
        };
    }
}

Q_DECLARE_METATYPE(Coquillo::Tags::Container);

#endif
