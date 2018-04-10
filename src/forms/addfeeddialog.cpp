#include "addfeeddialog.h"

AddFeedDialog::AddFeedDialog(DatabaseManager *db, QWidget *parent) : QWidget(parent)
{
    m_db = db;
    m_selected = 0;
    connect(this, SIGNAL(createFeed(QString,QString,qint32)), m_db, SLOT(addFeed(QString,QString,qint32)));
    formLayout = new QVBoxLayout(this);
    setLayout(formLayout);
    setEnterUrlDialog();
}

void AddFeedDialog::setEnterUrlDialog()
{
    lb_enterUrl = new QLabel();
    lb_enterUrl->setText("Enter url: ");

    le_enterUrl = new QLineEdit();

    pb_submitUrl = new QPushButton();
    pb_submitUrl->setText(tr("Next"));
    connect(pb_submitUrl, SIGNAL(clicked()), this, SLOT(validateUrl()));
    formLayout->addWidget(lb_enterUrl);
    formLayout->addWidget(le_enterUrl);
    formLayout->addWidget(pb_submitUrl);

}

void AddFeedDialog::setEnterFeedData()
{
    clearEnterUrl();
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

void AddFeedDialog::clearEnterUrl()
{
    formLayout->removeWidget(lb_enterUrl);
    formLayout->removeWidget(le_enterUrl);
    formLayout->removeWidget(pb_submitUrl);

    delete lb_enterUrl;
    delete le_enterUrl;
    delete pb_submitUrl;
}

void AddFeedDialog::validateUrl()
{
    QHostInfo::lookupHost(QUrl(le_enterUrl->text()).host(), this, SLOT(checkUrlResult(QHostInfo)));
}

void AddFeedDialog::checkUrlResult(const QHostInfo &info)
{
    if(info.error() == QHostInfo::NoError) {
        qDebug() << "Succes!";
        m_url = le_enterUrl->text();
        setEnterFeedData();
    }
    else {
        qDebug() << "Failure: " << info.errorString();
    }
}

void AddFeedDialog::handleFinishPressed()
{
    if(le_enterTitle->text() == "") return;
    QList<QTreeWidgetItem*>items = folderStruct->selectedItems();
    qint32 selectedFolderID = 0;
    if(items.count() > 0) {
        selectedFolderID = items[0]->data(1, Qt::UserRole).toInt();
    }
    emit createFeed(le_enterTitle->text(), m_url, selectedFolderID);
    close();
}

void AddFeedDialog::setDefaultSelected(qint32 id)
{
    m_selected = id;
}
