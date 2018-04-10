#ifndef FEEDDISPLAY_H
#define FEEDDISPLAY_H

#include <QTreeView>
#include <QAction>
#include <QMenu>

class FeedDisplay : public QTreeView
{
    Q_OBJECT

    void createActions();
public slots:
    void handleContextMenu(const QPoint &pos);
public:
    FeedDisplay(QWidget *parent = 0);
    QAction *m_addFeed;
    QAction *m_addFolder;
    QAction *m_removeItem;
    QAction *m_properties;
    ~FeedDisplay();
};

#endif // FEEDDISPLAY_H
