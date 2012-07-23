
#include <phonon/AudioOutput>
#include <phonon/MediaObject>

#include <QDebug>

#include "MiniPlayer.h"

#include "ui_MiniPlayer.h"

MiniPlayer::MiniPlayer(QWidget * parent) {
	_defaultText = tr("Nothing is playing!");

	_ui = new Ui::MiniPlayer();
	_ui->setupUi(this);

	_output = new Phonon::AudioOutput(Phonon::MusicCategory);
	_backend = new Phonon::MediaObject(this);

	Phonon::createPath(_backend, _output);

	_ui->playerVolume->setAudioOutput(_output);
	_ui->playerSeekBar->setMediaObject(_backend);

	_ui->playerTogglePlayback->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	_ui->playerTogglePlayback->setText("");

	_ui->playerPlayNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
	_ui->playerPlayNext->setText("");

	_ui->playerPlayPrevious->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
	_ui->playerPlayPrevious->setText("");

	_ui->playerClose->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
	_ui->playerClose->setText("");

	connect(_backend, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
		SLOT(debugState(Phonon::State)));

	connect(_backend, SIGNAL(tick(qint64)),
		SLOT(tock(qint64)));

	connect(_ui->playerTogglePlayback, SIGNAL(clicked(bool)),
		SLOT(togglePlayback(bool)));

	connect(_ui->playerPlayNext, SIGNAL(clicked()),
		SIGNAL(nextButtonClicked()));

	connect(_ui->playerPlayPrevious, SIGNAL(clicked()),
		SIGNAL(prevButtonClicked()));
}

MiniPlayer::~MiniPlayer() {
	delete _ui;
}





/*
 * PUBLIC SLOTS
 */

void MiniPlayer::setVisible(bool state) {
	if (isVisible() == state) {
		return;
	}

	emit visibilityChanged(state);

	if (!state) {
		_backend->stop();
	}

	QWidget::setVisible(state);
}

void MiniPlayer::playPath(const QString & path) {
	_backend->setCurrentSource(path);
	togglePlayback(true);
}





/*
 * PRIVATE SLOTS
 */

void MiniPlayer::debugState(Phonon::State state) {

	switch (state) {
		case Phonon::StoppedState:
			_ui->playerDisplay->setText(_defaultText);

		case Phonon::PausedState:
			_ui->playerDisplay->setDisabled(true);
			_ui->playerTogglePlayback->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
			break;

		case Phonon::PlayingState: {

			_ui->playerDisplay->setText(nowPlaying());
			_ui->playerDisplay->setEnabled(true);

			_ui->playerTogglePlayback->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		}
			break;

		default:
			// pass
			;
	}
}

void MiniPlayer::tock(qint64 ms) {
	QString label = nowPlaying();
}

void MiniPlayer::togglePlayback(bool state) {
	bool isPlaying = (_backend->state() == Phonon::PlayingState);

	if (state == isPlaying) {
		return;
	}

	if (state) {
		if (_backend->currentSource().fileName().isEmpty()) {
			emit playButtonClicked();
		} else {
		_backend->play();
		}
	} else {
		_backend->pause();
	}

	_ui->playerTogglePlayback->setChecked(state);
}





/*
 * PRIVATE METHODS
 */

QString MiniPlayer::nowPlaying() const {
	QString artist = _backend->metaData("ARTIST").first();
	QString title = _backend->metaData("TITLE").first();

	if (title.isEmpty()) {
		title = "<i>n/a</i>";
	} else {
		title.prepend("<b>").append("</b>") ;
	}

	QString message = title;

	if (!artist.isEmpty()) {
		artist.prepend("<b>").append("</b>");

		message += " ";
		message += tr("by");
		message += QString(" ").append(artist);
	}

	return message;
}
