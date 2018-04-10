#ifndef CHANGEFILTERS_H
#define CHANGEFILTERS_H

#include <QWidget>
#include <QListView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>


#include "filtersmodel.h"

class ChangeFilters : public QWidget
{
    Q_OBJECT
    QStringList m_keywords;
    QListView *m_keywordsDisplay;
    FiltersModel *m_keywordsModel;

    QHBoxLayout *m_listControl;
    QPushButton *pb_addRow;
    QPushButton *pb_delRow;

    QPushButton *pb_ok;
    QPushButton *pb_cancel;
public:
    explicit ChangeFilters(QStringList keywords, QWidget *parent = nullptr);

signals:
    void updateKeywords(QStringList newKeywords);
private slots:
    void editingFinished();
    void handleDeleteRow();
};

#endif // CHANGEFILTERS_H
