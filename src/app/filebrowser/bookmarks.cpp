#include "bookmarks.h"
#include <qsettings.h>

namespace Coquillo {

Bookmark::Bookmark(const QString &path, const QString &label) : path(path), label(label) {}

Bookmarks::Bookmarks(QObject *parent) : QObject(parent) {
    loadLegacy();
    load();
}

void Bookmarks::add(const QString &path, const QString &label) {
    _entries << std::move(Bookmark(path, label));
}

void Bookmarks::remove(int index) { _entries.remove(index); }

void Bookmarks::loadLegacy() {
    QSettings settings;

    if (settings.contains("history")) {
        settings.beginGroup("history");
        int size = settings.beginReadArray("bookmarks");

        for (int i = 0; i < size; i++) {
            settings.setArrayIndex(i);

            const auto label = settings.value("0").toString();
            const auto path = settings.value("1").toString();

            add(path, label);
        }

        settings.endArray();
        settings.endGroup();

        settings.remove("history");

        save();
    }
}

void Bookmarks::load() {
    QSettings settings;

    _entries.clear();
    int size = settings.beginReadArray("Bookmarks");

    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);

        const auto path = settings.value("Path").toString();
        const auto label = settings.value("Label").toString();

        add(path, label);
    }

    settings.endArray();
}

void Bookmarks::save() {
    QSettings settings;

    settings.beginWriteArray("Bookmarks", _entries.size());

    for (int i = 0; i < _entries.size(); i++) {
        settings.setArrayIndex(i);

        settings.setValue("Path", _entries[i].path);
        settings.setValue("Label", _entries[i].label);
    }

    settings.endArray();
}
} // namespace Coquillo