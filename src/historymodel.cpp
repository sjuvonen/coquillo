
#include <QDebug>
#include <QSettings>
#include "historymodel.h"

namespace Coquillo {
    HistoryModel::HistoryModel(QObject * parent)
    : QStringListModel(parent), _storage(0) {

    }
    HistoryModel::HistoryModel(const QString & key, QObject * parent)
    : QStringListModel(parent), _storage(0), _key(key) {
        read();
    }

    void HistoryModel::setKey(const QString & key) {
        if (key != _key) {
            _key = key;

            if (storage()) {
                read();
            }
        }
    }

    void HistoryModel::setLimit(int limit) {
        _limit = limit;

        if (limit > rowCount()) {
            removeRows(limit, rowCount() - limit);
        }
    }

    void HistoryModel::setStorage(QSettings * settings) {
        _storage = settings;

        if (settings && !key().isNull()) {
            read();
        }
    }

    void HistoryModel::read() {
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

    bool HistoryModel::submit() {
        if (!storage() || key().isNull()) {
            return false;
        }

        qDebug() << "submit";

        QSettings * settings = storage();
        settings->beginWriteArray(key());
        settings->clear();

        for (int i = 0; i < rowCount(); i++) {
            settings->setArrayIndex(i);
            settings->setValue("item", index(i).data());
        }

        settings->endArray();
        settings->sync();

        return settings->status() == QSettings::NoError;
    }
}
