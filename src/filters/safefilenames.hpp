#ifndef COQUILLO_FILTER_SAFEFILENAMES_H
#define COQUILLO_FILTER_SAFEFILENAMES_H

namespace Coquillo {
    namespace MetaData {
        class MetaData;
    }

    namespace Filter {
        class SafeFilenames {
            public:
                MetaData::MetaData filter(MetaData::MetaData data) const;
        };
    }
}

#endif
