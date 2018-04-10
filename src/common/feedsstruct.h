#ifndef FEEDSSTRUCT_H
#define FEEDSSTRUCT_H

#include <QtCore>

struct FeedItem {
    qint32 id;
    QString title;
    QString link;
    QString description;
    QString language;
    QString copyright;
    QString managingEditor;
    QString webMaster;
    QString pubDate;
    QString lastBuildDate;
    QString category;
    QString generator;
    QString docs;
    QString cloud;
    QString ttl;
    QString image;
    QString rating;
    QString textInput;
    QString skipHours;
    QString skipDays;
};

#endif // FEEDSSTRUCT_H
