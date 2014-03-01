
#include <QDebug>
#include <QSettings>
#include <QStandardItemModel>
#include "stringstoremodel.h"

namespace Coquillo {
    StringStoreModel::StringStoreModel(QObject * parent)
    : QIdentityProxyModel(parent), _limit(10) {
        init(1);
    }

    StringStoreModel::StringStoreModel(const QString & key, QObject * parent)
    : QIdentityProxyModel(parent), _limit(10), _key(key) {
        init(1);
        read();
    }

    StringStoreModel::StringStoreModel(const QString & key, int cols, QObject * parent)
    : QIdentityProxyModel(parent), _limit(10), _key(key) {
        init(cols);
        read();
    }

    void StringStoreModel::setColumnCount(int count) {
        QStandardItemModel * backend = qobject_cast<QStandardItemModel*>(sourceModel());

        if (backend) {
            backend->setColumnCount(count);
        }
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

            for (int j = 0; j < columnCount(); j++) {
                const QString key = QString("item-%1").arg(j);
                setData(index(i, j), settings->value(key));
            }
        }

        settings->endArray();
    }

    bool StringStoreModel::submit() {
        if (!storage() || key().isNull()) {
            return false;
        }

        qDebug() << "save history" << rowCount();

        QSettings * settings = storage();
        settings->beginWriteArray(key());
        settings->remove("");

        for (int i = 0; i < rowCount(); i++) {
            settings->setArrayIndex(i);

            for (int j = 0; j < columnCount(); j++) {
                const QString key = QString("item-%1").arg(j);
                settings->setValue(key, index(i, j).data());
            }
        }

        settings->endArray();
        settings->sync();

        return settings->status() == QSettings::NoError;
    }

    void StringStoreModel::init(int columns) {
        QStandardItemModel * backend = new QStandardItemModel;
        backend->setColumnCount(columns);
        setSourceModel(backend);
    }
}
