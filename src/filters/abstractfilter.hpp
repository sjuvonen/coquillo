#ifndef COQUILLO_FILTER_ABSTRACTFILTER_H
#define COQUILLO_FILTER_ABSTRACTFILTER_H

namespace Coquillo {
    namespace MetaData {
        class MetaData;
    }

    namespace Filter {
        class AbstractFilter {
            public:
                AbstractFilter() { }
                virtual ~AbstractFilter() { }
                virtual MetaData::MetaData filter(MetaData::MetaData data) const = 0;
        };
    }
}

#endif
