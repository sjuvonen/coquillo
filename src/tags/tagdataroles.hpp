#ifndef COQUILLO_TAGS_TAGDATAROLES_H
#define COQUILLO_TAGS_TAGDATAROLES_H

#include <Qt>

namespace Coquillo {
    namespace Tags {
        enum TagDataRoles {
            FilePathRole = Qt::UserRole+1,
            FileNameRole = Qt::UserRole+2,
            ContainerRole,
            FieldModifiedStateRole,
            ItemModifiedStateRole,
            RootPathRole,
        };
    }
}

#endif
