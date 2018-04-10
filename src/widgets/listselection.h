#ifndef LISTSELECTION_H
#define LISTSELECTION_H

#include <QWidget>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>

class ListSelection : public QWidget
{
    Q_OBJECT
    QStringList m_options;
    QLabel *m_currentValue;
    QListWidget *m_optionsWidget;
public:
    explicit ListSelection(QWidget *parent = nullptr);

signals:
    void optionChanged(const QString& option);
public slots:
    void setOptionsList(const QStringList& options);
    void setCurrentOption(QString option);
    void createLayout();
private slots:
    void selectionChanged(const QModelIndex& index);
};

#endif // LISTSELECTION_H
