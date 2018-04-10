#include "listselection.h"

ListSelection::ListSelection(QWidget *parent) : QWidget(parent)
{
    createLayout();
}

void ListSelection::setOptionsList(const QStringList &options)
{
    m_optionsWidget->clear();
    foreach(QString option, options) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(option);
        m_optionsWidget->addItem(item);
    }
}

void ListSelection::setCurrentOption(QString option)
{
    m_currentValue->setText(option);
}

void ListSelection::createLayout()
{
    setLayout(new QVBoxLayout);
    QHBoxLayout *current = new QHBoxLayout;
    QLabel *currentLabel = new QLabel(tr("Current"));
    current->addWidget(currentLabel);
    m_currentValue = new QLabel();
    current->addWidget(m_currentValue);

    QWidget *currentWidget = new QWidget;
    currentWidget->setLayout(current);
    layout()->addWidget(currentWidget);

    m_optionsWidget = new QListWidget;
    layout()->addWidget(m_optionsWidget);
    connect(m_optionsWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectionChanged(QModelIndex)));

}

void ListSelection::selectionChanged(const QModelIndex &index)
{
    m_currentValue->setText(index.data().toString());
    emit optionChanged(m_currentValue->text());
}
