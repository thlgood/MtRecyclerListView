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
     * ˮƽ�����Ǵ�ֱ������б�
     */
    virtual Qt::Orientation getOrientation() = 0;

    /**
     * �����б�Ԫ�صĿ���ߣ�ע�⣺����Ŀ�߲�����Ԫ�ؼ�ļ��
     */
    virtual QSize getItemSize() = 0;

    /**
     * Ԫ�ؼ�ļ��
     */
    virtual int getItemSpace() = 0;

    /**
     * �б���left/top/right/bottom������
     */
    virtual QMargins margin() = 0;

    /**
     * ��ȡ�б�Ԫ�صĸ���
     */
    virtual int getCount() = 0;

    /**
     * �����б�ĵ���Ԫ�أ�ע�⣺ÿ��Ԫ�ض���Ҫʹ�ô����parent��Ϊparent
     */
    virtual QWidget* createListItem(int index, MtRecyclerListView* parent) = 0;

    /**
     * ����ĳ��Ԫ��
     */
    virtual void updateItem(QWidget* item, int index) = 0;

    /**
     * scrollBarWidth ���������
     */
    virtual int scrollBarWidth()
    {
        return 20;
    }

    /**
     * scrollBar �������Ŀռ�
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
