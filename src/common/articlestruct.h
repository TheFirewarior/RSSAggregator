#ifndef ARTICLESTRUCT_H
#define ARTICLESTRUCT_H

#include <QObject>

struct ArticleItem {
    qint32 id;
    qint32 feed_id;
    QString title;
    QString link;
    QString description;
    QString author;
    QString category;
    QString comments;
    QString enclosure;
    QString guid;
    QString pubDate;
    QString source;
};

#endif // ARTICLESTRUCT_H
