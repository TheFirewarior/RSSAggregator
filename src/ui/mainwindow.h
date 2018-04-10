#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QThread>
#include <QAction>
#include <QMenuBar>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QToolBar>
#include <QApplication>
#include <QWidget>

#include "feeddisplay.h"
#include "articlelistdisplay.h"
#include "feedmodel.h"
#include "databasemanager.h"
#include "articlelistmodel.h"
#include "downloadmanager.h"
#include "articledisplay.h"
#include "articlelistproxymodel.h"
#include "articlelistdelegate.h"
#include "commonstrings.h"
#include "settingsdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    DatabaseManager *m_db;

    QToolBar *m_toolbar;
    // Properties
    Q_PROPERTY(QString settingsIcon READ getSettingsIcon WRITE setSettingsIcon DESIGNABLE true)
    Q_PROPERTY(QString feedAddIcon READ getFeedAddIcon WRITE setFeedAddIcon DESIGNABLE true)
    Q_PROPERTY(QString folderAddIcon READ getFolderAddIcon WRITE setFolderAddIcon DESIGNABLE true)

public:
    explicit MainWindow(DatabaseManager *db, QWidget *parent = nullptr);
    ~MainWindow();
    FeedDisplay *feedDisplay;
    ArticleListDisplay *articleListDisplay;
    FeedModel *feedModel;
    ArticleListModel *articleModel;
    ArticleListProxyModel *articleListProxy;
    ArticleDisplay *articleDisplay;
    QTreeWidget *displayFilter;



signals:

public slots:
    void createFeedDialog();
    void createFolderDialog();
    void createSettingsDialog();
private:
    DownloadManager *m_manager;
    QThread *m_networkThread;
    void createFeedView();
    void createLayout();
    void createDownloadManager();
    void createArticleList();
    void createArticleDisplay();
    void createDisplayFilter();

    void createMenuBar();
    // file section
    QMenu *m_fileMenu;
    QMenu *m_viewMenu;
    QMenu *m_articleMenu;
    QAction *m_createFeedAction;
    QAction *m_createFolderAction;
    QAction *m_settingsAction;
    void createFileMenuBarItem();

    QString m_settingsIcon;
    QString getSettingsIcon();
    void setSettingsIcon(QString icon);

    QString m_feedAddIcon;
    QString getFeedAddIcon();
    void setFeedAddIcon(QString icon);

    QString m_folderAddIcon;
    QString getFolderAddIcon();
    void setFolderAddIcon(QString icon);
private slots:
    void displayFilterPressed(QTreeWidgetItem *item, int column);
};

#endif // MAINWINDOW_H
