#ifndef FAVOURITESTAR_H
#define FAVOURITESTAR_H
#include <QPainter>
#include <QRect>
#include <QPalette>
#include <QPolygonF>
#include <QPointF>
#include <cmath>
#include <QDebug>

#define PI 3.14159265

class FavouriteStar
{
public:
    FavouriteStar();
    QPolygonF *star;
    QPolygonF *test;
    bool m_state;
    void paint(QPainter *painter, const QRect &rect, const QPalette &pallete) const;
    QSize sizeHint(const QRect &rect);
public slots:
    void setState(bool state = false);
};

#endif // FAVOURITESTAR_H
