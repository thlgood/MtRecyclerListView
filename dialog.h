#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QList>
#include "src/mtrecyclerlistviewadapter.h"
#include "src/mtrecyclerlistview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE


class Adapter: public QObject, public MtRecyclerListViewAdapter
{
    Q_OBJECT
public:
    Adapter(QObject* parent);

    Qt::Orientation getOrientation() override {
        return Qt::Orientation::Vertical;
    }

    virtual QSize getItemSize() override {
        QMargins m = margin();
        return QSize(m_listView->width() - m.left() - m.right(), 111);
    }

    virtual int getItemSpace() override {
        return 4;
    }

    virtual int getCount() override {
        return m_items.size();
    }

    virtual QMargins margin() override {
        return QMargins(10, 10, 30, 10);
    }

    QWidget* createListItem(int index, MtRecyclerListView* parent) override;

    void updateItem(QWidget* item, int index) override;

private:
    QList<QString> m_items;
};


class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
