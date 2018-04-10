#include "audioplayer.h"

AudioPlayer::AudioPlayer(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QHBoxLayout *layout = new QHBoxLayout();
    m_playlist = new QMediaPlaylist;
    m_player = new QMediaPlayer(this);
    m_player->setPlaylist(m_playlist);
    m_playing = false;

    m_play = new QPushButton("play");
    connect(m_play, SIGNAL(clicked()), this, SLOT(play()));
    m_next = new QPushButton("next");
    connect(m_next, SIGNAL(clicked()), m_playlist, SLOT(next()));
    m_previous = new QPushButton("previous");
    connect(m_previous, SIGNAL(clicked()), m_playlist, SLOT(previous()));
    layout->addWidget(m_play);

    layout->insertSpacing(1, 3);
    layout->addWidget(m_previous);
    layout->addWidget(m_next);

    setLayout(layout);
}

void AudioPlayer::addAudio(QString url)
{
    m_playlist->addMedia(QUrl(url));
}

void AudioPlayer::play()
{
    if(m_playing) {
        m_player->stop();
    }
    else m_player->play();
    m_playing = !m_playing;
}
