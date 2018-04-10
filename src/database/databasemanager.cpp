#include "databasemanager.h"



DatabaseManager::DatabaseManager(QString connectionName, QObject *parent) : QObject(parent)
{
    m_connection = connectionName;
    initDatabase(connectionName);
}

DatabaseManager::~DatabaseManager()
{
    if(m_db.isOpen())
        m_db.close();
}

void DatabaseManager::initDatabase(QString connectionName)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    m_db.setDatabaseName(DATABASE_NAME);
    if (!m_db.open()) {
        qCritical() << "Couldn't open database: " << m_db.lastError().text();
    }
    else {
        qDebug() << "Connected to database!";
        //QSqlQuery enableForeignKeys(m_db);
        //enableForeignKeys.prepare("PRAGMA foreign_keys = ON;");
        //enableForeignKeys.exec();
        // create tables
        createTable(createFolderTableQuery());
        createTable(createFeedsTableQuery());
        createTable(createArticleTableQuery());
        createTable(createMediaTableQuery());
        createTable(createMediaGroupTableQuery());
        createTable(createSettingsTableQuery());
        //createTable(createConfigurationTableQuery());
    }
    qDebug() << "Finished init";
}

bool DatabaseManager::createTable(QString table)
{
    QSqlQuery query(m_db);
    query.prepare(table);
    if(query.exec())
        return true;
    else {
        qDebug() << table << query.lastError();
        return false;
    }
}


QSqlDatabase DatabaseManager::getDatabaseInstance()
{
    return m_db;
}

QSqlQuery DatabaseManager::getFeedData(qint32 feedID)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT display_title, url, update_period FROM Feed WHERE id = :id");
    query.bindValue(":id", feedID);
    if(!query.exec()) {
        qDebug() << "Error getting feed data: " << query.lastError();
    }
    return query;
}

QSqlQuery DatabaseManager::getFolderData(qint32 folderID)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT title, update_period FROM Folder WHERE id = :id");
    query.bindValue(":id", folderID);
    if(!query.exec()) {
        qDebug() << "Error getting feed data: " << query.lastError();
    }
    return query;
}

QSqlQuery DatabaseManager::getFeedsViewData()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id, folder_id, display_title, row_num FROM Feed;");
    query.exec();
    return query;
}

QSqlQuery DatabaseManager::getFolderViewData()
{
    //QSqlQuery query("SELECT id, parent_id, title, row_num FROM Folder;");
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Folder");
    query.exec();
    return query;
}

QSqlQuery DatabaseManager::getFeedsToUpdate()
{
    QSqlQuery query(m_db);
    // returns all feeds which need to be updated
    query.prepare("SELECT id, url FROM Feed WHERE (last_update IS NULL) OR ((strftime(\'%s\', last_update)+(update_period*60)) - strftime(\'%s\',datetime(\'now\')) <= 0)");
    if(!query.exec()) {
        qDebug() << "Feeds to update failed: " << query.lastError();
    }
    return query;
}

QSqlQuery DatabaseManager::getListData(qint32 id, QStringList columns)
{
    QSqlQuery query(m_db);
    QString queryValue("SELECT ");
    foreach(QString column, columns) {
        queryValue.append(column + " ");
    }
    queryValue.append("FROM Article WHERE feed_id = :id");
    query.prepare(queryValue);
    query.bindValue(":id", id);
    if(!query.exec()) {
        qDebug() << "Get list failed: " << query.lastError();
    }
    return query;
}

QSqlQuery DatabaseManager::getChildFolders(qint32 folder_id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM Folder WHERE parent_id = :folder_id");
    query.bindValue(":folder_id", folder_id);
    if(!query.exec()) {
        qDebug() << "Get child folders failed: " << query.lastError();
    }
    return query;
}

QSqlQuery DatabaseManager::getChildFeeds(qint32 folder_id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM Feed WHERE folder_id = :folder_id");
    query.bindValue(":folder_id", folder_id);
    if(!query.exec()) {
        qDebug() << "Get child feeds failed: " << query.lastError();
    }
    return query;
}

QSqlQuery DatabaseManager::getFolderFilter(qint32 folder_id)
{
    QSqlQuery query(m_db);
    QString queryString = "SELECT global_filter, title_filter, description_filter, category_filter FROM Folder WHERE id=:id";
    qint32 parentID = getFolderParentID(folder_id);
    while(parentID != 0) {
        queryString.append(" UNION ALL SELECT global_filter, title_filter, description_filter, category_filter FROM Folder WHERE id="+QString::number(parentID));
        parentID = getFolderParentID(parentID);
    }
    query.prepare(queryString);
    query.bindValue(":id", folder_id);
    if(!query.exec()) {
        qDebug() << "Get folder filters failed: " << query.lastError();
    }
    return query;
}

QSqlQuery DatabaseManager::getFeedFilter(qint32 feed_id)
{
    QSqlQuery query(m_db);
    QString queryString = "SELECT global_filter, title_filter, description_filter, category_filter FROM Feed WHERE id=:id";
    qint32 parentID = getFeedParentID(feed_id);
    while(parentID != 0) {
        queryString.append(" UNION ALL SELECT global_filter, title_filter, description_filter, category_filter FROM Folder WHERE id="+QString::number(parentID));
        parentID = getFolderParentID(parentID);
    }
    query.prepare(queryString);
    qDebug() << queryString;
    query.bindValue(":id", feed_id);
    if(!query.exec()) {
        qDebug() << "Get folder filters failed: " << query.lastError();
    }
    return query;
}

QSqlQuery DatabaseManager::getOwnFolderFilter(qint32 folder_id)
{
    QSqlQuery query(m_db);
    QString queryString = "SELECT global_filter, title_filter, description_filter, category_filter FROM Folder WHERE id=:id";
    query.prepare(queryString);
    query.bindValue(":id", folder_id);
    if(!query.exec()) {
        qDebug() << "Get folder filters failed: " << query.lastError();
    }
    return query;
}

QSqlQuery DatabaseManager::getOwnFeedFilter(qint32 feed_id)
{
    QSqlQuery query(m_db);
    QString queryString = "SELECT global_filter, title_filter, description_filter, category_filter FROM Feed WHERE id=:id";
    query.prepare(queryString);
    query.bindValue(":id", feed_id);
    if(!query.exec()) {
        qDebug() << "Get folder filters failed: " << query.lastError();
    }
    return query;
}

QSqlQuery DatabaseManager::getMediaGroupData(qint32 article_id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM MediaGroup WHERE article_id = :article_id");
    query.bindValue(":article_id", article_id);
    if(!query.exec()) {
        qDebug() << "Failed to get MediaGroup data: " << query.lastError();
    }
    return query;
}

QSqlQuery DatabaseManager::getMediaData(qint32 group_id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Media WHERE group_id = :group_id");
    query.bindValue(":group_id", group_id);
    if(!query.exec()) {
        qDebug() << "Failed to get Media data: " << query.lastError();
    }
    return query;
}

qint32 DatabaseManager::getFolderParentID(qint32 child)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT parent_id FROM Folder WHERE id=:id");
    query.bindValue(":id", child);
    if(!query.exec()) {
        qDebug() << "Get parent id for folder failed: " << query.lastError();
        return 0;
    }
    query.next();
    return query.value("parent_id").toInt();
}

qint32 DatabaseManager::getFeedParentID(qint32 child)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT folder_id FROM Feed WHERE id=:id");
    query.bindValue(":id", child);
    if(!query.exec()) {
        qDebug() << "Get parent id for feed failed: " << query.lastError();
        return 0;
    }
    query.next();
    return query.value("folder_id").toInt();
}

bool DatabaseManager::addFolder(QString title, qint32 parent_id, qint32 row_num, qint32 update_period)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Folder (title, parent_id, row_num, update_period) VALUES(:title, :parent_id, :row_num, :update_period)");
    query.bindValue(":title", title);
    query.bindValue(":parent_id", parent_id);
    query.bindValue(":row_num", row_num);
    query.bindValue(":update_period", update_period);
    if(query.exec()) {
        emit dataChanged();
        return true;
    }
    else  {
        qDebug() << "Error adding folder: " << query.lastError();
        return false;
    }
}

bool DatabaseManager::addFolder(QString title, qint32 parent_id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(row_num) AS max_row FROM Feed WHERE folder_id = :folder_id");
    query.bindValue(":folder_id", parent_id);
    QSqlQuery query2(m_db);
    query2.prepare("SELECT COUNT(row_num) AS max_row FROM Folder WHERE parent_id = :folder_id");
    query2.bindValue(":folder_id", parent_id);
    if(!query.exec() || !query2.exec()) {
        qDebug() << "Select max row failed: " << query.lastError() << query2.lastError();
        return false;
    }
    if(!query.next() || !query2.next()) {
        return false;
    }
    qint32 max = query.value("max_row").toInt() + query2.value("max_row").toInt();
    return addFolder(title, parent_id, max, getUpdatePeriod(parent_id));
}

bool DatabaseManager::addFeed(QString title, QString url, qint32 folder_id, qint32 row_num, qint32 update_period)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Feed (display_title, url, folder_id, row_num, update_period) VALUES (:title, :url, :folder_id, :row_num, :update_period)");
    query.bindValue(":title", title);
    query.bindValue(":url", url);
    query.bindValue(":folder_id", folder_id);
    query.bindValue(":row_num", row_num);
    query.bindValue(":update_period", update_period);
    if(query.exec()) {
        emit dataChanged();
        emit feedAdded();
        return true;
    }
    else {
        qDebug() << "Error adding feed: " << query.lastError();
        return false;
    }
}

bool DatabaseManager::addFeed(QString title, QString url, qint32 folder_id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(row_num) AS max_row FROM Feed WHERE folder_id = :folder_id");
    query.bindValue(":folder_id", folder_id);
    QSqlQuery query2(m_db);
    query2.prepare("SELECT COUNT(row_num) AS max_row FROM Folder WHERE parent_id = :folder_id");
    query2.bindValue(":folder_id", folder_id);
    if(!query.exec() || !query2.exec()) {
        qDebug() << "Select max row failed: " << query.lastError() << query2.lastError();
        return false;
    }
    if(!query.next() || !query2.next()) {
        return false;
    }
    qint32 max = query.value("max_row").toInt() + query2.value("max_row").toInt();
    return addFeed(title, url, folder_id, max, getUpdatePeriod(folder_id));

}

bool DatabaseManager::addArticle(ArticleItem item)
{
    qDebug() << "Begining add new article";
    // add article only if it's new
    if(!checkArticle(item)) {
        QSqlQuery query(m_db);
        query.prepare("INSERT INTO Article (id, title, link, description, author, category, comments, enclosure, guid, pubDate, source, feed_id) "
                      "VALUES (:id, :title, :link, :description, :author, :category, :comments, :enclosure, :guid, :pubDate, :source, :feed_id)"
        );
        query.bindValue(":id", item.id);
        query.bindValue(":title", item.title);
        query.bindValue(":link", item.link);
        query.bindValue(":description", item.description);
        query.bindValue(":author", item.author);
        query.bindValue(":category", item.category);
        query.bindValue(":comments", item.comments);
        query.bindValue(":enclosure", item.enclosure);
        query.bindValue(":guid", item.guid);
        query.bindValue(":pubDate", item.pubDate);
        query.bindValue(":source", item.source);
        query.bindValue(":feed_id", item.feed_id);
        qDebug() << "query is ready to be executed!";
        if(query.exec()) {
            return true;
        }
        else {
            qDebug() << "Failed to add article: " << query.lastError();
            return false;
        }
    }
    return false;
}

bool DatabaseManager::addConfiguration(qint32 update_period)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Configuration (update_period) VALUES (:update_period)");
    query.bindValue(":update_period", update_period);
    if(query.exec()) {
        emit dataChanged();
        return true;
    }
    else {
        qDebug() << "Error adding configuration: " << query.lastError();
        return false;
    }
}

bool DatabaseManager::addMedia(MediaItem item)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Media (group_id, medium, url, height, width, type) VALUES (:group_id, :medium, :url, :height, :width, :type)");
    query.bindValue(":medium", item.medium);
    query.bindValue(":group_id", item.groupID);
    query.bindValue(":url", item.url);
    query.bindValue(":height", item.height);
    query.bindValue(":width", item.width);
    query.bindValue(":type", item.type);
    if(query.exec()) {
        return true;
    }
    else {
        qDebug() << "Error adding media: " << query.lastError();
        return false;
    }
}

bool DatabaseManager::addMediaGroup(MediaGroup group)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO MediaGroup (article_id) VALUES (:article_id)");
    query.bindValue(":article_id", group.articleID);
    if(query.exec()) {
        return true;
    }
    else {
        qDebug() << "Error adding media group: " << query.lastError();
        return false;
    }
}

bool DatabaseManager::updateMetadata(FeedItem item)
{
    QSqlQuery query(m_db);
    QString text (
                "UPDATE Feed SET "
                    "title = :title, "
                    "link = :link, "
                    "description = :description, "
                    "language = :language, "
                    "copyright = :copyright, "
                    "managingEditor = :managingEditor, "
                    "webMaster = :webMaster, "
                    "pubDate = :pubDate, "
                    "lastBuildDate = :lastBuildDate, "
                    "category = :category, "
                    "generator = :generator, "
                    "docs = :docs, "
                    "cloud = :cloud, "
                    "ttl = :ttl, "
                    "image = :image, "
                    "rating = :rating, "
                    "textInput = :textInput, "
                    "skipHours = :skipHours, "
                    "skipDays = :skipDays "
                "WHERE id = :id"
    );
    query.prepare(text);
    query.bindValue(":title", item.title);
    query.bindValue(":link", item.link);
    query.bindValue(":description", item.description);
    query.bindValue(":language", item.language);
    query.bindValue(":copyright", item.copyright);
    query.bindValue(":managingEditor", item.managingEditor);
    query.bindValue(":webMaster", item.webMaster);
    query.bindValue(":pubDate", item.pubDate);
    query.bindValue(":lastBuildDate", item.lastBuildDate);
    query.bindValue(":category", item.category);
    query.bindValue(":generator",  item.generator);
    query.bindValue(":docs", item.docs);
    query.bindValue(":cloud", item.cloud);
    query.bindValue(":ttl", item.ttl);
    query.bindValue(":image", item.image);
    query.bindValue(":rating", item.rating);
    query.bindValue(":textInput", item.textInput);
    query.bindValue(":skipHours", item.skipHours);
    query.bindValue(":skipDays", item.skipDays);
    query.bindValue(":id", item.id);
    if(query.exec()) {
        emit dataChanged();
        return true;
    }
    else {
        qDebug() << "Failed to update metadata: " << query.lastError();
        return false;
    }
}

bool DatabaseManager::feedUpdated(qint32 feed_id)
{
    qDebug() << feed_id;
    QSqlQuery query(m_db);
    query.prepare("UPDATE Feed SET last_update=datetime(\'now\') WHERE id=:id");
    query.bindValue(":id", feed_id);
    if(query.exec())
        return true;
    else {
        qDebug() << "Feed update time change failed: " << query.lastError();
        return false;
    }
}

bool DatabaseManager::removeFeed(qint32 feed_id)
{
    m_db.transaction();
    qDebug() << "Now removing: " <<  feed_id;
    // adjust rows
    qint32 folder_id = getFeedParentID(feed_id);
    QSqlQuery updateRows(m_db);
    updateRows.prepare("UPDATE Feed SET row_num = row_num-1 WHERE folder_id = :folder_id AND row_num > (SELECT row_num FROM Feed WHERE id=:feed_id)");
    updateRows.bindValue(":folder_id", folder_id);
    updateRows.bindValue(":feed_id", feed_id);
    if(!updateRows.exec()) {
        qDebug() << "Updating rows failed: " << updateRows.lastError();
        m_db.rollback();
        return false;
    }
    QSqlQuery updateRowsFolder(m_db);
    updateRowsFolder.prepare("UPDATE Folder SET row_num = row_num-1 WHERE parent_id = :folder_id AND row_num > (SELECT row_num FROM Feed WHERE id=:feed_id)");
    updateRowsFolder.bindValue(":feed_id", feed_id);
    updateRows.bindValue(":folder_id", folder_id);
    if(!updateRowsFolder.exec()) {
        qDebug() << "Updating rows for folder failed: " << updateRowsFolder.lastError();
        m_db.rollback();
        return false;
    }


    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Feed WHERE id=:feed_id");
    query.bindValue(":feed_id", feed_id);
    if(!query.exec()) {
        qDebug() << "Removal of feed failed: " << query.lastError();
        m_db.rollback();
        return false;
    }
    QSqlQuery getAllArticles(m_db);
    getAllArticles.prepare("SELECT id FROM Article WHERE feed_id = :feed_id");
    getAllArticles.bindValue(":feed_id", feed_id);

    if(!getAllArticles.exec()) {
        qDebug() << "Failed to get all articles to delete: " << getAllArticles.lastError();
        m_db.rollback();
        return false;
    }
    while(getAllArticles.next()) {
        removeMediaGroup(getAllArticles.value(0).toInt());
    }
    QSqlQuery removeArticle(m_db);
    removeArticle.prepare("DELETE FROM Article WHERE feed_id = :feed_id");
    removeArticle.bindValue(":feed_id", feed_id);
    if(!removeArticle.exec()) {
        qDebug() << "Failed to remove article: " << removeArticle.lastError();
        m_db.rollback();
        return false;
    }
    else {
        m_db.commit();
        emit dataChanged();
        return true;
    }
}

bool DatabaseManager::removeMediaGroup(qint32 articleID)
{
    qDebug() << "Now removing all media groups for article: " << articleID;
    QSqlQuery getAllGroups(m_db);
    getAllGroups.prepare("SELECT id FROM MediaGroup WHERE article_id = :article_id");
    getAllGroups.bindValue(":article_id", articleID);
    if(!getAllGroups.exec()) {
        qDebug() << "Failed to get all MediaGroups: "<< getAllGroups.lastError();
        return false;
    }
    while (getAllGroups.next()) {
        removeMedia(getAllGroups.value(0).toInt());
    }
    QSqlQuery removeGroup(m_db);
    removeGroup.prepare("DELETE FROM MediaGroup WHERE article_id = :article_id");
    removeGroup.bindValue(":article_id", articleID);
    if(!removeGroup.exec()) {
        qDebug() << "Failed to remove group: " << removeGroup.lastError();
        return false;
    }
    return true;

}

bool DatabaseManager::removeMedia(qint32 mediaGroupID)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Media WHERE group_id = :group_id");
    query.bindValue(":group_id", mediaGroupID);
    if(!query.exec()) {
        qDebug() << "Failed to remove media: " << query.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::removeFolder(qint32 folder_id)
{
    // adjust rows
    qint32 parent_id = getFolderParentID(folder_id);
    QSqlQuery updateRows(m_db);
    updateRows.prepare("UPDATE Feed SET row_num = row_num-1 WHERE folder_id = :parent_id AND row_num > (SELECT row_num FROM Folder WHERE id=:folder_id)");
    updateRows.bindValue(":folder_id", folder_id);
    updateRows.bindValue(":parent_id", parent_id);
    if(!updateRows.exec()) {
        qDebug() << "Updating rows failed: " << updateRows.lastError();
        return false;
    }
    QSqlQuery updateRowsFolder(m_db);
    updateRowsFolder.prepare("UPDATE Folder SET row_num = row_num-1 WHERE parent_id = :parent_id AND row_num > (SELECT row_num FROM Folder WHERE id=:folder_id)");
    updateRowsFolder.bindValue(":folder_id", folder_id);
    updateRows.bindValue(":parent_id", parent_id);
    if(!updateRowsFolder.exec()) {
        qDebug() << "Updating rows for folder failed: " << updateRowsFolder.lastError();
        return false;
    }

    // remove child items
    removeChildren(folder_id);

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Folder WHERE id=:folder_id");
    query.bindValue(":folder_id", folder_id);
    if(!query.exec()) {
        qDebug() << "Removal of folder failed: " << query.lastError();
        return false;
    }
    else {
        emit dataChanged();
        return true;
    }
}

bool DatabaseManager::removeChildren(qint32 folder_id)
{
    // remove all children of children
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM Folder WHERE parent_id = :folder_id");
    query.bindValue(":folder_id", folder_id);
    if(!query.exec()) {
        qDebug() << "Getting all child folders failed: " << query.lastError();
        return false;
    }
    while(query.next()) {
        removeChildren(query.value("id").toInt());
    }
    QSqlQuery removeFeeds(m_db);
    removeFeeds.prepare("DELETE FROM Feed WHERE folder_id = :folder_id");
    removeFeeds.bindValue(":folder_id", folder_id);
    if(!removeFeeds.exec()) {
        qDebug() << "Problem removing feeds: " << removeFeeds.lastError();
        return false;
    }
    else return true;



}

bool DatabaseManager::updateFeed(qint32 id, QString title, QString url, qint32 updatePeriod)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Feed SET display_title = :title, url = :url, update_period = :update_period WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":title", title);
    query.bindValue(":url", url);
    query.bindValue(":update_period", updatePeriod);
    if(!query.exec()) {
        qDebug() << "Updating feed failed: " << query.lastError();
        return false;
    }
    else {
        emit dataChanged();
        return true;
    }
}

bool DatabaseManager::updateFolder(qint32 id, QString title, qint32 updatePeriod)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Folder SET title = :title, update_period = :update_period WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":title", title);
    query.bindValue(":update_period", updatePeriod);
    if(!query.exec()) {
        qDebug() << "Updating folder failed: " << query.lastError();
        return false;
    }
    else {
        emit dataChanged();
        return true;
    }
}

bool DatabaseManager::setFilterFeed(qint32 id, QString globalFilter, QString titleFilter, QString descriptionFilter, QString categoryFilter)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Feed SET global_filter = :global_filter, title_filter = :title_filter, description_filter = :description_filter, category_filter = :category_filter WHERE id=:id");
    query.bindValue(":global_filter", globalFilter);
    query.bindValue(":title_filter", titleFilter);
    query.bindValue(":description_filter", descriptionFilter);
    query.bindValue(":category_filter", categoryFilter);
    query.bindValue(":id", id);
    if(!query.exec()) {
        qDebug() << "Setting filter for feed failed: " << query.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::setFilterFolder(qint32 id, QString globalFilter, QString titleFilter, QString descriptionFilter, QString categoryFilter)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Folder SET global_filter = :global_filter, title_filter = :title_filter, description_filter = :description_filter, category_filter = :category_filter WHERE id=:id");
    query.bindValue(":global_filter", globalFilter);
    query.bindValue(":title_filter", titleFilter);
    query.bindValue(":description_filter", descriptionFilter);
    query.bindValue(":category_filter", categoryFilter);
    query.bindValue(":id", id);
    if(!query.exec()) {
        qDebug() << "Setting filter for folder failed: " << query.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::setArticleToRead(qint32 id)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Article SET read=1 WHERE id=:id");
    query.bindValue(":id", id);
    if(!query.exec()) {
        qDebug() << "Failed to set article to read" << query.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::alterFavouriteState(qint32 articleID)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Article SET favourite=(CASE WHEN favourite = 1 THEN 0 ELSE 1 END) WHERE id=:id");
    query.bindValue(":id", articleID);
    if(!query.exec()) {
        qDebug() << "Failed to alter favourite state for article: " << query.lastError();
        return false;
    }
    emit articleChanged(articleID);
    return true;
}

void DatabaseManager::insertTestData()
{
    addFolder("TestingFilter", 0);
    setFilterFolder(1, "", "sql\nphp", "", "");
    //addFolder("Test2", 0, 1);
    //addFolder("Test3", 1, 0);

    //addConfiguration(2);

    //addFeed("xkcd", "https://www.xkcd.com/rss.xml", 0, 2, 1);
}

bool DatabaseManager::checkArticle(ArticleItem item)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM Article WHERE feed_id IS :id AND title IS :title AND link IS :link AND description IS :description AND author IS :author AND category IS :category AND comments IS :comments AND enclosure IS :enclosure "
                            "AND guid IS :guid AND pubDate IS :pubDate AND source IS :source");
    query.bindValue(":title", item.title);
    query.bindValue(":link", item.link);
    query.bindValue(":description", item.description);
    query.bindValue(":author", item.author);
    query.bindValue(":category", item.category);
    query.bindValue(":comments", item.comments);
    query.bindValue(":enclosure", item.enclosure);
    query.bindValue(":guid", item.guid);
    query.bindValue(":pubDate", item.pubDate);
    query.bindValue(":source", item.source);
    query.bindValue(":id", item.feed_id);
    if(query.exec())
        return query.next();
    else {
        qDebug() << "Failed to check query: " << query.lastError();
        return 0;
    }
}

QSqlQuery DatabaseManager::getArticle(qint32 id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Article WHERE id = :id");
    query.bindValue(":id", id);
    if(!query.exec()) {
        qDebug() << "Error getting article: " << query.lastError();
    }
    return query;
}

qint32 DatabaseManager::getUpdatePeriod(qint32 id)
{
    if(id == 0) return 10; // if item is root item return default time
    QSqlQuery query(m_db);
    query.prepare("SELECT update_period FROM Folder WHERE id = :id");
    query.bindValue(":id", id);
    if(!query.exec()) {
        qDebug() << "Getting update period failed: " << query.lastError();
        return -1;
    }
    if(!query.next()) {
        qDebug() << "No such folder exists";
        return -1;
    }
    return query.value("update_period").toInt();
}

qint32 DatabaseManager::addMediaGroupWithReturn(MediaGroup group)
{
    QSqlQuery getID(m_db);
    getID.prepare("SELECT MAX(id)+1 FROM MediaGroup");
    if(!getID.exec() || !getID.next()) {
        qDebug() << "Error getting new media group ID" << getID.lastError();
        return -1;
    }
    qint32 id = getID.value(0).toInt();
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO MediaGroup (id, article_id) VALUES (:id, :article_id)");
    query.bindValue(":id", id);
    query.bindValue(":article_id", group.articleID);
    if(query.exec()) {
        return id;
    }
    else {
        qDebug() << "Error adding media group with return: " << query.lastError();
        return -1;
    }
}

qint32 DatabaseManager::getNextArticleID()
{
    QSqlQuery getID(m_db);
    getID.prepare("SELECT MAX(id) + 1 FROM Article");
    if(!getID.exec() || !getID.next()) {
        qDebug() << "Failed to get new ID: " << getID.lastError();
        return -1;
    }
    return getID.value(0).toInt();
}

QString DatabaseManager::getSetting(QString name)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT value FROM Settings WHERE name = :name");
    query.bindValue(":name", name);
    if(!query.exec()) {
        qDebug() << "Get setting failed: " << query.lastError();
    }
    if(!query.next()) {
        return "";
    }
    return query.value("value").toString();
}

bool DatabaseManager::setSetting(QString name, QString value)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM Settings WHERE name = :name");
    query.bindValue(":name", name);
    if(!query.exec()) {
        qDebug() << "Settings change failed";
        return false;
    }
    else if(query.next()) {
        QSqlQuery changeSetting(m_db);
        changeSetting.prepare("UPDATE Settings SET value = :value WHERE name=:name");
        changeSetting.bindValue(":name", name);
        changeSetting.bindValue(":value", value);
        if(!changeSetting.exec()) {
            qDebug() << "Updating settings failed: " << changeSetting.lastError();
            return false;
        }
        else return true;
    }
    else {
        QSqlQuery insertSetting(m_db);
        insertSetting.prepare("INSERT INTO Settings (value, name) VALUES(:value, :name)");
        insertSetting.bindValue(":name", name);
        insertSetting.bindValue(":value", value);
        if(!insertSetting.exec()) {
            qDebug() << "Inserting settings failed: " << insertSetting.lastError();
            return false;
        }
        else return true;
    }
}

QString DatabaseManager::createFeedsTableQuery()
{
    QString createTable (
        "CREATE TABLE IF NOT EXISTS Feed ("
        // --- Structure data --- //
        "id INTEGER PRIMARY KEY,"
        "folder_id  INTEGER,"
        "row_num    INTEGER,"
        "display_title TEXT,"
        // --- Feed data --- //
        "title TEXT,"
        "link TEXT,"
        "description TEXT,"

        "language TEXT,"
        "copyright TEXT,"
        "managingEditor TEXT,"
        "webMaster TEXT,"
        "pubDate TEXT,"
        "lastBuildDate TEXT,"
        "category TEXT,"
        "generator TEXT,"
        "docs TEXT,"
        "cloud TEXT,"
        "ttl TEXT,"
        "image TEXT,"
        "rating TEXT,"
        "textInput TEXT,"
        "skipHours TEXT,"
        "skipDays TEXT,"

        // --- Meta data --- //
        "url   TEXT,"
        "last_update INTEGER,"
        //"configuration_id INTEGER,"
        "update_period INTEGER,"


        // --- Filter data --- //
        "global_filter TEXT,"
        "title_filter TEXT,"
        "description_filter TEXT,"
        "category_filter TEXT,"
        // --- Integrity rules --- //
        "FOREIGN KEY (folder_id) REFERENCES Folder(id)"
        //"FOREIGN KEY (configuration_id) REFERENCES Configuration(id) ON DELETE CASCADE"
        ");"
);
    return createTable;
}

QString DatabaseManager::createArticleTableQuery()
{
    QString createTable (
        "CREATE TABLE IF NOT EXISTS Article ("
        // --- Meta data --- //
        "id INTEGER PRIMARY KEY,"
        "feed_id INTEGER,"
        "read INTEGER DEFAULT 0,"
        "favourite INTEGER DEFAULT 0,"

        // --- Article content --- //
        "title TEXT,"
        "link  TEXT,"
        "description TEXT,"
        "author TEXT,"
        "category TEXT,"
        "comments TEXT,"
        "enclosure TEXT,"
        "guid TEXT,"
        "pubDate TEXT,"
        "source TEXT,"

        // --- Integrity rules --- //
        "FOREIGN KEY (feed_id) REFERENCES Feed(id)"
        ");"
);
    return createTable;
}

QString DatabaseManager::createFolderTableQuery()
{
    QString createTable (
        "CREATE TABLE IF NOT EXISTS Folder ("
        // --- Meta data --- //
        "id         INTEGER,"
        "parent_id  INTEGER,"
        "row_num    INTEGER,"
        "title TEXT,"
        "update_period INTEGER,"

        // --- Filter data --- //
        "global_filter TEXT,"
        "title_filter TEXT,"
        "description_filter TEXT,"
        "category_filter TEXT,"

        // --- Integrity rules --- //
        "PRIMARY KEY (id),"
        "FOREIGN KEY (parent_id) REFERENCES Folder(id) ON DELETE CASCADE"
        ");"
    );
    return createTable;
}

QString DatabaseManager::createMediaTableQuery()
{
    QString createTable(
        "CREATE TABLE IF NOT EXISTS Media ("
        "id INTEGER,"
        "group_id INTEGER,"
        "medium INTEGER,"
        "url TEXT,"
        "height INTEGER,"
        "width INTEGER,"
        "type TEXT,"

        // --- Integrity rules --- //
        "PRIMARY KEY(id)"
        ");"
    );
    return createTable;
}

QString DatabaseManager::createMediaGroupTableQuery()
{
    QString createTable(
        "CREATE TABLE IF NOT EXISTS MediaGroup ("
        "id INTEGER PRIMARY KEY,"
        "article_id INTEGER"
        ");"
    );
    return createTable;
}

QString DatabaseManager::createConfigurationTableQuery()
{
    QString createTable (
        "CREATE TABLE IF NOT EXISTS Configuration ("
        "id INTEGER,"
        "update_period INTEGER," // in minutes
        // the rest of the filters
        "PRIMARY KEY(id)"
        ");"
    );
    return createTable;
}

QString DatabaseManager::createSettingsTableQuery()
{
    QString createTable(
        "CREATE TABLE IF NOT EXISTS Settings("
        "id INTEGER,"
        "name TEXT,"
        "value TEXT,"
        "PRIMARY KEY(id)"
        ");"
    );
    return createTable;
}
