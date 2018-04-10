#include "articlelistdisplay.h"

void ArticleListDisplay::hideColumns(QAbstractItemModel *model)
{
    QSqlTableModel *modelSQL = static_cast<QSqlTableModel*>(static_cast<ArticleListProxyModel*>(model)->sourceModel());
    for(int i = 0; i < modelSQL->columnCount(); ++i) {
        if(i != modelSQL->fieldIndex("title") && i != modelSQL->fieldIndex("author") && i != modelSQL->fieldIndex("pubDate") && i != modelSQL->fieldIndex("category") && i != modelSQL->fieldIndex("favourite")) {
            hideColumn(i);
        }
    }
    horizontalHeader()->setSectionResizeMode(modelSQL->fieldIndex("title"), QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(modelSQL->fieldIndex("pubDate"), QHeaderView::ResizeToContents);
}

ArticleListDisplay::ArticleListDisplay(QWidget *parent)
    : QTableView(parent)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    verticalHeader()->hide();
    setSortingEnabled(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(handleClick(QModelIndex)));

}

void ArticleListDisplay::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    hideColumns(model);
    qDebug() << "property: " << property("readItemBackground").toString();
}

void ArticleListDisplay::handleClick(const QModelIndex &index)
{
    qDebug() << "property: " << property("readItemBackground").toString();
    qint32 row = index.row();
    qint32 article = model()->data(model()->index(row, 0)).toInt();
    qDebug() << "Article selected: " << article;
    emit articleSelected(article);
    static_cast<ArticleListProxyModel*>(model())->selectRow(row);
}

void ArticleListDisplay::mousePressEvent(QMouseEvent *event)
{
    if (indexAt(event->pos()).column() == static_cast<ArticleListProxyModel*>(model())->fieldIndex("favourite")) {
        QModelIndex pressed = indexAt(event->pos());
        QModelIndex idField = model()->index(pressed.row(), static_cast<ArticleListProxyModel*>(model())->fieldIndex("id"));
        emit starPressed(idField.data().toInt());
    }
    QTableView::mousePressEvent(event);
}

QVariant ArticleListDisplay::getReadItemBackground()
{
    return m_readItemBackground;
}

void ArticleListDisplay::setReadItemBackground(QVariant value)
{
    m_readItemBackground = value;
}
