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
            void setStorage(QSettings * settings);
            QSettings * storage() const;

        public slots:
            void read();
            bool submit();

        private:
            void init(int cols);
            int _limit;
            QPointer<QSettings> _storage;
            QString _key;
    };
}

#endif
