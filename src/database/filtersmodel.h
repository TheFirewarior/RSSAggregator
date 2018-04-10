#ifndef FILTERSMODEL_H
#define FILTERSMODEL_H
#include <QAbstractListModel>

class FiltersModel : public QAbstractListModel
{
    Q_OBJECT
    QStringList m_keywords;
public:
    FiltersModel(const QStringList& keywords, QObject *parent);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int row, const QModelIndex &parent);
    bool removeRow(int row, const QModelIndex &parent);

    const QStringList& getUpdatedKeywords() const;
public slots:
    bool addRow();
    bool delRow(qint32 selected);
};

#endif // FILTERSMODEL_H
