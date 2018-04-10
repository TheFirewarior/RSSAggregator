#ifndef MEDIASTRUCT_H
#define MEDIASTRUCT_H

#include <QtCore>

enum MediaTypes {
    VIDEO,
    PICTURE,
    AUDIO,
    DOCUMENT,
    EXECUTABLE,
    UNKNOWN

};

struct MediaItem {
    qint32 id;
    qint32 groupID;
    MediaTypes medium;
    QString url;
    qint32 height;
    qint32 width;
    QString type;

    static MediaTypes mediumFromString(QString name) {
            if(name == "image")
                return MediaTypes::PICTURE;
            else if(name == "audio")
                return MediaTypes::AUDIO;
            else if(name == "video")
                return MediaTypes::VIDEO;
            else if(name == "document")
                return MediaTypes::DOCUMENT;
            else if(name == "executable")
                return MediaTypes::EXECUTABLE;
            else return MediaTypes::UNKNOWN;
    }
};

#endif // MEDIASTRUCT_H
