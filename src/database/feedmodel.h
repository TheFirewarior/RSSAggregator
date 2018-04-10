#ifndef FEEDMODEL_H
#define FEEDMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QMap>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMapIterator>
#include <QDebug>
#include <QAction>

#include "databasemanager.h"
#include "addfeeddialog.h"
#include "addfolderdialog.h"
#include "changefeeddialog.h"
#include "changefolderdialog.h"

class FeedModel : public QAbstractItemModel {
    Q_OBJECT
    DatabaseManager *m_db;
public:
    FeedModel(DatabaseManager *db);
    ~FeedModel();
private:
    struct ItemData {
        QList<QString> childItems;
        QString title;
        QString parent;
        qint32 row;
    };
    QMap<QString, ItemData*> m_items;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    ItemData *rootItem;
public slots:
    void handleClick(const QModelIndex& index);
    void handleAddFeed();
    void handleAddFolder();
    void handleRemoveItem();
    void handleProperties();
private slots:
    void refreshModel();
signals:
    //void itemClicked(QString);
    void itemSelected(QString item);
};

#endif // FEEDMODEL_H
