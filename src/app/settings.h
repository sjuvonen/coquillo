#ifndef COQUILLO_SETTINGS_H
#define COQUILLO_SETTINGS_H

#include <QObject>
#include <QVariantMap>

namespace Coquillo {
class Settings : public QObject {
  public:
    Settings(QObject *parent = nullptr);
    ~Settings();

    QSize mainWindowSize() const;
    bool mainWindowMaximized() const;

    void set(const QString key, const QVariant &value);

  private:
    void read();
    void write();

    bool dirty;
    QVariantMap values;
};
} // namespace Coquillo

#endif