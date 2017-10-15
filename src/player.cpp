#include <QMediaPlayer>

#include "player.hpp"
#include "ui_player.h"

namespace Coquillo {
    Player::Player(QWidget * parent)
    : QWidget(parent) {
        _ui = new Ui::Player;
        _ui->setupUi(this);

        _player = new QMediaPlayer(this);

        connect(_player, &QMediaPlayer::seekableChanged, _ui->sliderSeek, &QSlider::setEnabled);

        connect(_player, &QMediaPlayer::positionChanged, this, [this](qint64) {
            if (_player->duration() > 0) {
                int position = _player->position() / 1000;
                int duration = _player->duration() / 1000;

                int value = (double)position / duration * 100;

                _ui->sliderSeek->blockSignals(true);
                _ui->sliderSeek->setSliderPosition(value);
                _ui->sliderSeek->blockSignals(false);
            }

        });

        connect(_player, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State state) {
            // Prevent sigfault upon closing the app.
            if (isVisible()) {
                _ui->buttonPlay->setChecked(state == QMediaPlayer::PlayingState);
            }
        });

        connect(_ui->sliderSeek, &QSlider::valueChanged, this, [this](int value) {
            qint64 duration = _player->duration();
            qint64 position = (float)(value) / 100 * duration;

            _player->setPosition(position);
        });
    }

    Player::~Player() {
        delete _ui;
    }

    void Player::play(bool state) {
        state ? _player->play() : _player->pause();
    }

    void Player::playFile(const QString & file) {
        _player->setMedia(QUrl::fromLocalFile(file));
        _player->play();
    }
}
