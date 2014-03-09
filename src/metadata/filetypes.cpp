
#include <taglib/taglib.h>
#include <taglib/fileref.h>

#include <taglib/attachedpictureframe.h>
#include <taglib/flacfile.h>
#include <taglib/mp4tag.h>
#include <taglib/mpegfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/oggfile.h>
#include <taglib/xiphcomment.h>
#include <taglib/xingheader.h>

#include "filetypes.h"
#include "metadata.h"
#include "tagreader.h"

namespace Coquillo {
    namespace MetaData {
        void MpegReader::read() {
            TagLib::MPEG::File file(path().toUtf8().constData());
            MetaData meta(path());

            if (file.hasID3v2Tag()) {
                Id3v2Reader reader(file.ID3v2Tag());
                meta.addTag("id3v2", reader.read());
            }

            if (file.hasID3v1Tag()) {
                Id3v1Reader reader(file.ID3v1Tag());
                meta.addTag("id3v1", reader.read());
            }

            finish(meta);
        }

        void FlacReader::read() {
            TagLib::FLAC::File file(path().toUtf8().constData());
            MetaData meta(path());

            if (file.hasXiphComment()) {
                XiphReader reader(file.xiphComment());
                meta.addTag("xiph", reader.read());
            }

            if (file.hasID3v2Tag()) {
                Id3v2Reader reader(file.ID3v2Tag());
                meta.addTag("id3v2", reader.read());
            }

            if (file.hasID3v1Tag()) {
                Id3v1Reader reader(file.ID3v1Tag());
                meta.addTag("id3v1", reader.read());
            }

            finish(meta);
        }

        void OggVorbisReader::read() {
            TagLib::Ogg::Vorbis::File file(path().toUtf8().constData());
            MetaData meta(path());

            if (file.tag()) {
                XiphReader reader(file.tag());
                meta.addTag("xiph", reader.read());
            }

            finish(meta);
        }
    }
}
