#ifndef COQUILLO_PROCESSOR_PATHS_H
#define COQUILLL_PROCESSOR_PATHS_H

#include <QVariantHash>

class QString;

namespace Coquillo {
    namespace Processor {
        class Paths {
            public:
                Paths();

                QVariantHash parseTagsFromPath(const QString & path, const QString & pattern) const;
                QString mergeFileNames(const QString & original, const QString & current) const;

        };
    }
}

#endif
