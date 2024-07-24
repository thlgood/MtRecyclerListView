#include "listitem.h"
#include "ui_listitem.h"

#include <QPainter>

ListItem::ListItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListItem)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &ListItem::onBtnClicked);
}

ListItem::~ListItem()
{
    delete ui;
}

void ListItem::setParam(int index, QString name)
{
    QString str = QString::number(index);
    ui->label_3->setText(str);
    ui->pushButton->setText(name);
    m_index = index;
}

void ListItem::paintEvent(QPaintEvent *evt)
{
#if 1
    QPainter painter(this);
    QPen pen(Qt::SolidLine);
    pen.setColor(QColor::fromRgb(0xC8, 0xC8, 0xC8));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(0, 0, width()-1, height()-1);
#endif
    QWidget::paintEvent(evt);
}

void ListItem::onBtnClicked()
{
    QString str = QString("you clicked %1 button").arg(m_index);
    QMessageBox box;
    box.setText(str);
    box.exec();
}
