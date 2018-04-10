#ifndef DOWNLOADFEED_H
#define DOWNLOADFEED_H

#include <QObject>
#include <QNetworkReply>
#include <QtXml>
#include <QDebug>

#include "databasemanager.h"
#include "articlefilter.h"
#include "articlestruct.h"
#include "feedsstruct.h"
#include "mediagroup.h"
#include "mediastruct.h"

class DownloadFeed : public QObject
{
    Q_OBJECT
    qint32 m_id;
    QNetworkReply *m_feed;
    QDomDocument *m_document;
    DatabaseManager *m_db;
    ArticleFilter *m_filter;
    QString getContentOfElement(QString atr, QDomElement el);
    QString getAuthorNameAtom(QDomElement authorData);
    QString getLinkAtom(QDomElement data);
public:
    explicit DownloadFeed(qint32 id, DatabaseManager *db, QObject *parent = nullptr);
    ~DownloadFeed();
    void setReply(QNetworkReply *reply);
signals:
    void metadataUpdate(FeedItem item);
    void newItem(ArticleItem item);
    void feedUpdated(qint32 id);
public slots:
    void replyFinished();
    void parseXml();
    void parseChannel(QDomElement el);
    void parseFeed(QDomElement el);
    void parseArticle(QDomNode itemNode);
    void parseArticleAtom(QDomNode itemNode);
    void parseMedia(qint32 mediaGroup, QDomNode mediaNode);
    void parseMediaGroup(QDomNode mediaGroupNode);
    void parseRSS(QDomElement feed);
    void parseAtom();
};

#endif // DOWNLOADFEED_H
