#ifndef COQUILLO_PLAYER_H
#define COQUILLO_PLAYER_H

#include <QWidget>

class QMediaPlayer;

namespace Ui {
    class Player;
}

namespace Coquillo {
    class Player : public QWidget {
        Q_OBJECT

        public:
            Player(QWidget * parent = nullptr);
            ~Player();

        public slots:
            void play(bool state = true);
            void playFile(const QString & file);

        private:
            Ui::Player * _ui;
            QMediaPlayer * _player;
    };
}

#endif
