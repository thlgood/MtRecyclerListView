#include "mtrecyclermulticolumnlistviewadapter.h"

void MtRecyclerMultiColumnListViewAdapter::createRow(int row, QList<QWidget *> &columns)
{
    for (int c = 0; c < getColumnCount(); c++)
    {
        columns.append(createCeil(row, c, (QWidget*)m_listView));
    }
}

int MtRecyclerMultiColumnListViewAdapter::bottomSpace() {
    return 40;
}

void MtRecyclerMultiColumnListViewAdapter::updateRow(int row, QList<QWidget *> &columns, int columnCount)
{
    if (columns.size() > columnCount)
    {
        while (columns.size() > columnCount)
        {
            columns.back()->deleteLater();
            columns.pop_back();
        }
    }

    for (int c = 0; c < columnCount; c++)
    {
        if (c >= columns.size())
        {
            columns.append(createCeil(row, c, (QWidget*)m_listView));
        }
        else
        {
            updateCeil(columns[c], row, c);
        }
    }
}

int MtRecyclerMultiColumnListViewAdapter::scrollBarWidth()
{
    return 20;
}

QMargins MtRecyclerMultiColumnListViewAdapter::getCeilMargins()
{
    return QMargins(12, 4, 12, 4);
}

QMargins MtRecyclerMultiColumnListViewAdapter::getHeaderMargins()
{
    return QMargins(24, 0, 0, 0);
}

QMargins MtRecyclerMultiColumnListViewAdapter::scrollBarMargin()
{
    return QMargins(1, 1, 1, 1);
}

void MtRecyclerMultiColumnListViewAdapter::setListView(MtRecyclerMultiColumnListView *lv)
{
    m_listView = lv;
}
