#ifndef COQUILLO_FILTER_PARSEDISCNUMBER_H
#define COQUILLO_FILTER_PARSEDISCNUMBER_H

#include "abstractfilter.hpp"

namespace Coquillo {
    namespace Filter {
        class ParseDiscNumber : public AbstractFilter {
            public:
                MetaData::MetaData filter(MetaData::MetaData data) const;
        };
    }
}

#endif
