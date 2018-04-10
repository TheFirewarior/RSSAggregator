#include "imagedisplay.h"

void ImageDisplay::initLayout()
{
    m_image = new QLabel;

    m_previous = new QPushButton(tr("Previous"));
    m_next = new QPushButton(tr("Next"));

    connect(m_previous, SIGNAL(clicked()), this, SLOT(previous()));
    connect(m_next, SIGNAL(clicked()), this, SLOT(next()));

    QGridLayout *displayLayout = new QGridLayout;
    displayLayout->addWidget(m_image, 0, 0, 1, 2);
    displayLayout->addWidget(m_previous, 1, 0);
    displayLayout->addWidget(m_next, 1, 1);

    setLayout(displayLayout);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

ImageDisplay::ImageDisplay(QWidget *parent) : QWidget(parent)
{
    m_index = 0;
    initLayout();
    connect(this, &ImageDisplay::imageChanged , [=]{setImage(m_index); });
}

void ImageDisplay::addImage(QString url)
{
    DownloadMedia *download = new DownloadMedia(url, this);
    connect(download, &DownloadMedia::downloaded, [=]{ \
        m_images.append(download->getDownloadedData());
        emit imageChanged();
    });
}

void ImageDisplay::next()
{
    m_index = (m_index + 1) % m_images.size();
    emit imageChanged();
}

void ImageDisplay::previous()
{
    --m_index;
    m_index = m_index >= 0 ? m_index : m_images.size() - 1;
    emit imageChanged();
}

void ImageDisplay::setImage(qint32 index)
{
    QPixmap img;
    img.loadFromData(m_images[index]);
    m_image->setPixmap(img);
}
