#ifndef MINIPLAYER_H
#define MINIPLAYER_H

#include <phonon/phononnamespace.h>

#include <QWidget>

namespace Phonon {
	class AudioOutput;
	class MediaObject;
};

namespace Ui {
	class MiniPlayer;
};

class MiniPlayer : public QWidget {
	Q_OBJECT

	public:
		MiniPlayer(QWidget * parent = 0);
		~MiniPlayer();

	public slots:
		void setVisible(bool);
		void playPath(const QString & path);

	signals:
		void playButtonClicked();
		void prevButtonClicked();
		void nextButtonClicked();

		void visibilityChanged(bool);

	private slots:
		void debugState(Phonon::State);
		void tock(qint64 ms);
		void togglePlayback(bool);

	private:
		QString nowPlaying() const;

		Ui::MiniPlayer * _ui;

		Phonon::AudioOutput * _output;
		Phonon::MediaObject * _backend;

		QString _defaultText;

};

#endif
