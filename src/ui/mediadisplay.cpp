#include "mediadisplay.h"

ImageDisplay *MediaDisplay::imageDisplay(QList<MediaItem *> items)
{
    ImageDisplay *display = new ImageDisplay;
    foreach(MediaItem *item, items) {
        display->addImage(item->url);
    }
    return display;
}

QVideoWidget *MediaDisplay::videoDisplay(MediaItem *item)
{
    QMediaPlayer *player = new QMediaPlayer;
    //QMediaPlaylist *playlist = new QMediaPlaylist(player);
    player->setMedia(QUrl(item->url));
    //playlist->addMedia(QUrl(item->url));
    QVideoWidget *widget = new QVideoWidget;
    widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //widget->resize(item->width, item->height);
    player->setVideoOutput(widget);
    qDebug() << "here";
    player->play();
    return new QVideoWidget();
}

AudioPlayer *MediaDisplay::audioDisplay(QList<MediaItem *> items)
{
    AudioPlayer *player = new AudioPlayer(this);
    foreach(MediaItem *item, items) {
        player->addAudio(item->url);
    }
    return player;
}

MediaTypes MediaDisplay::checkType(MediaItem *item)
{
    qDebug() << "Type is: " << item->type;
    if (item->type == "audio/mpeg")
        return MediaTypes::AUDIO;
    else if(item->type == "video/mp4")
        return MediaTypes::VIDEO;
    return MediaTypes::UNKNOWN;
}

MediaDisplay::MediaDisplay(DatabaseManager *db, QWidget *parent) : QWidget(parent)
{
    m_db = db;
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setLayout(new QVBoxLayout);
}

void MediaDisplay::setArticleID(qint32 articleID)
{
    m_id = articleID;
    getNewData();
}

void MediaDisplay::getNewData()
{
    m_mediaItems.clear();
    QSqlQuery getGroups = m_db->getMediaGroupData(m_id);
    while(getGroups.next()) {
        MediaGroup *group = new MediaGroup;
        group->articleID = m_id;
        group->id = getGroups.value("id").toInt();
        m_mediaItems.insert(group, QVector<MediaItem*>());
        getNewMediaData(group);

    }
    displayData();
}

void MediaDisplay::getNewMediaData(MediaGroup *group)
{
    QSqlQuery getMedia = m_db->getMediaData(group->id);
    while(getMedia.next()) {
        MediaItem *item = new MediaItem;
        item->groupID = group->id;
        item->id = getMedia.value("id").toInt();
        item->medium = static_cast<MediaTypes>(getMedia.value("medium").toInt());
        item->height = getMedia.value("height").toInt();
        item->width = getMedia.value("width").toInt();
        item->url = getMedia.value("url").toString();
        item->type = getMedia.value("type").toString();
        m_mediaItems[group].append(item);
    }
}

void MediaDisplay::displayData()
{
    QLayoutItem *item;
    while((item = layout()->takeAt(0))) {
        delete item->widget();
        delete item;
    }
    qDebug() << "Starting display of media";
    QMapIterator<MediaGroup*, QVector<MediaItem*>> iterateGroups(m_mediaItems);
    while(iterateGroups.hasNext()) {
        iterateGroups.next();
        MediaGroup *group = iterateGroups.key();
        QVector<MediaItem*> items = iterateGroups.value();
        QTabWidget *groupTabs = new QTabWidget;
        groupTabs->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        qint32 minHeight = 0;
        QList<MediaItem*> audio;
        QList<MediaItem*> video;
        QList<MediaItem*> pictures;
        foreach (MediaItem *item, items) {
            if(item->height > minHeight) minHeight = item->height;
            if(item->medium == MediaTypes::PICTURE) {
                pictures.append(item);
            }
            else if (item->medium == MediaTypes::VIDEO || checkType(item) == MediaTypes::VIDEO) {
                groupTabs->addTab(videoDisplay(item), "Tab " + QString::number(groupTabs->count()+1));
            }
            else if (item->medium == MediaTypes::AUDIO || checkType(item) == MediaTypes::AUDIO) {
                audio.append(item);
            }
        }
        //groupTabs->setMinimumHeight(minHeight);
        if(audio.count() > 0)
            layout()->addWidget(audioDisplay(audio));
        if(pictures.count() > 0)
            layout()->addWidget(imageDisplay(pictures));

    }
}
