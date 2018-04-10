#include "articledisplay.h"

QWidget *ArticleDisplay::createTitleBar()
{
    QWidget *widget = new QWidget();
    QGridLayout *grid = new QGridLayout(widget);
    m_title = new QLabel("");
    m_date = new QLabel("");
    m_author = new QLabel("");

    connect(m_title, SIGNAL(linkActivated(QString)), this, SLOT(handleLinkPressed(QString)));
    m_title->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_title, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createTitleContextMenu(QPoint)));

    m_title->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_date->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_author->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //widget->setStyleSheet("background-color: white;");

    grid->addWidget(m_title, 0, 0, Qt::AlignLeft);
    grid->addWidget(m_date, 0, 1,  Qt::AlignRight);
    grid->addWidget(m_author, 1, 0, Qt::AlignLeft);

    widget->setLayout(grid);
    return widget;
}

void ArticleDisplay::createLayout()
{
    //setLayout(new QVBoxLayout);
    //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    createTabWidget();
    QWidget *articleWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(m_tabWidget);
    createWebView();
    createMediaDisplay();
    layout->addWidget(createTitleBar());
    layout->addWidget(m_view);
    layout->addWidget(m_media);
    articleWidget->setLayout(layout);
    articleWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidget(articleWidget);
    m_scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_scrollArea->setWidgetResizable(true);
    m_tabWidget->addTab(m_scrollArea, "Article");
    m_tabWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QVBoxLayout *base = new QVBoxLayout;
    base->addWidget(m_tabWidget);
    setLayout(base);
}

void ArticleDisplay::createWebView()
{
    m_view = new QWebEngineView(this);
    m_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

}

void ArticleDisplay::createTabWidget()
{
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_tabWidget->setTabsClosable(true);
    connect(m_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void ArticleDisplay::createMediaDisplay()
{
    m_media = new MediaDisplay(m_db, this);
}

void ArticleDisplay::getArticleData()
{
    QSqlQuery articleData = m_db->getArticle(m_articleID);
    if(!articleData.next()) {
        qDebug() << "Error retrieving article";
        return;
    }
    m_articleTitle = articleData.value("title").toString();
    m_articleAuthor = articleData.value("author").toString();
    m_articleDate = articleData.value("pubDate").toString();
    m_description = articleData.value("description").toString();
    m_url = articleData.value("link").toString();
}

void ArticleDisplay::createActions()
{
    m_openLink = new QAction(tr("Open link"));
    m_openLinkExternally = new QAction(tr("Open link in external browser"));

    connect(m_openLink, SIGNAL(triggered()), this, SLOT(openLink()));
    connect(m_openLinkExternally, SIGNAL(triggered()), this, SLOT(openLinkExternally()));
}

void ArticleDisplay::createMenu()
{
}

ArticleDisplay::ArticleDisplay(DatabaseManager *db, QWidget *parent) : QWidget(parent)
{
    m_db = db;
    createActions();
    createLayout();
}

void ArticleDisplay::setArticle(qint32 articleID)
{
    m_articleID = articleID;
    getArticleData();
    m_media->setArticleID(articleID);
    update();
}

void ArticleDisplay::update()
{
    m_title->setText("<a href=\""+m_url+"\">"+m_articleTitle+"</a>");
    m_date->setText(m_articleDate);
    m_author->setText(m_articleAuthor);
    m_view->setHtml(m_description);

}

void ArticleDisplay::handleLinkPressed(QString link)
{
    QWebEngineView *newView = new QWebEngineView;
    newView->setUrl(QUrl(link));
    int tab = m_tabWidget->addTab(newView, newView->title());
    connect(newView, &QWebEngineView::titleChanged, [=](QString newTitle){ m_tabWidget->setTabText(tab, newTitle); });
}

void ArticleDisplay::openLink()
{
    handleLinkPressed(m_url);
}

void ArticleDisplay::openLinkExternally()
{
    QDesktopServices::openUrl(m_url);
}

void ArticleDisplay::createTitleContextMenu(QPoint pos)
{
    QMenu *context = new QMenu(this);
    context->addAction(m_openLink);
    context->addAction(m_openLinkExternally);
    context->exec(mapToGlobal(pos));
}

void ArticleDisplay::closeTab(int index)
{
    if(index != 0) {
        m_tabWidget->widget(index)->deleteLater();
        m_tabWidget->removeTab(index);
    }
}

