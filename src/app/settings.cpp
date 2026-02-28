#include "settings.h"
#include <QDebug>
#include <QSettings>
#include <QSize>
#include <QStandardPaths>
#include <QTimer>

namespace Coquillo {
Settings::Settings(QObject *parent)
    : QObject(parent), dirty(false),
      values({
          {"DefaultLocation",
           QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first()},
          {"DeleteEmptyDirs", true},
          {"NumberPadWidth", 2},
          {"RecursiveScan", true},
          {"Compat/FlacEnableId3v2", false},
          {"Compat/MpegEnableId3v1", false},
          {"Filter/SafeFilenames", false},
          {"Filter/ScaleImages", false},
          {"Filter/ScaleImagesWidth", 200},
          {"Filter/ScaleImagesHeight", 200},
          {"Filter/ParseDiscNumber", false},
          {"UI/Size", QSize(1200, 800)},
          {"UI/Maximized", false},
      }) {
    read();
}

Settings::~Settings() {
    if (dirty) {
        write();
    }
}

QSize Settings::mainWindowSize() const { return values["UI/Size"].toSize(); }
bool Settings::mainWindowMaximized() const { return values["UI/Maximized"].toBool(); }

void Settings::set(const QString key, const QVariant &value) {
    if (!values.contains(key)) {
        qWarning() << "Cannot update invalid configuration key:" << key;
    } else {
        values[key] = value;

        if (!dirty) {
            dirty = true;

            QTimer::singleShot(100, this, &Settings::write);
        }
    }
}

void Settings::read() {
    QSettings store;

    foreach (const QString key, store.allKeys()) {
        if (!values.contains(key)) {
            qDebug() << "Ignoring invalid key from found in user configuration:" << key;
        } else {
            values[key] = store.value(key);
        }
    }
}

void Settings::write() {
    if (dirty) {
        QSettings store;
        dirty = false;

        for (const auto key : values.keys()) {
            store.setValue(key, values[key]);
        }
    }
}

} // namespace Coquillo