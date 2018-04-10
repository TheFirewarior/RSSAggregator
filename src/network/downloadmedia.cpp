#include "downloadmedia.h"
#include <QDebug>
DownloadMedia::DownloadMedia(QUrl url, QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedDownloading(QNetworkReply*)));
    QNetworkRequest request(url);
    m_manager->get(request);
}

QByteArray DownloadMedia::getDownloadedData()
{
    return m_downloadedData;
}

void DownloadMedia::finishedDownloading(QNetworkReply *reply)
{
    m_downloadedData = reply->readAll();
    reply->deleteLater();
    emit downloaded();
}
