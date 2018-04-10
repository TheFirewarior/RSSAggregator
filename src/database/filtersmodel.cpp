#include "filtersmodel.h"
FiltersModel::FiltersModel(const QStringList &keywords, QObject *parent): QAbstractListModel(parent), m_keywords(keywords)
{

}

int FiltersModel::rowCount(const QModelIndex &parent) const
{
    return m_keywords.size();
}

QVariant FiltersModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }
    if(index.row() >= m_keywords.size()) {
        return QVariant();
    }
    if(role == Qt::DisplayRole || role == Qt::EditRole) {
        return m_keywords.at(index.row());
    }
    else return QVariant();
}

QVariant FiltersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();

    if (orientation == Qt::Horizontal)
        return tr("Keywords");
    else
        return QVariant();
}

Qt::ItemFlags FiltersModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()) {
        return Qt::ItemIsEnabled;
    }
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool FiltersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole) {
        m_keywords.replace(index.row(), value.toString());
        emit dataChanged(index, index);
    }
}

bool FiltersModel::insertRow(int row, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), row, row);
    m_keywords.append("");
    endInsertRows();
    return true;
}

bool FiltersModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row);
    m_keywords.removeAt(row);
    endRemoveRows();
    return true;
}

bool FiltersModel::addRow()
{
    insertRow(rowCount(QModelIndex()), QModelIndex());
    return true;
}

bool FiltersModel::delRow(qint32 selected)
{
    removeRow(selected, QModelIndex());
    return true;
}

const QStringList &FiltersModel::getUpdatedKeywords() const
{
    return m_keywords;
}
