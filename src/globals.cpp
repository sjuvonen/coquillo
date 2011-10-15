
#include "globals.h"

QString toQString(const char * str, int encoding) {
	switch (encoding) {
		case Coquillo::ISO_8859_15:
			return QString::fromLatin1(str);

		case Coquillo::UTF_8:
		default:
			return QString::fromUtf8(str);
	}
}

int Coquillo::encoding = Coquillo::UTF_8;
int Coquillo::numberPadWidth = 0;
int Coquillo::fileExtensionCase = 0;

bool Coquillo::removeDiscFromAlbumName = false;
bool Coquillo::intelligentCapitalization = false;
bool Coquillo::safeFileNames = false;
bool Coquillo::trimWhiteSpace = false;

bool Coquillo::dimSubdirectoriesBackground = false;

bool Coquillo::Flac::id3v2 = false;
bool Coquillo::Mpeg::id3v1 = false;
bool Coquillo::OggVorbis::stripLegacyCovers = false;

QStringList Coquillo::ignoreWords = QStringList() << "in" << "the" << "at" << "of";
