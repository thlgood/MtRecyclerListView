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
     * �����б��adapter
     */
    void setAdapter(MtRecyclerListViewAdapter* adapter);

    /*
     * ����ĳ��Ԫ��
     */
    void updateItem(int index);

    /*
     * ���������б�
     */
    void updateList();

    /*
     * ����item�������ţ�ע�⣺���item�����б�����߲��ڿ��ӷ�Χ�ڣ��ú���������-1
     */
    int getItemIndex(QWidget* item);

    /*
     * ������Ϊindex��item�������ɼ�����
     */
    void makeItemVisible(int index);

    /*
     * ������ĳ��λ�ã�pos >= 0�����posֵ����ȷ���ú���������posֵ
     */
    void scrollTo(int pos);

    /*
     * Ԫ���Ƿ�ൽ��Ҫ����
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
    int m_contentLength = 0; // �������������߼�����
    int m_itemCount = 0;
    int m_itemLen = 0;
    int m_itemSpace = 0;

    MtRecyclerListViewAdapter* m_listAdapter = nullptr;
    QScrollBar* m_sb = nullptr;
};

#endif //MTRECYCLER_VIEW_H

