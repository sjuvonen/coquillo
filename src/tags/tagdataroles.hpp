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
            ValuesMapRole,
            RootPathRole,
        };

        enum TagModelField {
            FeedbackField = 0,
            NumberField = 7,
            TrackCountField = 8,
            DiscNumberField = 9,
            PathField = 15,
            ImageField = 16
        };
    }
}

#endif
