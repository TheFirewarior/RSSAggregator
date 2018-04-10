#ifndef ARTICLELISTDELEGATE_H
#define ARTICLELISTDELEGATE_H
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPolygon>
#include <QBrush>
#include <QDebug>

#include "favouritestar.h"
#include "articlelistproxymodel.h"

class ArticleListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    FavouriteStar *m_favouriteStar;
public:
    ArticleListDelegate(QWidget *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
public slots:
};

#endif // ARTICLELISTDELEGATE_H
