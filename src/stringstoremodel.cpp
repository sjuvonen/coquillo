
#include <QDebug>
#include <QSettings>
#include "stringstoremodel.h"

namespace Coquillo {
    StringStoreModel::StringStoreModel(QObject * parent)
    : QStringListModel(parent), _limit(10) {

    }
    StringStoreModel::StringStoreModel(const QString & key, QObject * parent)
    : QStringListModel(parent), _limit(10), _key(key) {
        read();
    }

    void StringStoreModel::setKey(const QString & key) {
        if (key != _key) {
            _key = key;

            if (storage()) {
                read();
            }
        }
    }

    void StringStoreModel::setLimit(int limit) {
        _limit = limit;

        if (limit > rowCount()) {
            removeRows(limit, rowCount() - limit);
        }
    }

    void StringStoreModel::setStorage(QSettings * settings) {
        _storage = QPointer<QSettings>(settings);

        if (settings && !key().isNull()) {
            read();
        }
    }

    QSettings * StringStoreModel::storage() const {
        return _storage.data();
    }

    void StringStoreModel::read() {
        if (!storage() || key().isNull()) {
            return;
        }

        QSettings * settings = storage();
        int size = settings->beginReadArray(key());

        for (int i = 0; i < size; i++) {
            settings->setArrayIndex(i);
            insertRow(i);
            setData(index(i), settings->value("item"));
        }

        settings->endArray();
    }

    bool StringStoreModel::submit() {
        if (!storage() || key().isNull()) {
            return false;
        }

        qDebug() << "save history";

        QSettings * settings = storage();
        settings->beginWriteArray(key());
        settings->remove("");

        for (int i = 0; i < rowCount(); i++) {
            settings->setArrayIndex(i);
            settings->setValue("item", index(i).data());
        }

        settings->endArray();
        settings->sync();

        return settings->status() == QSettings::NoError;
    }
}
