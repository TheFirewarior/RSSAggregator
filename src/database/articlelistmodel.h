#ifndef ARTICLELISTMODEL_H
#define ARTICLELISTMODEL_H
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMap>
#include <QDateTime>
#include <QColor>
#include <QVariant>

#include "databasemanager.h"
#define DATE_COLUMN 10

class ArticleListModel : public QSqlTableModel
{
    Q_OBJECT
    //Q_PROPERTY(QVariant readItemBackground READ getReadItemBackground WRITE setReadItemBackground DESIGNABLE true)
    DatabaseManager *m_db;
    QString m_id;
    QList<qint32> queryToIDs(QSqlQuery query);
    QList<qint32> getChildFeedsID(qint32 id);
    void changeHeaderNames();
    QColor m_readColor;
public:
    ArticleListModel(DatabaseManager *db, QObject *parent = 0);
    QVariant data(const QModelIndex &idx, int role) const;
public slots:
    void setNewID(QString id);
    void setReadColor(QString color);
    void handleArticleChanged(qint32 articleID);
protected slots:
};

#endif // ARTICLELISTMODEL_H
