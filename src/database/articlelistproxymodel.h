#ifndef ARTICLELISTPROXYMODEL_H
#define ARTICLELISTPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QSqlTableModel>
#include <QDebug>

enum class DisplayFilters {
    ALL,
    READ,
    UNREAD,
    FAVOURITE
};

class ArticleListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    DisplayFilters m_filter;
public:
    ArticleListProxyModel(QObject * parent = 0);
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool isRead(int source_row, const QModelIndex &source_parent) const;
    bool isFavourite(int source_row, const QModelIndex &source_parent) const;
    qint32 fieldIndex(QString field) const;
public slots:
    void setFilter(DisplayFilters filter);
    void selectRow(int row);
};

#endif // ARTICLELISTPROXYMODEL_H
