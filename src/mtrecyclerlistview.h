#ifndef MT_RECYCLER_LIST_VIEW_H
#define MT_RECYCLER_LIST_VIEW_H

#include <QWidget>
#include <QFrame>
#include <QScrollBar>

class MtRecyclerListViewAdapter;

class MtRecyclerListView: public QWidget
{
    Q_OBJECT
public:
    explicit MtRecyclerListView(QWidget* parent);
    ~MtRecyclerListView();

    /*
     * 设置列表的adapter
     */
    void setAdapter(MtRecyclerListViewAdapter* adapter);

    /*
     * 更新某个元素
     */
    void updateItem(int index);

    /*
     * 更新整个列表。
     */
    void updateList();

    /*
     * 返回item的索引号，注意：如果item不在列表里或者不在可视范围内，该函数将返回-1
     */
    int getItemIndex(QWidget* item);

    /*
     * 将索引为index的item滚动到可见区域
     */
    void makeItemVisible(int index);

    /*
     * 滚动到某个位置，pos >= 0，如果pos值不正确，该函数会修正pos值
     */
    void scrollTo(int pos);

    /*
     * 元素是否多到需要滚动
     */
    bool isBigEnoughForScroll();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

protected:
    virtual void buildListItems(int defaultScrollPos = 0);
    void layoutListItems();
    void scrollToImpl(int pos);

    int getListGeometryLength();

    int fixScrollPos(int newScrollPos);
    void updateViewPort(bool updateAll);
    void onScrollBarChanged(int val);

    struct ItemData
    {
        QWidget* widget = nullptr;
        int index = -1;
    };

protected:
    Qt::Orientation m_layoutOrient = Qt::Orientation::Horizontal;

    QList<ItemData*> m_listCachedItems;

    int m_scrollPos = 0;

    int m_layoutLeadSpace = 0;
    int m_layoutTailSpace = 0;
    int m_contentLength = 0; // 整个滚动区域逻辑长度
    int m_itemCount = 0;
    int m_itemLen = 0;
    int m_itemSpace = 0;

    MtRecyclerListViewAdapter* m_listAdapter = nullptr;
    QScrollBar* m_sb = nullptr;
};

#endif //MTRECYCLER_VIEW_H

