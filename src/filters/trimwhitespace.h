#ifndef COQUILLO_FILTER_TRIMWHITESPACE_H
#define COQUILLO_FILTER_TRIMWHITESPACE_H

#include "abstractfilter.h"

namespace Coquillo {
    namespace Filter {
        class TrimWhiteSpace : public AbstractFilter {
            public:
                MetaData::MetaData filter(MetaData::MetaData data) const;
        };
    }
}

#endif
