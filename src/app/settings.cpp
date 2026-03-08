#include "settings.h"
#include <QDebug>
#include <QSettings>
#include <QSize>
#include <QStandardPaths>
#include <QTimer>
#include <qstringview.h>

namespace Coquillo {
bool match_prefix(const QStringList &prefixes, const QString &value) {

    for (const auto prefix : prefixes) {
        if (value.startsWith(prefix)) {
            return true;
        }
    }

    return false;
}

Settings::Settings(QObject *parent)
    : QObject(parent), dirty(false),
      values({
          {"SettingsVersion", QVariant()},
          {"DefaultLocation",
           QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first()},
          {"Compat/FlacEnableId3v2", false},
          {"Compat/MpegEnableId3v1", false},
          {"DeleteEmptyDirs", true},
          {"Filter/ParseDiscNumber", false},
          {"Filter/SafeFilenames", false},
          {"Filter/ScaleImages", false},
          {"Filter/ScaleImagesHeight", 200},
          {"Filter/ScaleImagesWidth", 200},
          {"NumberPadWidth", 2},
          {"RecursiveScan", true},
          {"UI/Header", QByteArray()},
          {"UI/LockToolBars", false},
          {"UI/Maximized", false},
          {"UI/MenuBar", false},
          {"UI/Size", QSize(1200, 800)},
          {"UI/Splitter", QByteArray()},
          {"UI/State", QByteArray()},
          {"UI/StatusBar", true},
      }) {

    read();
    migrate();
}

Settings::~Settings() {
    if (dirty) {
        write();
    }
}

const QSize Settings::mainWindowSize() const { return values["UI/Size"].toSize(); }

bool Settings::mainWindowMaximized() const { return values["UI/Maximized"].toBool(); }

bool Settings::toolBarsLocked() const { return values["UI/LockToolBars"].toBool(); }

bool Settings::statusBarVisible() const { return values["UI/StatusBar"].toBool(); }

bool Settings::menuBarVisible() const { return values["UI/MenuBar"].toBool(); }

const QByteArray Settings::mainWindowState() const { return values["UI/State"].toByteArray(); }

const QByteArray Settings::mediaViewHeaderState() const {
    return values["UI/Header"].toByteArray();
}

const QByteArray Settings::mainSplitterState() const { return values["UI/Splitter"].toByteArray(); }

const QString Settings::location() const { return values["DefaultLocation"].toString(); }

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

    // "history/": prefix in Coquillo 2.
    // "Bookmarks/": prefix in Coquillo 3.
    const QStringList ignores = {"history/", "Bookmarks/"};

    foreach (const QString key, store.allKeys()) {
        if (match_prefix(ignores, key)) {
            continue;
        }

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

void Settings::migrate() {
    if (values["SettingsVersion"].isNull()) {
        values["SettingsVersion"] = "3.0";

        // NOTE: Main window internal settings have changed: reset.
        values["UI/State"] = QByteArray();

        // NOTE: Splitter internal settings have changed: reset.
        values["UI/Splitter"] = QByteArray();

        // NOTE: Header columns have changed: reset.
        values["UI/Header"] = QByteArray();
    }
}

} // namespace Coquillo