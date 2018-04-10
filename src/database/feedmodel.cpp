#include "feedmodel.h"


FeedModel::FeedModel(DatabaseManager *db)
{
    m_db = db;
    //m_db->insertTestData();
    refreshModel();
    connect(m_db, SIGNAL(dataChanged()), this, SLOT(refreshModel()));
}

FeedModel::~FeedModel()
{
    qDeleteAll(m_items);
}

void FeedModel::refreshModel()
{
    beginResetModel();
    m_items.clear();
    //m_db->insertTestData();
    // add folders
    QSqlQuery folders = m_db->getFolderViewData();
    while(folders.next()) {
        ItemData *data = new ItemData;
        data->title = folders.value("title").toString();
        data->parent= "Fo" + QString::number(folders.value("parent_id").toInt());
        data->row   = folders.value("row_num").toInt();
        m_items.insert("Fo"+folders.value("id").toString(), data);
    }

    // add feeds
    QSqlQuery feeds = m_db->getFeedsViewData();
    while(feeds.next()) {
        ItemData *data = new ItemData;
        data->title = feeds.value("display_title").toString();
        data->parent= "Fo" + QString::number(feeds.value("folder_id").toInt());
        data->row   = feeds.value("row_num").toInt();
        m_items.insert("Fe"+feeds.value("id").toString(), data);
    }

    // process children
    rootItem = new ItemData;
    QMapIterator<QString, ItemData*> i(m_items);
    while(i.hasNext()) {
        i.next();
        if(i.value()->parent == "Fo0")
            rootItem->childItems.append(i.key());
        else {
            m_items[i.value()->parent]->childItems.append(i.key());
        }
    }
    endResetModel();
}

int FeedModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid()) return rootItem->childItems.count();
    return static_cast<ItemData*>(parent.internalPointer())->childItems.count();
}

int FeedModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant FeedModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole)
        return static_cast<ItemData*>(index.internalPointer())->title;
    else return QVariant();
}

QVariant FeedModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant(tr("Feeds"));
}

QModelIndex FeedModel::parent(const QModelIndex &child) const
{
    if(!child.isValid()) return QModelIndex();
    ItemData *data = static_cast<ItemData*> (child.internalPointer());
    if(data->parent == "Fo0") return QModelIndex();
    return createIndex(m_items[data->parent]->row, 0, m_items[data->parent]);
}

QModelIndex FeedModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();
    ItemData *parentItem;
    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ItemData*> (parent.internalPointer());
    foreach(QString child, parentItem->childItems) {
        if(m_items[child]->row == row)
            return createIndex(row, column, m_items[child]);
    }
}

void FeedModel::handleClick(const QModelIndex &index)
{
    ItemData *clicked = static_cast<ItemData*>(index.internalPointer());
    QString clickedID = m_items.key(clicked);
    emit itemSelected(clickedID);
}

void FeedModel::handleAddFeed()
{
    QAction *feedData = qobject_cast<QAction *> (sender());
    ItemData *item = static_cast<ItemData*>(feedData->data().toModelIndex().internalPointer());

    // if item is feed add to parent folder instead
    QString clickedID = m_items.key(item);
    if(clickedID.left(2) == 'Fe') {
        item = m_items[item->parent];
        clickedID = m_items.key(item);
    }
    qDebug() << "Add feed clicked on: " << clickedID;
    qint32 pressed;
    if(clickedID == "")
        pressed = 0;
    else {
        pressed = clickedID.right(clickedID.count()-2).toInt();
    }
    AddFeedDialog *dialog = new AddFeedDialog(m_db);
    dialog->setDefaultSelected(pressed);
    dialog->show();
}

void FeedModel::handleAddFolder()
{
    QAction *feedData = qobject_cast<QAction *> (sender());
    ItemData *item = static_cast<ItemData*>(feedData->data().toModelIndex().internalPointer());

    // if item is feed add to parent folder instead
    QString clickedID = m_items.key(item);
    if(clickedID.left(2) == 'Fe') {
        item = m_items[item->parent];
        clickedID = m_items.key(item);
    }
    qDebug() << "Add folder clicked on: " << clickedID;
    qint32 pressed;
    if(clickedID == "")
        pressed = 0;
    else {
        pressed = clickedID.right(clickedID.count()-2).toInt();
    }
    AddFolderDialog *dialog = new AddFolderDialog(m_db);
    dialog->setDefaultSelected(pressed);
    dialog->show();
}

void FeedModel::handleRemoveItem()
{
    QAction *feedData = qobject_cast<QAction *> (sender());
    ItemData *item = static_cast<ItemData*>(feedData->data().toModelIndex().internalPointer());

    QString clickedID = m_items.key(item);
    qDebug() << "Now removing: " << clickedID;
    if(clickedID.left(2) == "Fo") {
        m_db->removeFolder(clickedID.right(clickedID.count()-2).toInt());
    }
    else if(clickedID.left(2) == "Fe") {
        m_db->removeFeed(clickedID.right(clickedID.count()-2).toInt());
    }
}

void FeedModel::handleProperties()
{
    QAction *feedData = qobject_cast<QAction *> (sender());
    ItemData *item = static_cast<ItemData*>(feedData->data().toModelIndex().internalPointer());

    QString clickedID = m_items.key(item);
    if(clickedID.left(2) == "Fo") {
        ChangeFolderDialog *cfDialog = new ChangeFolderDialog(clickedID.right(clickedID.count()-2).toInt(), m_db);
        cfDialog->show();
    }
    else if(clickedID.left(2) == "Fe") {
        ChangeFeedDialog *cfDialog = new ChangeFeedDialog(clickedID.right(clickedID.count()-2).toInt(), m_db);
        cfDialog->show();
    }
}

