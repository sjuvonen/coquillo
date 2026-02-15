#ifndef COQUILLO_CORE_TAG_H
#define COQUILLO_CORE_TAG_H

namespace coquillo {
enum TagClass { ID3v1, ID3v2, XiphComment };

struct Tag {
    Tag(TagClass type);
    const TagClass type;
};
} // namespace coquillo

#endif