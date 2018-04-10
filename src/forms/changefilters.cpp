#include "changefilters.h"

ChangeFilters::ChangeFilters(QStringList keywords, QWidget *parent) : QWidget(parent)
{
    setLayout(new QVBoxLayout);

    m_keywordsModel = new FiltersModel(keywords, this);
    m_keywordsDisplay = new QListView(this);
    m_keywordsDisplay->setModel(m_keywordsModel);


    m_listControl = new QHBoxLayout;
    pb_addRow = new QPushButton(tr("Add row"));
    connect(pb_addRow, SIGNAL(clicked()), m_keywordsModel, SLOT(addRow()));
    pb_delRow = new QPushButton(tr("Delete row"));
    connect(pb_delRow, SIGNAL(clicked()), this, SLOT(handleDeleteRow()));

    m_listControl->addWidget(pb_addRow);
    m_listControl->addWidget(pb_delRow);
    QWidget *container = new QWidget;
    container->setLayout(m_listControl);

    pb_ok = new QPushButton(tr("Ok"));
    connect(pb_ok, SIGNAL(clicked()), this, SLOT(editingFinished()));
    pb_cancel = new QPushButton(tr("Cancel"));
    connect(pb_cancel, SIGNAL(clicked()), this, SLOT(close()));
    layout()->addWidget(m_keywordsDisplay);
    layout()->addWidget(container);
    layout()->addWidget(pb_ok);
    layout()->addWidget(pb_cancel);
}

void ChangeFilters::editingFinished()
{
    emit updateKeywords(m_keywordsModel->getUpdatedKeywords());
    close();
}

void ChangeFilters::handleDeleteRow()
{
    qint32 item = m_keywordsDisplay->selectionModel()->selectedRows().takeFirst().row();
    m_keywordsModel->delRow(item);
}
