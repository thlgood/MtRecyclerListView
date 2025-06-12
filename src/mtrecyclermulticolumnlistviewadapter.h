#ifndef MT_RECYCLER_MULTI_COLUMN_LIST_VIEW_ADAPTER_H_
#define MT_RECYCLER_MULTI_COLUMN_LIST_VIEW_ADAPTER_H_

#include <QWidget>
#include <QMargins>
#include <QStringList>

class MtRecyclerMultiColumnListView;

class MtRecyclerMultiColumnListViewAdapter
{
public:
    /**
     * 头部的高度，如果高度为0则表示不展示头部
     */
    virtual int GetHeaderHeight() = 0;

    /**
     * 获取头部字段，元素个数必须等于getColumnCount，如果不想展示头部，让GetHeaderHeight返回0
     */
    virtual QStringList getHeader() = 0;

    /**
     * 创建单个格子
     */
    virtual QWidget* createCeil(int row, int column, QWidget* parent) = 0;

    /**
     * 创建行
     */
    virtual void createRow(int row, QList<QWidget*>& columns);

    /**
     * 返回列表元素的高
     */
    virtual int getRowHeight() = 0;

    /**
     * 列表底部的留白
     */
    virtual int bottomSpace();

    /**
     * 获取列表元素的行数
     */
    virtual int getRowCount() = 0;

    /**
     * 获取列表元素的列树
     */
    virtual int getColumnCount() = 0;

    /**
     * 更新某个元素
     */
    virtual void updateCeil(QWidget* item, int row, int col) = 0;

    /**
     * 更行一整行
     */
    virtual void updateRow(int row, QList<QWidget*>& columns, int columnCount);

    /**
     * scrollBarWidth 滚动条宽度
     */
    virtual int scrollBarWidth();

    /**
     * 每个元素的margin
     */
    virtual QMargins getCeilMargins();

    /**
     * 头部每个元素的margin，目前只有left生效
     */
    virtual QMargins getHeaderMargins();

    /**
     * scrollBar 四周留的空间
     */
    virtual QMargins scrollBarMargin();

    /**
     * 设置列表
     */
    void setListView(MtRecyclerMultiColumnListView* lv);

protected:
    MtRecyclerMultiColumnListView* m_listView = nullptr;
    friend class MtRecyclerMultiColumnListView;
};

#endif //MT_RECYCLER_MULTI_COLUMN_LIST_VIEW_ADAPTER_H_
