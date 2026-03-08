#ifndef COQUILLO_SETTINGS_H
#define COQUILLO_SETTINGS_H

#include <QByteArray>
#include <QObject>
#include <QVariantMap>

namespace Coquillo {
class Settings : public QObject {
  public:
    Settings(QObject *parent = nullptr);
    ~Settings();

    const QSize mainWindowSize() const;
    bool mainWindowMaximized() const;
    bool toolBarsLocked() const;
    bool menuBarVisible() const;
    bool statusBarVisible() const;

    const QByteArray mainWindowState() const;
    const QByteArray mediaViewHeaderState() const;
    const QByteArray mainSplitterState() const;

    const QString location() const;

    void set(const QString key, const QVariant &value);

  private:
    void read();
    void write();
    void migrate();

    bool dirty;
    QVariantMap values;
};
} // namespace Coquillo

#endif