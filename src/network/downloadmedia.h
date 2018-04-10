#ifndef DOWNLOADMEDIA_H
#define DOWNLOADMEDIA_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

class DownloadMedia : public QObject
{
    Q_OBJECT
    QNetworkAccessManager *m_manager;
    QByteArray m_downloadedData;
public:
    explicit DownloadMedia(QUrl url, QObject *parent = nullptr);
    QByteArray getDownloadedData();
signals:
    void downloaded();
public slots:
    void finishedDownloading(QNetworkReply *reply);
};

#endif // DOWNLOADMEDIA_H
