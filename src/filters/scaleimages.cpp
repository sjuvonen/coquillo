
#include "metadata/imagecache.h"
#include "metadata/metadata.h"
#include "scaleimages.h"

namespace Coquillo {
    namespace Filter {
        MetaData::MetaData ScaleImages::filter(MetaData::MetaData data) const {
            auto cache = MetaData::ImageCache::instance();

            /*
             * Be lazy and call resize() on each image and let the cache check
             * if the image actually needs resizing.
             *
             * Running this on every MetaData item means lots of unnecessary work,
             * but let's misuse this great idea of unified filter API.
             *
             * This filter should, though, still check if passed metadata contains
             * and image that has been resized and then set the modification flag on.
             * (Once such modification flag is implemented!)
             */
            foreach (quint16 id, cache->ids()) {
                cache->resize(id, _size);
            }

            return data;
        }
    }
}
