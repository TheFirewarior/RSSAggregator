#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>

#include "articlestruct.h"
#include "feedsstruct.h"
#include "mediastruct.h"
#include "mediagroup.h"

#define CONNECTION_NAME "feeds_database"
#define DATABASE_NAME   "feeds_database.db"

class DatabaseManager : public QObject
{
    Q_OBJECT
    QSqlDatabase m_db;
    QString m_connection;
public:
    explicit DatabaseManager(QString connectionName, QObject *parent = nullptr);
    ~DatabaseManager();
    QSqlDatabase getDatabaseInstance();
    QSqlQuery getFeedData(qint32 feedID);
    QSqlQuery getFolderData(qint32 folderID);
    QSqlQuery getFeedsViewData();
    QSqlQuery getFolderViewData();
    QSqlQuery getFeedsToUpdate();
    QSqlQuery getListData(qint32 id, QStringList columns);
    QSqlQuery getChildFolders(qint32 folder_id);
    QSqlQuery getChildFeeds(qint32 folder_id);
    QSqlQuery getFolderFilter(qint32 folder_id);
    QSqlQuery getFeedFilter(qint32 feed_id);
    QSqlQuery getOwnFolderFilter(qint32 folder_id);
    QSqlQuery getOwnFeedFilter(qint32 feed_id);
    QSqlQuery getMediaGroupData(qint32 article_id);
    QSqlQuery getMediaData(qint32 group_id);
    qint32 getFolderParentID(qint32 child);
    qint32 getFeedParentID(qint32 child);
    void insertTestData();
    bool checkArticle(ArticleItem item);
    QSqlQuery getArticle(qint32 id);
    qint32 getUpdatePeriod(qint32 id);
    qint32 addMediaGroupWithReturn(MediaGroup group);
    qint32 getNextArticleID();
    QString getSetting(QString name);
signals:
    void dataChanged();
    void feedAdded();
    void articleChanged(qint32 article);
public slots:
    bool setSetting(QString name, QString value);
    bool addFolder(QString title, qint32 parent_id, qint32 row_num, qint32 update_period);
    bool addFolder(QString title, qint32 parent_id);
    bool addFeed(QString title, QString url, qint32 folder_id, qint32 row_num, qint32 update_period);
    bool addFeed(QString title, QString url, qint32 folder_id);
    bool addArticle(ArticleItem item);
    bool addConfiguration(qint32 update_period);
    bool addMedia(MediaItem item);
    bool addMediaGroup(MediaGroup group);
    bool updateMetadata(FeedItem item);
    bool feedUpdated(qint32 feed_id);
    bool removeFeed(qint32 feed_id);
    bool removeMediaGroup(qint32 articleID);
    bool removeMedia(qint32 mediaGroupID);
    bool removeFolder(qint32 folder_id);
    bool removeChildren(qint32 folder_id);
    bool updateFeed(qint32 id, QString title, QString url, qint32 updatePeriod);
    bool updateFolder(qint32 id, QString title, qint32 updatePeriod);
    bool setFilterFeed(qint32 id, QString globalFilter, QString titleFilter, QString descriptionFilter, QString categoryFilter);
    bool setFilterFolder(qint32 id, QString globalFilter, QString titleFilter, QString descriptionFilter, QString categoryFilter);
    bool setArticleToRead(qint32 id);
    bool alterFavouriteState(qint32 articleID);
private:

    void initDatabase(QString connectionName);
    bool createTable(QString table);
    QString createFeedsTableQuery();
    QString createArticleTableQuery();
    QString createFolderTableQuery();
    QString createMediaTableQuery();
    QString createMediaGroupTableQuery();
    QString createConfigurationTableQuery();
    QString createSettingsTableQuery();


};


#endif // DATABASEMANAGER_H
