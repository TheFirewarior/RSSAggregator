#ifndef ARTICLEFILTER_H
#define ARTICLEFILTER_H

#include <QObject>
#include <QSqlQuery>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "databasemanager.h"
#include "articlestruct.h"

class ArticleFilter : public QObject
{
    Q_OBJECT
    DatabaseManager *m_db;
    qint32 m_currentID;
    QStringList m_globalKeywords;
    QStringList m_titleKeywords;
    QStringList m_descriptionKeywords;
    QStringList m_categoryKeywords;
public:
    explicit ArticleFilter(DatabaseManager *db,QObject *parent = nullptr);

signals:
    void addArticle(ArticleItem item);
public slots:
    void validateArticle(ArticleItem item);
private:
    bool validateTitle(QString title);
    bool validateDescription(QString description);
    bool validateCategory(QString category);
    bool validateGlobal(QString title, QString description, QString category);
private slots:
    void getFilters();
};

#endif // ARTICLEFILTER_H
