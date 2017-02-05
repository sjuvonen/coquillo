#ifndef COQUILLO_TAGS_TAGWRITER_H
#define COQUILLO_TAGS_TAGWRITER_H

#include <QObject>
#include <QRunnable>
#include "tagcontainer.hpp"

namespace Coquillo {
    namespace Tags {
        class Writer : public QObject {
            Q_OBJECT

            public:
                Writer(QObject * parent = 0);

                void write(const Container & items);
                void write(const QList<Container> & items);

            public slots:
                void abort();

            signals:
                void aborted();
                void finished();
                void progress(int);
                void started();

            private slots:
                void processItem();

            private:
                volatile bool _aborted = false;
                QList<Container> _items;
        };
    }
}

#endif
