#ifndef MEDIADISPLAY_H
#define MEDIADISPLAY_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QVector>
#include <QMap>
#include <QSqlQuery>
#include <QMapIterator>
#include <QTabWidget>
#include <QPixmap>
#include <QUrl>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QSpacerItem>

#include "databasemanager.h"
#include "downloadmedia.h"
#include "mediagroup.h"
#include "mediastruct.h"
#include "audioplayer.h"
#include "imagedisplay.h"

class MediaDisplay : public QWidget
{
    Q_OBJECT
    qint32 m_id;
    DatabaseManager *m_db;
    QVector<MediaGroup> groups;
    QMap<MediaGroup*, QVector<MediaItem*>> m_mediaItems;

    ImageDisplay *imageDisplay(QList<MediaItem*> items);
    QVideoWidget *videoDisplay(MediaItem *item);
    AudioPlayer *audioDisplay(QList<MediaItem*> items);
    MediaTypes checkType(MediaItem *item);

public:
    explicit MediaDisplay(DatabaseManager *db, QWidget *parent = nullptr);

signals:

public slots:
    void setArticleID(qint32 articleID);
    void getNewData();
    void getNewMediaData(MediaGroup *group);
    void displayData();
};

#endif // MEDIADISPLAY_H
