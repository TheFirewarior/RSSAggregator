#include "articlelistmodel.h"

QList<qint32> ArticleListModel::queryToIDs(QSqlQuery query)
{
    QList<qint32> ids;
    while(query.next()) {
        ids.append(query.value("id").toInt());
    }
    return ids;
}

QList<qint32> ArticleListModel::getChildFeedsID(qint32 id)
{
    QList<qint32> feeds;
    // get all feeds
    feeds.append(queryToIDs(m_db->getChildFeeds(id)));

    QList<qint32> folders = queryToIDs(m_db->getChildFolders(id));
    foreach(qint32 folder, folders) {
        feeds.append(getChildFeedsID(folder));
    }
    return feeds;
}

void ArticleListModel::changeHeaderNames()
{
    for(int i = 0; i < columnCount(); ++i) {
        if(i == fieldIndex("title")) {
            setHeaderData(i, Qt::Horizontal, tr("Title"), Qt::DisplayRole);
        }
        else if(i == fieldIndex("author")) {
            setHeaderData(i, Qt::Horizontal, tr("Author"), Qt::DisplayRole);
        }
        else if(i == fieldIndex("category")) {
            setHeaderData(i, Qt::Horizontal, tr("Category"), Qt::DisplayRole);
        }
        else if(i == fieldIndex("pubDate")) {
            setHeaderData(i, Qt::Horizontal, tr("Date"), Qt::DisplayRole);
        }
        else if(i == fieldIndex("favourite")) {
            setHeaderData(i, Qt::Horizontal, tr(" "), Qt::DisplayRole);
        }
    }
}


ArticleListModel::ArticleListModel(DatabaseManager *db, QObject *parent)
    : QSqlTableModel(parent, db->getDatabaseInstance())
{
    m_db = db;
    setTable("Article");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();
    changeHeaderNames();
}

QVariant ArticleListModel::data(const QModelIndex &idx, int role) const
{
    if(idx.column() == fieldIndex("pubDate") && role == Qt::DisplayRole) {
        QDateTime date;
        date = QDateTime::fromString(QSqlTableModel::data(idx, role).toString(), Qt::RFC2822Date);
        //qDebug() << date.toLocalTime().toString();
        return date.toString("ddd, d.M.yyyy");
    }
    if(role == Qt::BackgroundRole) {
        if(data(index(idx.row(), fieldIndex("read")), Qt::DisplayRole).toInt() == 1) {
            return m_readColor;

        }
        else
            return QSqlTableModel::data(idx, role);
    }
    return QSqlTableModel::data(idx, role);
}

void ArticleListModel::setNewID(QString id)
{
    m_id = id;
    qDebug() << "Got new id: " << m_id;
    QList<qint32> feeds;
    qint32 idNum = m_id.right(m_id.size()-2).toInt();
    qDebug() << idNum;
    // id is a feed id
    if(m_id.left(2) == "Fe") {
        feeds.append(idNum);
    }
    // get all feeds that are children of a given id
    else if(m_id.left(2) == "Fo") {
        feeds = getChildFeedsID(idNum);
    }
    //qDebug() << feeds;
    //QSqlQuery query(m_db->getDatabaseInstance());
    //QString queryVal = "SELECT title, pubDate, author FROM Article WHERE ";
    QString queryVal = "";
    if(feeds.count() != 0) {
        for(int i = 0; i < feeds.count(); ++i) {
            if(i != 0) queryVal.append(" OR ");
            queryVal.append("feed_id = " + QString::number(feeds[i]));
        }
    }
    else {
        queryVal = "1=0";
    }
    //query.prepare(queryVal);
    //setQuery(query);
    //qDebug() << queryVal;
    setFilter(queryVal);
    select();
}

void ArticleListModel::setReadColor(QString color)
{
    m_readColor = QColor(color);
}

void ArticleListModel::handleArticleChanged(qint32 articleID)
{
    qDebug() << articleID << " changed";
    int idCol = fieldIndex("id");
    for (int i = 0; i < rowCount(); ++i) {
        if(index(i, idCol).data().toInt() == articleID) {
            dataChanged(index(i, 0), index(i, columnCount()));
            return;
        }
    }
}



