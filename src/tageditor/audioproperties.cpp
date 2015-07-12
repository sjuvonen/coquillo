
#include <QDebug>
#include <QTime>
#include "audioproperties.hpp"
#include "ui_audioproperties.h"

namespace Coquillo {
    namespace TagEditor {
        AudioProperties::AudioProperties(QWidget * parent)
        : QWidget(parent) {
            _ui = new Ui::AudioProperties;
            _ui->setupUi(this);
        }

        AudioProperties::~AudioProperties() {
            delete _ui;
        }

        void AudioProperties::setData(const QVariantMap & props) {
            int length = props.value("length").toInt();
            double filesize = props.value("filesize").value<qint64>() / 1000000.0;
            _ui->labelFilesize->setText(QString::number(filesize, 'f', 2) + " MB");
            _ui->labelBitrate->setText(props.value("bitrate").toString() + " kbps");
            _ui->labelLength->setText(QTime(length / 3600, length % 3600 / 60, length % 60).toString("mm:ss"));
            _ui->labelSamplerate->setText(props.value("samplerate").toString() + " Hz");
        }
    }
}
