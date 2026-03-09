#ifndef COQUILLO_MEDIASTORAGEMODELCOLUMNS_H
#define COQUILLO_MEDIASTORAGEMODELCOLUMNS_H

#include <QStringList>

namespace Coquillo {
class MediaStorageModelColumns {
  public:
    MediaStorageModelColumns();

    static const QString label(int column);
    static const QString labelFor(const QString &field);

    static const QString field(int column);
    static const QString fieldFor(const QString &label);

    inline static int indicatorColumn();
    inline static int pathColumn();
    inline static int imagesColumn();

    static int column(const QString &field);
    static int size();

  private:
    static QStringList labels;
    static QStringList fields;
};

int MediaStorageModelColumns::indicatorColumn() { return 0; }

int MediaStorageModelColumns::pathColumn() { return 1; }

int MediaStorageModelColumns::imagesColumn() { return 16; }
} // namespace Coquillo

#endif