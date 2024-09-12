#include "dialog.h"
#include "ui_dialog.h"
#include "src/mtrecyclerlistview.h"
#include "src/multicolumnlistheader.h"
#include "listitem.h"

#include <QHBoxLayout>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
#if 0
    MtRecyclerListView* listView = new MtRecyclerListView(this);
    layout->addWidget(listView);
    layout->setMargin(30);

    Adapter* adapter = new Adapter(this);
    listView->setAdapter(adapter);
#endif
    MtRecyclerMultiColumnListView* multiListView = new MtRecyclerMultiColumnListView(this);
    multiListView->setGrideVisible(true);
    MultiColumnListHeader* header = multiListView->getHeader();

    layout->addWidget(multiListView);
    layout->setMargin(30);

    MultiColumnListAdapter* adapter = new MultiColumnListAdapter();
    multiListView->setAdapter(adapter);

    header->setColumnWidth(0, 230);
    header->setColumnWidth(1, 92);
    header->setColumnWidth(2, 134);
    header->setColumnWidth(3, 120);
    header->setColumnWidth(4, 400);
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

int MultiColumnListAdapter::GetHeaderHeight() {
    return 36;
}

QStringList MultiColumnListAdapter::getHeader()
{
    return QStringList{W2Q(L"文件名"),W2Q(L"大小"), W2Q(L"创建日期"), W2Q(L"修改日期"), W2Q(L"路径")};
}

QWidget *MultiColumnListAdapter::createCeil(int row, int column, QWidget *parent)
{
    if (column == 0)
    {
        QWidget* widget = new QWidget(parent);
        QHBoxLayout* layout = new QHBoxLayout(widget);
        widget->setLayout(layout);
        layout->setSpacing(4);
        layout->setMargin(0);

        QCheckBox* checkBox = new QCheckBox(widget);
        layout->addWidget(checkBox);

        QLabel* label = new QLabel(widget);
        label->setObjectName("name");
        QFont font("Microsoft YaHei");
        font.setPixelSize(14);
        label->setFont(font);
        label->setText(W2Q(L"海边骑摩托.jpeg"));
        label->setStyleSheet("QLabel{ padding-bottom:2px; }");

        layout->addWidget(label);
        layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));

        return widget;
    }
    else
    {
        QLabel* lab = new QLabel(parent);
        lab->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
        char buf[64] = {0};
        sprintf_s(buf, 64, "column:%d,row:%d", column, row);
        lab->setText(QString(buf));
        return lab;
    }
}

int MultiColumnListAdapter::getRowHeight()
{
    return 32;
}

int MultiColumnListAdapter::getRowCount()
{
    return 3000000;
}

int MultiColumnListAdapter::getColumnCount()
{
    return 5;
}

void MultiColumnListAdapter::updateCeil(QWidget *item, int row, int col)
{
    QLabel* label = (QLabel*)item;
    if (col == 0)
    {
        label = item->findChild<QLabel*>("name");
        label->setText(W2Q(L"海边骑摩托.jpeg"));
    }
    else
    {
        char buf[64];
        sprintf_s(buf, 64, "column:%d,row:%d", col, row);

        label->setText(buf);
    }
}
