#include "history.h"
#include <qsettings.h>

namespace Coquillo {
History::History(QObject *parent) : QObject(parent) { load(); }

void History::add(const QString &path) {
    history.prepend(path);
    history = history.sliced(10);
}

void History::remove(int index) { history.removeAt(index); }

void History::load() {
    QSettings settings;

    int size = settings.beginReadArray("History");

    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);

        history << settings.value("Path").toString();
    }

    settings.endArray();
}

void History::save() {
    QSettings settings;

    settings.beginWriteArray("History", history.size());

    for (int i = 0; i < history.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("Path", history[i]);
    }

    settings.endArray();
}
} // namespace Coquillo