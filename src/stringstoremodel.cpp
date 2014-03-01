
#include <QDebug>
#include <QSettings>
#include <QStandardItemModel>
#include "stringstoremodel.h"

namespace Coquillo {
    StringStoreModel::StringStoreModel(QObject * parent)
    : QIdentityProxyModel(parent), _duplicates(false), _limit(10) {
        init(1);
    }

    StringStoreModel::StringStoreModel(const QString & key, QObject * parent)
    : QIdentityProxyModel(parent), _duplicates(false), _limit(10), _key(key) {
        init(1);
        read();
    }

    StringStoreModel::StringStoreModel(const QString & key, int cols, QObject * parent)
    : QIdentityProxyModel(parent), _duplicates(false), _limit(10), _key(key) {
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

    void StringStoreModel::setDuplicatesAllowed(bool state) {
        if (state && !_duplicates) {
            // filter duplicates
        }
        _duplicates = state;
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

    bool StringStoreModel::setData(const QModelIndex & idx, const QVariant & value, int role) {
        bool state = QIdentityProxyModel::setData(idx, value, role);
        if (!allowDuplicates()) {
            filterDuplicates(idx);
        }
        return state;
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

    void StringStoreModel::filterDuplicates(const QModelIndex & new_idx) {
        const QModelIndex start = new_idx.sibling(new_idx.row() + 1, new_idx.column());
        int old = findValue(new_idx.data(), start);

        if (old != -1 && old != new_idx.row()) {
            removeRow(old);
        }
    }

    void StringStoreModel::filterInserted(const QModelIndex & parent, int start, int end) {
        QList<QPersistentModelIndex> cache;
        for (int i = start; i <= end; i++) {
            cache << QPersistentModelIndex(index(i, 0, parent));
        }

        foreach (const QPersistentModelIndex idx, cache) {
            if (idx.isValid()) {
                filterDuplicates(index(idx.row(), idx.column(), idx.parent()));
            }
        }
    }

    int StringStoreModel::findValue(const QVariant & value, const QModelIndex & start) const {
        const QModelIndexList matches = match(start, Qt::DisplayRole, value, 1, Qt::MatchFixedString | Qt::MatchWrap);

        qDebug() << "match" << value.toString() << matches.count() << matches.value(0).row();
        return matches.value(0).row();
    }

    void StringStoreModel::init(int columns) {
        QStandardItemModel * backend = new QStandardItemModel;
        backend->setColumnCount(columns);
        setSourceModel(backend);
//         connect(this, SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(filterInserted(QModelIndex, int, int)));
    }
}
