#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
}

void DownloadManager::init()
{
    if(QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "init");
        return;
    }
    m_networkManager = new QNetworkAccessManager(this);
    m_updateCheckInterval = new QTimer(this);
    m_updateCheckInterval->setInterval(1 * MIN);
    connect(m_updateCheckInterval, SIGNAL(timeout()), this, SLOT(checkForUpdates()));
    m_updateCheckInterval->start();
    m_db = new DatabaseManager("DifferentThread", this);
    checkForUpdates();
}

void DownloadManager::checkForUpdates()
{
    qDebug() << "Update check";
    QSqlQuery feedsToUpdate = m_db->getFeedsToUpdate();
    while(feedsToUpdate.next()) {
        DownloadFeed *download = new DownloadFeed(feedsToUpdate.value("id").toInt(), m_db, this);
        QNetworkRequest request;
        request.setUrl(QUrl(feedsToUpdate.value("url").toString()));
        request.setRawHeader("User-Agent", "RSSAggregator");
        QNetworkReply *reply = m_networkManager->get(request);
        download->setReply(reply);
    }
}
