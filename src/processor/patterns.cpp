
#include <QDebug>
#include <QRegularExpression>
#include <QVariant>

#include "patterns.h"

namespace Coquillo {
    namespace Processor {
        Patterns::Patterns() {
            _symbols["artist"] = "%a";
            _symbols["album"] = "%b";
            _symbols["title"] = "%t";
            _symbols["number"] = "%n";
            _symbols["disc"] = "%d";
            _symbols["genre"] = "%g";
            _symbols["comment"] = "%c";
            _symbols["original_artist"] = "%q";
            _symbols["ignore"] = "%i";
        }

        Patterns::Patterns(const Symbols & symbols)
        : _symbols(symbols) {

        }

        QString Patterns::compile(const QString & pattern, const QVariantHash & data) const {
            QString result(pattern);

            foreach (const QString key, _symbols.keys()) {
                const QString value = data.value(key).toString();
                const QString rx_pattern = QString("(?<!%)%1").arg(_symbols[key]);
                const QRegularExpression rx(rx_pattern, QRegularExpression::CaseInsensitiveOption);
                result.replace(rx, value);
            }

            return result;
        }

        QVariantHash Patterns::extract(const QString & pattern, const QString & string) const {
            const QString rx_pattern = patternToRegex(pattern);
            const QRegularExpression rx(rx_pattern, QRegularExpression::CaseInsensitiveOption);
            const QStringList matches = rx.match(string).capturedTexts();
            const QStringList symbols = extractSymbols(pattern);
            QVariantHash values;

            for (int i = 1; i < matches.count(); i++) {
                const QString key = _symbols.key(symbols[i-1]);
                values[key] = matches[i];
            }

//             qDebug() << extractSymbols(pattern);
//             qDebug() << patternToRegex(pattern);
//             qDebug() << rx.match(string).capturedTexts();

            return values;
        }

        QString Patterns::patternToRegex(QString pattern) const {
            const QString symbols = QStringList(_symbols.values()).join('|');
            const QString rx_pattern = QString("(?<!%)%1").arg(symbols);
            const QRegularExpression rx(rx_pattern, QRegularExpression::CaseInsensitiveOption);
            pattern.replace(rx, "(.+)");
            return QString("^%1$").arg(pattern);
        }

        QStringList Patterns::extractSymbols(const QString & pattern) const {
            const QString symbols = QStringList(_symbols.values()).join('|');
            const QString rx_pattern = QString("(?<!%)%1").arg(symbols);
            const QRegularExpression rx(rx_pattern, QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatchIterator i = rx.globalMatch(pattern);
            QStringList matches;

            while (i.hasNext()) {
                matches << i.next().captured(0);
            }

            return matches;
        }
    }
}
