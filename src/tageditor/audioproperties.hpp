#ifndef COQUILLO_TAGEDITOR_AUDIOPROPERTIES_H
#define COQUILLO_TAGEDITOR_AUDIOPROPERTIES_H

#include <QWidget>

namespace Ui {
    class AudioProperties;
};

namespace Coquillo {
    namespace TagEditor {
        class AudioProperties : public QWidget {
            Q_OBJECT

            public:
                AudioProperties(QWidget * parent = 0);
                ~AudioProperties();

                void setData(const QVariantMap & props);

            private:
                Ui::AudioProperties * _ui;

        };
    }
}

#endif
