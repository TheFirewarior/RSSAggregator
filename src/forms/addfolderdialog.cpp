#include "addfolderdialog.h"

AddFolderDialog::AddFolderDialog(DatabaseManager *db, QWidget *parent) : QWidget(parent), m_db(db), m_selected(0)
{
    formLayout = new QVBoxLayout(this);
    setLayout(formLayout);
    connect(this, SIGNAL(createFolder(QString,qint32)), m_db, SLOT(addFolder(QString,qint32)));
    setEnterData();
}

void AddFolderDialog::setEnterData()
{
    le_enterTitle = new QLineEdit;
    formLayout->addWidget(le_enterTitle);

    folderStruct = new QTreeWidget(this);
    folderStruct->setSelectionBehavior(QAbstractItemView::SelectItems);
    folderStruct->setSelectionMode(QAbstractItemView::SingleSelection);
    folderStruct->setColumnCount(1);
    QTreeWidgetItem *root = new QTreeWidgetItem;
    root->setText(0, "Root");
    root->setData(1, Qt::UserRole, 0);
    folderStruct->addTopLevelItem(root);
    if(m_selected == 0) root->setSelected(true);
    // add folders from database
    QSqlQuery folders = m_db->getFolderViewData();
    QList<Folder*> foldersStruct;
    while(folders.next()) {
        Folder *tmp = new Folder;
        tmp->id = folders.value("id").toInt();
        tmp->parent = folders.value("parent_id").toInt();
        tmp->title = folders.value("title").toString();
        foldersStruct.append(tmp);
    }
    QQueue<QTreeWidgetItem*> toProcess;
    toProcess.enqueue(root);
    while(!toProcess.isEmpty()) {
        QTreeWidgetItem *parent = toProcess.dequeue();
        qint32 parentID = parent->data(1, Qt::UserRole).toInt();
        foreach (Folder* folder, foldersStruct) {
            if(folder->parent == parentID) {
                QTreeWidgetItem *child = new QTreeWidgetItem;
                child->setText(0, folder->title);
                child->setData(1, Qt::UserRole, folder->id);
                parent->addChild(child);
                toProcess.enqueue(child);
                if(folder->id == m_selected) {
                    child->setSelected(true);
                }
            }
        }
    }
    folderStruct->expandAll();
    formLayout->addWidget(folderStruct);
    pb_finish = new QPushButton;
    pb_finish->setText(tr("Finish"));
    formLayout->addWidget(pb_finish);
    connect(pb_finish, SIGNAL(clicked()), this, SLOT(handleFinishPressed()));
}

void AddFolderDialog::setDefaultSelected(qint32 id)
{
    m_selected = id;
}

void AddFolderDialog::handleFinishPressed()
{
    if(le_enterTitle->text() == "") return;
    QList<QTreeWidgetItem*>items = folderStruct->selectedItems();
    qint32 selectedFolderID = 0;
    if(items.count() > 0) {
        selectedFolderID = items[0]->data(1, Qt::UserRole).toInt();
    }
    emit createFolder(le_enterTitle->text(), selectedFolderID);
    close();
}
