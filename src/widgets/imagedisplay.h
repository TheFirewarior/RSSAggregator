#ifndef IMAGEDISPLAY_H
#define IMAGEDISPLAY_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QStringList>
#include <QList>
#include <QLabel>

#include "downloadmedia.h"

class ImageDisplay : public QWidget
{
    Q_OBJECT
    QLabel *m_image;
    QPushButton *m_previous;
    QPushButton *m_next;
    QList<QByteArray> m_images;
    qint32 m_index;
    void initLayout();
public:
    explicit ImageDisplay(QWidget *parent = nullptr);
    void addImage(QString url);

signals:
    void imageChanged();
public slots:
    void next();
    void previous();
    void setImage(qint32 index);
};

#endif // IMAGEDISPLAY_H
