#ifndef COQUILLO_FILTER_SCALEIMAGES_H
#define COQUILLO_FILTER_SCALEIMAGES_H

#include "abstractfilter.hpp"

namespace Coquillo {
    namespace Filter {
        class ScaleImages : public AbstractFilter {
            public:
                MetaData::MetaData filter(MetaData::MetaData data) const;

            private:
                QSize _size;
        };
    }
}

#endif
