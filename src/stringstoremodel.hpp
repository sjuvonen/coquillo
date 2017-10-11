#ifndef COQUILLO_STRINGSTOREMODEL_H
#define COQUILLO_STRINGSTOREMODEL_H

#include <QPointer>
#include <QIdentityProxyModel>

class QSettings;

namespace Coquillo {
    class StringStoreModel : public QIdentityProxyModel {
        Q_OBJECT

        public:
            StringStoreModel(QObject * parent = 0);
            StringStoreModel(const QString & key, QObject * parent = 0);
            StringStoreModel(const QString & key, int cols, QObject * parent = 0);
            void setColumnCount(int count);
            void setKey(const QString & key);
            inline QString key() const { return _key; }
            void setLimit(int limit);
            inline int limit() const { return _limit; }
            void setDuplicatesAllowed(bool state);
            inline bool allowDuplicates() const { return _duplicates; }

            bool setData(const QModelIndex & idx, const QVariant & value, int role = Qt::EditRole);

        public slots:
            void read();
            bool submit();

        private slots:
            void filterInserted(const QModelIndex & parent, int start, int end);

        private:
            void filterDuplicates(const QModelIndex & new_idx);
            int findValue(const QVariant & value, const QModelIndex & start) const;
            void init(int cols);
            bool _duplicates;
            int _limit;
            QString _key;
    };
}

#endif
