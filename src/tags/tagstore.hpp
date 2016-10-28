#ifndef COQUILLO_TAGS_TAGSTORE_H
#define COQUILLO_TAGS_TAGSTORE_H

#include "tagcontainer.hpp"

namespace Coquillo {
    namespace Tags {
        class Store {
            public:
                Store();
                void add(const QVariantHash & file);
                void add(const QList<QVariantHash> & files);
                const Container & at(int pos) const;
                bool isModified(int pos) const;
                bool isFieldModified(int pos, const QString & field) const;

                void remove(int pos);

                inline int size() const { return _items.size(); }

            private:
                Tag::Mapping _fallback;
                QHash< QString, Tag::Mapping> _mappings;
                QList<Container> _items;
                QHash<uint, Container> _backup;
        };
    }
}

#endif
