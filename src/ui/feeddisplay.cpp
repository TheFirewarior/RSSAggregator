#include "feeddisplay.h"

void FeedDisplay::createActions()
{
    m_addFeed = new QAction(tr("Add feed"), this);
    m_addFolder = new QAction(tr("Add folder"), this);
    m_removeItem = new QAction(tr("Remove"), this);
    m_properties = new QAction(tr("Properties"), this);
}

void FeedDisplay::handleContextMenu(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    m_addFeed->setData(indexAt(pos));
    m_addFolder->setData(indexAt(pos));
    m_removeItem->setData(indexAt(pos));
    m_properties->setData(indexAt(pos));
    menu->addAction(m_addFeed);
    menu->addAction(m_addFolder);
    if(indexAt(pos) != rootIndex()) {
        menu->addAction(m_removeItem);
        menu->addAction(m_properties);
    }
    menu->exec(mapToGlobal(pos));
}

FeedDisplay::FeedDisplay(QWidget *parent)
    : QTreeView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    createActions();
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(handleContextMenu(QPoint)));
}

FeedDisplay::~FeedDisplay()
{

}
