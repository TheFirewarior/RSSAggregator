#include "mainwindow.h"


MainWindow::MainWindow(DatabaseManager * db, QWidget *parent) : QMainWindow(parent)
{
    m_db = db;
    resize(1280, 900);
    createFeedView();
    createArticleList();
    createArticleDisplay();
    createDisplayFilter();
    //createFileMenuBarItem();
    createLayout();
    createDownloadManager();
    show();
    createMenuBar();
}

MainWindow::~MainWindow()
{
    m_db->deleteLater();
    m_networkThread->exit();
}

void MainWindow::createFeedDialog()
{
    AddFeedDialog *feed = new AddFeedDialog(m_db);
    feed->show();
}

void MainWindow::createFolderDialog()
{
    AddFolderDialog *folder = new AddFolderDialog(m_db);
    folder->show();
}

void MainWindow::createSettingsDialog()
{
    SettingsDialog *dialog = new SettingsDialog(m_db);
    dialog->show();
}

void MainWindow::createFeedView()
{
    feedModel = new FeedModel(m_db);
    feedDisplay = new FeedDisplay(this);
    feedDisplay->setModel(feedModel);
    connect(feedDisplay, SIGNAL(clicked(QModelIndex)), feedModel, SLOT(handleClick(QModelIndex)));
    connect(feedDisplay->m_addFeed, SIGNAL(triggered()), feedModel, SLOT(handleAddFeed()));
    connect(feedDisplay->m_addFolder, SIGNAL(triggered()), feedModel, SLOT(handleAddFolder()));
    connect(feedDisplay->m_removeItem, SIGNAL(triggered()), feedModel, SLOT(handleRemoveItem()));
    connect(feedDisplay->m_properties, SIGNAL(triggered()), feedModel, SLOT(handleProperties()));
}

void MainWindow::createLayout()
{
    // creation
    QSplitter *central = new QSplitter(this);
    central->setOrientation(Qt::Horizontal);
    QSplitter *right   = new QSplitter(central);
    right->setOrientation(Qt::Vertical);
    QSplitter *left    = new QSplitter(central);
    left->setOrientation(Qt::Vertical);

    // add left and right side
    central->addWidget(left);
    central->addWidget(right);

    // add to right side
    right->addWidget(articleListDisplay);
    right->addWidget(articleDisplay);
    setCentralWidget(central);

    // add to left side
    left->addWidget(feedDisplay);
    left->addWidget(displayFilter);
}

void MainWindow::createDownloadManager() {
    m_networkThread = new QThread(this);
    m_manager = new DownloadManager();
    m_manager->moveToThread(m_networkThread);
    m_networkThread->start();
    connect(m_db, SIGNAL(feedAdded()), m_manager, SLOT(checkForUpdates()));
    m_manager->init();
}

void MainWindow::createArticleList()
{
    articleModel = new ArticleListModel(m_db, this);
    articleListProxy = new ArticleListProxyModel(this);
    articleListDisplay = new ArticleListDisplay(this);
    //articleListDisplay->setModel(articleModel);
    articleListProxy->setSourceModel(articleModel);
    articleListDisplay->setModel(articleListProxy);
    articleListDisplay->setItemDelegate(new ArticleListDelegate);
    articleListDisplay->show();
    connect(feedModel, SIGNAL(itemSelected(QString)), articleModel, SLOT(setNewID(QString)));
    connect(articleListDisplay, SIGNAL(articleSelected(qint32)), m_db, SLOT(setArticleToRead(qint32)), Qt::DirectConnection);
    connect(articleListDisplay, SIGNAL(starPressed(qint32)), m_db, SLOT(alterFavouriteState(qint32)));
    connect(m_db, SIGNAL(articleChanged(qint32)), articleModel, SLOT(handleArticleChanged(qint32)));
    articleModel->setReadColor(articleListDisplay->property("readItemBackground").toString());
}

void MainWindow::createArticleDisplay()
{
    articleDisplay = new ArticleDisplay(m_db, this);
    connect(articleListDisplay, SIGNAL(articleSelected(qint32)), articleDisplay, SLOT(setArticle(qint32)), Qt::DirectConnection);

}

void MainWindow::createDisplayFilter()
{
    displayFilter = new QTreeWidget(this);
    displayFilter->setColumnCount(1);
    displayFilter->setHeaderLabel(tr("Select filter"));
    // add different filters
    QTreeWidgetItem *all = new QTreeWidgetItem;
    all->setText(0, tr("All"));
    all->setData(1, Qt::UserRole, static_cast<int>(DisplayFilters::ALL));

    QTreeWidgetItem *unread = new QTreeWidgetItem;
    unread->setText(0, tr("Unread"));
    unread->setData(1, Qt::UserRole, static_cast<int>(DisplayFilters::UNREAD));

    QTreeWidgetItem *read = new QTreeWidgetItem;
    read->setText(0, tr("Read"));
    read->setData(1, Qt::UserRole, static_cast<int>(DisplayFilters::READ));

    QTreeWidgetItem *favourite = new QTreeWidgetItem;
    favourite->setText(0, tr("Favourite"));
    favourite->setData(1, Qt::UserRole, static_cast<int>(DisplayFilters::FAVOURITE));

    displayFilter->addTopLevelItem(all);
    displayFilter->addTopLevelItem(unread);
    displayFilter->addTopLevelItem(read);
    displayFilter->addTopLevelItem(favourite);

    connect(displayFilter, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(displayFilterPressed(QTreeWidgetItem*,int)));

}

void MainWindow::createMenuBar()
{
    QString appPath = QApplication::applicationDirPath();
    m_toolbar = addToolBar(tr("Tools"));
    qDebug() << m_folderAddIcon;
    // Create actions
    m_createFeedAction = new QAction(QIcon(appPath + STYLE_PATH + m_feedAddIcon),"");
    m_createFolderAction = new QAction(QIcon(appPath + STYLE_PATH + m_folderAddIcon),"");
    m_settingsAction = new QAction(QIcon(appPath + STYLE_PATH + m_settingsIcon),"");

    // Connect actions
    connect(m_createFeedAction, SIGNAL(triggered()), this, SLOT(createFeedDialog()));
    connect(m_createFolderAction, SIGNAL(triggered()), this, SLOT(createFolderDialog()));
    connect(m_settingsAction, SIGNAL(triggered()), this, SLOT(createSettingsDialog()));

    // Add left actions
    m_toolbar->addAction(m_createFeedAction);
    m_toolbar->addAction(m_createFolderAction);

    // Add spacer
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_toolbar->addWidget(spacer);

    // Add right actions
    m_toolbar->addAction(m_settingsAction);
}

void MainWindow::createFileMenuBarItem()
{
    m_createFeedAction = new QAction(tr("Create feed"), this);
    m_createFolderAction = new QAction(tr("Create folder"), this);
    connect(m_createFeedAction, SIGNAL(triggered()), this, SLOT(createFeedDialog()));
    connect(m_createFolderAction, SIGNAL(triggered()), this, SLOT(createFolderDialog()));
    m_fileMenu = menuBar()->addMenu(tr("File"));
    m_fileMenu->addAction(m_createFeedAction);
    m_fileMenu->addAction(m_createFolderAction);

}

QString MainWindow::getSettingsIcon()
{
    return m_settingsIcon;
}

void MainWindow::setSettingsIcon(QString icon)
{
    m_settingsIcon = icon;
}

QString MainWindow::getFeedAddIcon()
{
    return m_feedAddIcon;
}

void MainWindow::setFeedAddIcon(QString icon)
{
    m_feedAddIcon = icon;
}

QString MainWindow::getFolderAddIcon()
{
    return m_folderAddIcon;
}

void MainWindow::setFolderAddIcon(QString icon)
{
    m_folderAddIcon = icon;
}

void MainWindow::displayFilterPressed(QTreeWidgetItem *item, int column)
{
    qDebug() << item->text(0);
    articleListProxy->setFilter(static_cast<DisplayFilters>(item->data(1, Qt::UserRole).toInt()));
}
