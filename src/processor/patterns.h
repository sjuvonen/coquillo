#ifndef COQUILLO_PROCESSOR_PATTERNS_H
#define COQUILLO_PROCESSOR_PATTERNS_H

#include <QHash>
#include <QObject>

namespace Coquillo {
    namespace Processor {
        typedef QHash<QString, QString> Symbols;

        class Patterns : public QObject {
            Q_OBJECT

            public:
                Patterns(QObject * parent = 0);
                Patterns(const Symbols & symbols, QObject * parent = 0);

                /**
                 * Replace symbols in a string with real values
                 *
                 * Use for e.g. creating filenames from patterns
                 */
                QString compile(const QString & pattern, const QVariantHash & data) const;

                /**
                 * Read values from a string
                 */
                QVariantHash extract(const QString & pattern, const QString & string) const;

            private:
                QString patternToRegex(QString pattern) const;
                QStringList extractSymbols(const QString & pattern) const;
                Symbols _symbols;
        };
    }
}

#endif
