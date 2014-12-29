
#include <QVariantMap>
#include <taglib/tag.h>
#include "dummy.h"

#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            Dummy::Dummy(TagLib::Tag * tag)
            : AbstractTag(tag) {

            }

            QVariantMap Dummy::read() const {
                return AbstractTag::readCommon();
            }

            void Dummy::write(const QVariantMap & data) {

            }
        }
    }
}
