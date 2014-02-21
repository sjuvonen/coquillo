#ifndef COQUILLO_HISTORYMODEL_H
#define COQUILLO_HISTORYMODEL_H

#include <QStringListModel>

class QSettings;

namespace Coquillo {
    class HistoryModel : public QStringListModel {
        Q_OBJECT

        public:
            HistoryModel(QObject * parent = 0);
            HistoryModel(const QString & key, QObject * parent = 0);
            void setKey(const QString & key);
            inline QString key() const { return _key; }
            void setLimit(int limit);
            inline int limit() const { return _limit; }
            void setStorage(QSettings * settings);
            inline QSettings * storage() const { return _storage; }

        public slots:
            void read();
            bool submit();

        private:
            QSettings * _storage;
            int _limit;
            QString _key;
    };
}

#endif
