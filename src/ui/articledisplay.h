#ifndef ARTICLEDISPLAY_H
#define ARTICLEDISPLAY_H

#include <QWidget>
#include <QFrame>
#include <QWebEngineView>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QTabWidget>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include <QScrollArea>

#include "databasemanager.h"
#include "mediadisplay.h"

class ArticleDisplay : public QWidget
{
    Q_OBJECT
    QTabWidget *m_tabWidget;
    QScrollArea *m_scrollArea;
    qint32 m_articleID;
    DatabaseManager *m_db;
    QWebEngineView *m_view;
    QWidget *createTitleBar();
    QLabel *m_title;
    QLabel *m_author;
    QLabel *m_date;
    void createLayout();
    void createWebView();
    void createTabWidget();
    void createMediaDisplay();

    // article data
    QString m_articleTitle;
    QString m_articleAuthor;
    QString m_articleDate;
    QString m_url;
    QString m_description;
    MediaDisplay *m_media;

    void getArticleData();

    // title context menu
    QAction *m_openLink;
    QAction *m_openLinkExternally;

    void createActions();
    void createMenu();
public:
    explicit ArticleDisplay(DatabaseManager *db, QWidget *parent = nullptr);
signals:

public slots:
    void setArticle(qint32 articleID);
    void update();
private slots:
    void handleLinkPressed(QString link);
    void openLink();
    void openLinkExternally();
    void createTitleContextMenu(QPoint pos);
    void closeTab(int index);
};

#endif // ARTICLEDISPLAY_H
