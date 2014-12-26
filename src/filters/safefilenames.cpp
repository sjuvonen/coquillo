
#include "metadata/metadata.h"
#include "safefilenames.h"

namespace Coquillo {
    namespace Filter {
        MetaData::MetaData SafeFilenames::filter(MetaData::MetaData data) const {
            const QString chars = QRegExp::escape("$%-_@~`!(){}^#&");
            const QRegExp filter("[a-z " + chars + "]");
            data.setPath(data.path().remove(filter));
            return data;
        }
    }
}
