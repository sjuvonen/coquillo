
#include <QDebug>
#include <QRegularExpression>
#include <QVariant>

#include "patterns.hpp"

namespace Coquillo {
    namespace Processor {
        Patterns::Patterns() {
            _symbols = {
                {"album", "%b"},
                {"artist", "%a"},
                {"comment", "%c"},
                {"composer", "%p"},
                {"disc", "%d"},
                {"genre", "%g"},
                {"ignore", "%i"},
                {"number", "%n"},
                {"original_artist", "%q"},
                {"padded_disc", "%D"},
                {"padded_number", "%N"},
                {"padded_year", "%Y"},
                {"title", "%t"},
                {"year", "%y"}
            };
        }

        Patterns::Patterns(const Symbols & symbols)
        : _symbols(symbols) {

        }

        QString Patterns::compile(const QString & pattern, const QVariantMap & data) const {
            QString result(pattern);
            const QRegularExpression dir_separator("[\\\\/]");

            foreach (const QString key, _symbols.keys()) {
                const QString value = data.value(key).toString().replace(dir_separator, "");
                const QString rx_pattern = QString("(?<!%)%1").arg(_symbols[key]);
                const QRegularExpression rx(rx_pattern);
                result.replace(rx, value);
            }

            return result;
        }

        QVariantMap Patterns::extract(const QString & pattern, const QString & string) const {
            const QString rx_pattern = patternToRegex(pattern);
            const QRegularExpression rx(rx_pattern, QRegularExpression::CaseInsensitiveOption);
            const QStringList matches = rx.match(string).capturedTexts();
            const QStringList symbols = extractSymbols(pattern);
            QVariantMap values;

            for (int i = 1; i < matches.count(); i++) {
                const QString key = _symbols.key(symbols[i-1]);
                values[key] = matches[i];
            }

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
