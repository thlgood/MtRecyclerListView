#ifndef MT_RECYCLER_LIST_VIEW_ADAPTER_H
#define MT_RECYCLER_LIST_VIEW_ADAPTER_H

#include <QSize>
#include <QWidget>

class MtRecyclerListView;

class MtRecyclerListViewAdapter
{
public:
    virtual ~MtRecyclerListViewAdapter() = default;

    /**
     * 水平方向还是垂直方向的列表
     */
    virtual Qt::Orientation getOrientation() = 0;

    /**
     * 返回列表元素的宽与高，注意：这里的宽高不包含元素间的间距
     */
    virtual QSize getItemSize() = 0;

    /**
     * 元素间的间距
     */
    virtual int getItemSpace() = 0;

    /**
     * 列表内left/top/right/bottom的留白
     */
    virtual QMargins margin() = 0;

    /**
     * 获取列表元素的个数
     */
    virtual int getCount() = 0;

    /**
     * 创建列表的单个元素，注意：每个元素都需要使用传入的parent作为parent
     */
    virtual QWidget* createListItem(int index, MtRecyclerListView* parent) = 0;

    /**
     * 更新某个元素
     */
    virtual void updateItem(QWidget* item, int index) = 0;

    /**
     * scrollBarWidth 滚动条宽度
     */
    virtual int scrollBarWidth()
    {
        return 20;
    }

    /**
     * scrollBar 四周留的空间
     */
    virtual QMargins scrollBarMargin()
    {
        return QMargins(1, 1, 1, 1);
    }

    void setListView(MtRecyclerListView* lv)
    {
        m_listView = lv;
    }

protected:
    MtRecyclerListView* m_listView = nullptr;
    friend class MtRecyclerListView;
};


#endif // MT_RECYCLER_LIST_VIEW_ADAPTER_H
