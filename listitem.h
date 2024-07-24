#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class ListItem;
}

class ListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListItem(QWidget *parent = nullptr);
    ~ListItem();

    void setParam(int index, QString name);

private:
    void paintEvent(QPaintEvent* evt) override;

private slots:
    void onBtnClicked();

private:
    Ui::ListItem *ui;

    int m_index = -1;
};

#endif // LISTITEM_H
