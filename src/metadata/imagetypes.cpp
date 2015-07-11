
#include <QObject>
#include <QStringList>
#include <taglib/flacpicture.h>
#include "imagetypes.hpp"

#define tr(str) QObject::tr(str)

typedef TagLib::FLAC::Picture Type;

namespace Coquillo {
    namespace MetaData {
        QMap<int, QString> ImageTypes::_types = {
            { Type::Other, tr("Other") },
            { Type::FileIcon, tr("File icon (PNG. 32x32 px") },
            { Type::OtherFileIcon, tr("File icon (other format)") },
            { Type::FrontCover, tr("Front cover") },
            { Type::BackCover, tr("Back cover") },
            { Type::LeafletPage, tr("Leaflet page") },
            { Type::Media, tr("Image from album") },
            { Type::LeadArtist, tr("Lead artist") },
            { Type::Artist, tr("Artist or performer") },
            { Type::Conductor, tr("Conductor") },
            { Type::Band, tr("Band") },
            { Type::Composer, tr("Composer") },
            { Type::Lyricist, tr("Lyricist") },
            { Type::RecordingLocation, tr("Recording location") },
            { Type::DuringRecording, tr("Artist during recording") },
            { Type::DuringPerformance, tr("Artist during performance") },
            { Type::MovieScreenCapture, tr("Movie screenshot") },
            { Type::Illustration, tr("Illustration") },
            { Type::BandLogo, tr("Band logo") },
            { Type::PublisherLogo, tr("Publisher logo") },
        };

        QStringList ImageTypes::types() {
            return _types.values();
        }
    };
}
