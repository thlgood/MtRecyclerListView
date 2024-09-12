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
     * ͷ���ĸ߶ȣ�����߶�Ϊ0���ʾ��չʾͷ��
     */
    virtual int GetHeaderHeight() = 0;

    /**
     * ��ȡͷ���ֶΣ�Ԫ�ظ����������getColumnCount���������չʾͷ������GetHeaderHeight����0
     */
    virtual QStringList getHeader() = 0;

    /**
     * ������������
     */
    virtual QWidget* createCeil(int row, int column, QWidget* parent) = 0;

    /**
     * ������
     */
    virtual void createRow(int row, QList<QWidget*>& columns);

    /**
     * �����б�Ԫ�صĸ�
     */
    virtual int getRowHeight() = 0;

    /**
     * �б�ײ�������
     */
    virtual int bottomSpace();

    /**
     * ��ȡ�б�Ԫ�ص�����
     */
    virtual int getRowCount() = 0;

    /**
     * ��ȡ�б�Ԫ�ص�����
     */
    virtual int getColumnCount() = 0;

    /**
     * ����ĳ��Ԫ��
     */
    virtual void updateCeil(QWidget* item, int row, int col) = 0;

    /**
     * ����һ����
     */
    virtual void updateRow(int row, QList<QWidget*>& columns, int columnCount);

    /**
     * scrollBarWidth ���������
     */
    virtual int scrollBarWidth();

    /**
     * ÿ��Ԫ�ص�margin
     */
    virtual QMargins getCeilMargins();

    /**
     * ͷ��ÿ��Ԫ�ص�margin��Ŀǰֻ��left��Ч
     */
    virtual QMargins getHeaderMargins();

    /**
     * scrollBar �������Ŀռ�
     */
    virtual QMargins scrollBarMargin();

    /**
     * �����б�
     */
    void setListView(MtRecyclerMultiColumnListView* lv);

protected:
    MtRecyclerMultiColumnListView* m_listView = nullptr;
    friend class MtRecyclerMultiColumnListView;
};

#endif //MT_RECYCLER_MULTI_COLUMN_LIST_VIEW_ADAPTER_H_
