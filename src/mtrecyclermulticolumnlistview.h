#ifndef MT_RECYCLER_MULTI_COLUMN_LIST_VIEW_H_
#define MT_RECYCLER_MULTI_COLUMN_LIST_VIEW_H_

#include <QStringList>

#include <QWidget>
#include <QScrollBar>
#include <QPainter>
#include <QWheelEvent>

class MtRecyclerMultiColumnListViewAdapter;
class MultiColumnListHeader;

class MtRecyclerMultiColumnListView: public QWidget
{
    Q_OBJECT
public:
    explicit MtRecyclerMultiColumnListView(QWidget* parent);

    void setAdapter(MtRecyclerMultiColumnListViewAdapter* adapter);
    void buildListItems();
    void updateViewPort(bool updateAll);

    void scrollTo(int pos, Qt::Orientation ori);
    int fixScrollPos(int newScrollPos, Qt::Orientation ori);

    MultiColumnListHeader* getHeader();
    void setGrideVisible(bool visible);
    bool getGrideVisible();

signals:
    void sigUpdate();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

public slots:
    void onVScrollBarChanged(int val);
    void onHScrollBarChanged(int val);
    void layoutListItems();
    void notifyUpdate();

private slots:
    void update();

private:
    struct RowData
    {
        QList<QWidget*> widgets;
        int index = -1;
    };

    void calcScrollBarShowStatus(bool* vScrollBarShow, bool* hScrollBarShow);

    int calcVScrollBarMax();
    int calcVScrollbarPageStep();
    int calcHScrollBarMax(bool isVsbShow);
    int calcHScrollbarPageStep(bool isVsbShow);
    int calcIndexOfPoint(QPoint pt);

private:
    int m_contentHeight = 0; // 整个滚动区域逻辑长度

    MultiColumnListHeader* m_header = nullptr;
    MtRecyclerMultiColumnListViewAdapter* m_listAdapter = nullptr;

    int m_tailSpace = 0;

    int m_rowCount = 0;
    int m_colCount = 0;

    int m_rowHeight = 0;
    int m_rowWidth = 0;
    int m_headHeight = 0;

    QMargins m_ceilMargin = {1, 1, 1, 1};

    QList<RowData*> m_listCachedRows;

    QScrollBar* m_hsb = nullptr;
    QScrollBar* m_vsb = nullptr;
    int m_vscrollPos = 0;
    int m_hscrollPos = 0;

    // 是否显示网格线
    bool m_showGride = false;

    // 需要调用buildListItems的标记
    bool m_markAsDirty = false;

    // 当前鼠标放在m_listCachedRows[m_indexOfHover]的元素上
    int m_indexOfHover = -1;
    // 是否hover
    bool m_isHover = false;

    // 鼠标位置，之后m_isHover为true时该字段才有意义
    QPoint m_mousePoint;
};

#endif //MT_RECYCLER_MULTI_COLUMN_LIST_VIEW_H_
