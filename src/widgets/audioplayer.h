#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QWidget>
#include <QStringList>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QUrl>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMediaContent>

class AudioPlayer : public QWidget
{
    Q_OBJECT
    //QStringList m_sources;
    QMediaPlaylist *m_playlist;
    QMediaPlayer *m_player;
    QPushButton *m_play;
    QPushButton *m_next;
    QPushButton *m_previous;
    bool m_playing;
public:
    explicit AudioPlayer(QWidget *parent = nullptr);
    void addAudio(QString url);

signals:

public slots:
    void play();
};

#endif // AUDIOPLAYER_H
