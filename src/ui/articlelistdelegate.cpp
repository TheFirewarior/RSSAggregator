#include "articlelistdelegate.h"

ArticleListDelegate::ArticleListDelegate(QWidget *parent): QStyledItemDelegate(parent)
{
    m_favouriteStar = new FavouriteStar;
}

void ArticleListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == static_cast<const ArticleListProxyModel*>(index.model())->fieldIndex("favourite")) {
        m_favouriteStar->setState(index.data().toBool());
        m_favouriteStar->paint(painter, option.rect, option.palette);
    }
    else
        QStyledItemDelegate::paint(painter, option, index);

}

QSize ArticleListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.model()->headerData(index.column(), Qt::Horizontal).toString() == "favourite") {
        return m_favouriteStar->sizeHint(option.rect);
    }
    else {
        return QStyledItemDelegate::sizeHint(option, index);
    }

}
