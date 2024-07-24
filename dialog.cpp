#include "dialog.h"
#include "ui_dialog.h"
#include "src/mtrecyclerlistview.h"
#include "listitem.h"

#include <QHBoxLayout>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);

    MtRecyclerListView* listView = new MtRecyclerListView(this);
    layout->addWidget(listView);
    layout->setMargin(30);

    Adapter* adapter = new Adapter(this);
    listView->setAdapter(adapter);
}

Dialog::~Dialog()
{
    delete ui;
}

Adapter::Adapter(QObject *parent):QObject(parent)
{
    for (size_t i = 0; i < 1000000; i++)
    {
        m_items.push_back("Dog!!");
    }
}

QWidget *Adapter::createListItem(int index, MtRecyclerListView *parent) {
    ListItem* item = new ListItem(parent);
    item->setParam(index, m_items[index]);
    return item;
}

void Adapter::updateItem(QWidget *item, int index) {
    ListItem* itemWidget =(ListItem*)item;
    itemWidget->setParam(index, m_items[index]);
}
