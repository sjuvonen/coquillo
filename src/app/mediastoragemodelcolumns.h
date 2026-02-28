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

    inline static int pathColumn();
    static int column(const QString &field);
    static int size();

  private:
    static QStringList labels;
    static QStringList fields;
};

int MediaStorageModelColumns::pathColumn() { return 1; }
} // namespace Coquillo

#endif