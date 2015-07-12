#ifndef COQUILLO_METADATA_ITEMDATAROLES_H
#define COQUILLO_METADATA_ITEMDATAROLES_H

namespace Coquillo {
    namespace MetaData {
        namespace ModelData {
            enum ItemDataRole {
                FilePathRole = Qt::UserRole+1,
                FileNameRole = Qt::UserRole+2,
                ModifiedRole,
                FieldNameRole,
                NamedRowDataRole,
                MetaDataRole,
                AudioPropertiesRole
            };
        }
    }
}

#endif
