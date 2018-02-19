#ifndef COQUILLO_TAGS_TAGSTORE_H
#define COQUILLO_TAGS_TAGSTORE_H

#include <QObject>
#include "tagcontainer.hpp"

namespace Coquillo {
    namespace Tags {
        class Store : public QObject {
            Q_OBJECT

            public:
                Store(QObject * parent = nullptr);

                void add(const QVariantMap & file);
                void add(const QList<QVariantMap> & files);

                const Container at(int pos) const;

                bool isModified(int pos) const;
                bool isFieldModified(int pos, const QString & field) const;

                void reset();
                void commit();
                void remove(int pos);

                inline int size() const { return _items.size(); }

                bool rename(int pos, const QString & new_path);
                bool setValue(int pos, const QString & field, const QVariant & value);
                bool setValues(int pos, const QVariantMap & values);
                bool setImages(int pos, const QList<Image> & images);

                const QList<Container> items() const;
                QList<Container> changedItems() const;
                QMap<QString, Container> changedItemsMap() const;

            signals:
                void aboutToCommit();
                void committed();
                void progress(int);

            private slots:
                void preCommit();

            private:
                Container & ref(int pos);
                void backup(int pos);
                void backup(const Container & item);

                void writeToDisk();

                Tag::Mapping _fallback;
                QMap< QString, Tag::Mapping> _mappings;
                QList<Container> _items;
                QHash<uint, Container> _backup;
        };
    }
}

#endif
