#ifndef COQUILLO_CORE_MEDIA_H
#define COQUILLO_CORE_MEDIA_H

#include <string>

namespace coquillo {
struct Media {
    Media(const std::string &path);

    const std::string path;
};
} // namespace coquillo

#endif