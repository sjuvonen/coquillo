
#include <QDebug>
#include <QDir>

#include "purgedirsaftercommit.hpp"

namespace Coquillo {
    PurgeDirsAfterCommit::PurgeDirsAfterCommit(const QStringList & watched, QObject * parent)
    : QObject(parent), _dirs(watched) {

    }

    void PurgeDirsAfterCommit::purge() {
        _dirs.sort(Qt::CaseInsensitive);

        QString name;
        QDir dir;

        // for (QString path : _dirs) {

        for (auto i = _dirs.rbegin(); i != _dirs.rend(); i++) {
            dir.setPath(*i);

            do {
                name = dir.dirName();
            } while (dir.cdUp() && dir.rmdir(name));
        }

        _dirs.clear();
    }
}
