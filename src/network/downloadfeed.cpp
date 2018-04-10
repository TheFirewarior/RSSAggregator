#include "downloadfeed.h"

QString DownloadFeed::getContentOfElement(QString atr, QDomElement el)
{
    QDomNodeList list = el.elementsByTagName(atr);
    if(list.count() == 0) {
        QString toNull;
        toNull.clear();
        return toNull;
    }
    else {
        return list.at(0).toElement().text();
    }
}

QString DownloadFeed::getAuthorNameAtom(QDomElement authorData)
{
    QDomNodeList authorStruct = authorData.elementsByTagName("author");
    if(authorStruct.count() < 0) return "";
    QDomNodeList list = authorStruct.at(0).toElement().elementsByTagName("name");
    if(list.count() < 0) {
        return "";
    }
    return list.at(0).toElement().text();
}

QString DownloadFeed::getLinkAtom(QDomElement data)
{
    QDomNodeList link = data.elementsByTagName("link");
    if(link.count() < 0) return "";
    QDomNamedNodeMap atr = link.at(0).attributes();

    return atr.namedItem("href").toAttr().value();
}

DownloadFeed::DownloadFeed(qint32 id, DatabaseManager *db, QObject *parent) : QObject(parent)
{
    m_id = id;
    m_db= db;
    m_filter = new ArticleFilter(m_db, this);
    connect(this, SIGNAL(metadataUpdate(FeedItem)), m_db, SLOT(updateMetadata(FeedItem)), Qt::DirectConnection);
    connect(this, SIGNAL(newItem(ArticleItem)), m_filter, SLOT(validateArticle(ArticleItem)), Qt::DirectConnection);
    connect(this, SIGNAL(feedUpdated(qint32)), m_db, SLOT(feedUpdated(qint32)), Qt::DirectConnection);
}

DownloadFeed::~DownloadFeed()
{
    delete m_document;
    m_db->deleteLater();
}

void DownloadFeed::setReply(QNetworkReply *reply)
{
    m_feed = reply;
    connect(m_feed, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void DownloadFeed::replyFinished()
{
    m_document = new QDomDocument();
    m_document->setContent(m_feed->readAll());
    m_feed->close();
    m_feed->deleteLater();
    parseXml();
}

void DownloadFeed::parseXml()
{
    QDomElement root = m_document->firstChildElement();

    // parse with this if it is rss
    parseRSS(root);

    // parse as atom instead
    parseAtom();

    emit feedUpdated(m_id);

}

void DownloadFeed::parseChannel(QDomElement el)
{
    // metadata update
    FeedItem item;
    item.title = getContentOfElement("title", el);
    item.link  = getContentOfElement("link", el);
    item.description = getContentOfElement("description", el);
    item.language = getContentOfElement("language", el);
    item.copyright = getContentOfElement("copyright", el);
    item.managingEditor = getContentOfElement("managingEditor", el);
    item.webMaster = getContentOfElement("webMaster", el);
    item.pubDate = getContentOfElement("pubDate", el);
    item.lastBuildDate = getContentOfElement("lastBuildDate", el);
    item.category = getContentOfElement("category", el);
    item.generator = getContentOfElement("generator", el);
    item.docs = getContentOfElement("docs", el);
    item.cloud = getContentOfElement("cloud", el);
    item.ttl = getContentOfElement("ttl", el);
    item.image = getContentOfElement("image", el);
    item.rating = getContentOfElement("rating", el);
    item.textInput = getContentOfElement("textInput", el);
    item.skipHours = getContentOfElement("skipHours", el);
    item.skipDays = getContentOfElement("skipDays", el);
    emit metadataUpdate(item);

    // article update
    QDomNodeList articles = el.elementsByTagName("item");
    m_db->getDatabaseInstance().transaction();
    qDebug() << "Recieved " << articles.count() << " items";
    for(int j = 0; j < articles.count(); ++j) {
        QDomNode itemNode = articles.at(j);
        parseArticle(itemNode);
    }
    m_db->getDatabaseInstance().commit();
}

void DownloadFeed::parseFeed(QDomElement el)
{
    qDebug() << "Parsing atom feed";
    // metadata update
    FeedItem item;
    item.title = getContentOfElement("title", el);
    item.link  = getContentOfElement("link", el);
    item.description = getContentOfElement("description", el);
    item.language = getContentOfElement("language", el);
    item.copyright = getContentOfElement("copyright", el);
    item.managingEditor = getContentOfElement("managingEditor", el);
    item.webMaster = getContentOfElement("webMaster", el);
    item.pubDate = getContentOfElement("pubDate", el);
    item.lastBuildDate = getContentOfElement("lastBuildDate", el);
    item.category = getContentOfElement("category", el);
    item.generator = getContentOfElement("generator", el);
    item.docs = getContentOfElement("docs", el);
    item.cloud = getContentOfElement("cloud", el);
    item.ttl = getContentOfElement("ttl", el);
    item.image = getContentOfElement("image", el);
    item.rating = getContentOfElement("rating", el);
    item.textInput = getContentOfElement("textInput", el);
    item.skipHours = getContentOfElement("skipHours", el);
    item.skipDays = getContentOfElement("skipDays", el);
    emit metadataUpdate(item);

    // article update
    QDomNodeList articles = el.elementsByTagName("entry");
    m_db->getDatabaseInstance().transaction();
    qDebug() << "Recieved " << articles.count() << " entries";
    for(int j = 0; j < articles.count(); ++j) {
        QDomNode itemNode = articles.at(j);
        parseArticleAtom(itemNode);
    }
    m_db->getDatabaseInstance().commit();
}

void DownloadFeed::parseArticle(QDomNode itemNode)
{
    if(itemNode.isElement()) {
        ArticleItem item;
        QDomElement itemContent = itemNode.toElement();
        item.id = m_db->getNextArticleID();
        item.feed_id = m_id;
        item.title = getContentOfElement("title", itemContent);
        item.link = getContentOfElement("link", itemContent);
        item.description = getContentOfElement("description", itemContent);
        item.author = getContentOfElement("author", itemContent);
        item.category = getContentOfElement("category", itemContent);
        item.comments = getContentOfElement("comments", itemContent);
        item.enclosure = getContentOfElement("enclosure", itemContent);
        item.guid = getContentOfElement("guid", itemContent);
        item.pubDate = getContentOfElement("pubDate", itemContent);
        item.source = getContentOfElement("source", itemContent);
        emit newItem(item);

        QDomNodeList childNodes = itemNode.childNodes();
        for(int i = 0; i < childNodes.size(); ++i) {
            // get top level media content
            if(childNodes.at(i).toElement().tagName() == "media:content") {
                QDomNode mediaNode = childNodes.at(i);
                MediaGroup group;
                group.articleID = item.id;
                qint32 uniqueGroup = m_db->addMediaGroupWithReturn(group);
                parseMedia(uniqueGroup, mediaNode);
            }
            // get groups
            else if (childNodes.at(i).toElement().tagName() == "media:group"){
                QDomNode mediaNode = childNodes.at(i);
                MediaGroup group;
                group.articleID = item.id;
                qint32 uniqueGroup = m_db->addMediaGroupWithReturn(group);
                QDomNodeList media = mediaNode.toElement().elementsByTagName("media:content");
                for(int j = 0; j < media.count(); ++j) {
                    parseMedia(uniqueGroup, media.at(j));
                }
            }
        }
    }
}

void DownloadFeed::parseArticleAtom(QDomNode itemNode)
{
    if(itemNode.isElement()) {
        ArticleItem item;
        QDomElement itemContent = itemNode.toElement();
        item.id = m_db->getNextArticleID();
        item.feed_id = m_id;
        item.title = getContentOfElement("title", itemContent);
        item.link = getContentOfElement("link", itemContent);
        item.description = getContentOfElement("description", itemContent);
        item.author = getAuthorNameAtom(itemContent);
        item.category = getContentOfElement("category", itemContent);
        item.comments = getContentOfElement("comments", itemContent);
        item.enclosure = getContentOfElement("enclosure", itemContent);
        item.guid = getContentOfElement("guid", itemContent);
        item.pubDate = getContentOfElement("pubDate", itemContent);
        item.source = getContentOfElement("source", itemContent);
        emit newItem(item);

        QDomNodeList childNodes = itemNode.childNodes();
        for(int i = 0; i < childNodes.size(); ++i) {
            // get top level media content
            if(childNodes.at(i).toElement().tagName() == "media:content") {
                QDomNode mediaNode = childNodes.at(i);
                MediaGroup group;
                group.articleID = item.id;
                qint32 uniqueGroup = m_db->addMediaGroupWithReturn(group);
                parseMedia(uniqueGroup, mediaNode);
            }
            // get groups
            else if (childNodes.at(i).toElement().tagName() == "media:group"){
                QDomNode mediaNode = childNodes.at(i);
                MediaGroup group;
                group.articleID = item.id;
                qint32 uniqueGroup = m_db->addMediaGroupWithReturn(group);
                QDomNodeList media = mediaNode.toElement().elementsByTagName("media:content");
                for(int j = 0; j < media.count(); ++j) {
                    parseMedia(uniqueGroup, media.at(j));
                }
            }
        }
    }
}

void DownloadFeed::parseMedia(qint32 mediaGroup, QDomNode mediaNode)
{
    QDomNamedNodeMap contentAtr = mediaNode.attributes();
    MediaItem item;
    item.medium = MediaTypes::UNKNOWN;
    for(int i = 0; i < contentAtr.size(); ++i) {
        QDomAttr atr = contentAtr.item(i).toAttr();
        qDebug() <<atr.name() << " " << atr.value();
        if(atr.name() == "url")
            item.url = atr.value();
        else if(atr.name() == "medium")
            item.medium = MediaItem::mediumFromString(atr.value());
        else if(atr.name() == "height")
            item.height = atr.value().toInt();
        else if(atr.name() == "width")
            item.width = atr.value().toInt();
        else if(atr.name() == "type")
            item.type = atr.value();
    }
    item.groupID = mediaGroup;
    m_db->addMedia(item);
}

void DownloadFeed::parseMediaGroup(QDomNode mediaGroupNode)
{

}

void DownloadFeed::parseRSS(QDomElement feed)
{
    QDomNodeList channels = feed.elementsByTagName("channel");
    // goes through all the channels
    for(int i = 0; i < channels.count(); ++i) {
        QDomNode channel = channels.at(i);
        if(channel.isElement()) {
            QDomElement el = channel.toElement();
            parseChannel(el);
        }
    }
}

void DownloadFeed::parseAtom()
{
    QDomNodeList feeds = m_document->elementsByTagName("feed");
    // goes through all the feeds
    for(int i = 0; i < feeds.count(); ++i) {
        qDebug() << "Parsing feeds";
        QDomNode feedContent = feeds.at(i);
        if(feedContent.isElement()) {
            QDomElement el = feedContent.toElement();
            parseFeed(el);
        }
    }
}
