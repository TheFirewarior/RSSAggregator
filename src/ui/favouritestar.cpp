#include "favouritestar.h"

FavouriteStar::FavouriteStar()
{
    m_state = false;
    star = new QPolygonF;
    test = new QPolygonF;
    for (int i = 0; i < 5; ++i) {
        double perCircle = i / 5.0;
        double perInner = (i+3)%5 / 5.0;
        qDebug() << 2*QPointF(std::sin(perCircle*2*PI), (-1)*std::cos(perCircle*2*PI));
        qDebug() << QPointF((-1)*std::sin(perCircle*2*PI), std::cos(perCircle*2*PI));
        star->append(2*QPointF(std::sin(perCircle*2*PI), (-1)*std::cos(perCircle*2*PI)));
        star->append(QPointF((-1)*std::sin(perInner*2*PI), std::cos(perInner*2*PI)));
    }

}

void FavouriteStar::paint(QPainter *painter, const QRect &rect, const QPalette &pallete) const
{
    painter->save();
    if(m_state) {
        painter->setBrush(pallete.dark());
    }
    else{
        painter->setBrush(Qt::NoBrush);
    }

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 0.1));
    painter->translate(rect.x() + rect.width()/2, rect.y() + rect.height()/2);
    painter->scale(5, 5);

    //painter->drawPolygon(*test, Qt::WindingFill);
    //painter->setBrush(pallete.light());
    painter->drawPolygon(*star, Qt::WindingFill);
    //painter->drawPolygon(*star);
    painter->restore();
}

QSize FavouriteStar::sizeHint(const QRect &rect)
{
    return QSize(10,10);
}

void FavouriteStar::setState(bool state)
{
    m_state = state;
}
