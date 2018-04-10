#ifndef ARTICLELISTDISPLAY_H
#define ARTICLELISTDISPLAY_H

#include <QTableView>
#include <QHeaderView>
#include <QDebug>
#include <QSqlTableModel>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <QMetaProperty>
#include <QApplication>

#include "articlelistproxymodel.h"

class ArticleListDisplay : public QTableView
{
    Q_OBJECT
    Q_PROPERTY(QVariant readItemBackground READ getReadItemBackground WRITE setReadItemBackground DESIGNABLE true)
    void hideColumns(QAbstractItemModel *model);
public:
    ArticleListDisplay(QWidget *parent = 0);
    void setModel(QAbstractItemModel *model);
private slots:
    void handleClick(const QModelIndex& index);
    void mousePressEvent(QMouseEvent *event);
signals:
    void articleSelected(qint32 articleID);
    void starPressed(qint32 articleID);
private:
    QVariant getReadItemBackground();
    void setReadItemBackground(QVariant value);

    QVariant m_readItemBackground;
};

#endif // ARTICLELISTDISPLAY_H
