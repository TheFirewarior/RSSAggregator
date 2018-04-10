#include "articlelistproxymodel.h"

ArticleListProxyModel::ArticleListProxyModel(QObject *parent): QSortFilterProxyModel(parent)
{

}

bool ArticleListProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    switch(m_filter) {
        case DisplayFilters::ALL:
            return true;
        break;
        case DisplayFilters::READ:
            return isRead(source_row,  source_parent);
        break;
        case DisplayFilters::UNREAD:
            return !isRead(source_row, source_parent);
        break;
        case DisplayFilters::FAVOURITE:
            return isFavourite(source_row, source_parent);
        break;

    }
}

bool ArticleListProxyModel::isRead(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex readIndex = sourceModel()->index(source_row, static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("read"), source_parent);
    if(sourceModel()->data(readIndex).toInt() == 1) {
        return true;
    }
    else return false;
}

bool ArticleListProxyModel::isFavourite(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex favouriteIndex = sourceModel()->index(source_row, static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("favourite"), source_parent);
    if(sourceModel()->data(favouriteIndex).toInt() == 1) {
        return true;
    }
    else return false;
}

qint32 ArticleListProxyModel::fieldIndex(QString field) const
{
    return static_cast<QSqlTableModel*>(sourceModel())->fieldIndex(field);
}



void ArticleListProxyModel::setFilter(DisplayFilters filter)
{
    qDebug() << "Changing filter";
    m_filter = filter;
    invalidateFilter();
}

void ArticleListProxyModel::selectRow(int row)
{
    static_cast<QSqlTableModel*>(sourceModel())->selectRow(mapToSource(index(row, 0)).row());
}
