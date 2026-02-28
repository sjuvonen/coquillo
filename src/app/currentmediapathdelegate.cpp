#include "currentmediapathdelegate.h"
#include "mediastoragemodelcolumns.h"

namespace Coquillo {
class CurrentMediaPathDelegatePrivate {
  public:
};

CurrentMediaPathDelegate::CurrentMediaPathDelegate(QObject *parent) : QObject(parent) {}

void CurrentMediaPathDelegate::setSelectionNotifier(SelectionNotifier *selectionNotifier) {
    _selectionNotifier = selectionNotifier;

    connect(_selectionNotifier, &SelectionNotifier::selectionChanged, [this]() {
        QStringList paths;
        const QString currentPath = _comboBox->currentText();

        for (int row : _selectionNotifier->selection()) {
            const auto path = _selectionNotifier->model()
                                  ->index(row, MediaStorageModelColumns::pathColumn())
                                  .data()
                                  .toString();

            paths << path;
        }

        std::sort(paths.begin(), paths.end());

        _comboBox->clear();
        _comboBox->addItems(paths);
        _comboBox->setCurrentText(currentPath);
    });

    connect(_selectionNotifier, &SelectionNotifier::currentChanged, [this](int row) {
        if (row == -1) {
            _comboBox->setDisabled(true);
        } else {
            _comboBox->setEnabled(true);

            const auto path = _selectionNotifier->model()
                                  ->index(row, MediaStorageModelColumns::pathColumn())
                                  .data()
                                  .toString();

            _comboBox->setCurrentText(path);
        }
    });
}

void CurrentMediaPathDelegate::setComboBox(QComboBox *comboBox) {
    _comboBox = comboBox;

    connect(_comboBox, &QComboBox::currentTextChanged, [=](const QString &newPath) {
        for (int row : _selectionNotifier->selection()) {
            const auto path = _selectionNotifier->model()
                                  ->index(row, MediaStorageModelColumns::pathColumn())
                                  .data()
                                  .toString();

            if (newPath == path) {
                _selectionNotifier->setCurrent(row);
                break;
            }
        }
    });
}
} // namespace Coquillo