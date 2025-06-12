#include "mtrecyclermulticolumnlistview.h"
#include "mtrecyclermulticolumnlistviewadapter.h"
#include "multicolumnlistheader.h"

MtRecyclerMultiColumnListView::MtRecyclerMultiColumnListView(QWidget *parent):QWidget(parent)
{
    m_vsb = new QScrollBar(Qt::Orientation::Vertical, this);
    m_hsb = new QScrollBar(Qt::Orientation::Horizontal, this);

    connect(m_vsb, &QScrollBar::valueChanged, this, &MtRecyclerMultiColumnListView::onVScrollBarChanged);
    connect(m_hsb, &QScrollBar::valueChanged, this, &MtRecyclerMultiColumnListView::onHScrollBarChanged);

    m_header = new MultiColumnListHeader(this);
    connect(m_header, &MultiColumnListHeader::onWidthChanged, this, &MtRecyclerMultiColumnListView::notifyUpdate);

    connect(this, &MtRecyclerMultiColumnListView::sigUpdate, this, &MtRecyclerMultiColumnListView::update, Qt::QueuedConnection);

    setAttribute(Qt::WA_MouseTracking);
}

void MtRecyclerMultiColumnListView::setAdapter(MtRecyclerMultiColumnListViewAdapter *adapter)
{
    if (adapter == m_listAdapter)
        return;

    if (m_listAdapter || !adapter)
    {
        return;
    }

    m_vscrollPos = 0;
    m_hscrollPos = 0;

    m_listAdapter = adapter;
    m_listAdapter->setListView(this);

    QStringList headerItems = m_listAdapter->getHeader();
    m_header->SetHeaderText(headerItems);
    m_header->setHeaderHeight(m_listAdapter->GetHeaderHeight());

    buildListItems();
}

void MtRecyclerMultiColumnListView::buildListItems()
{
    if (!m_listAdapter)
        return;

    m_rowCount = m_listAdapter->getRowCount();
    m_rowHeight = m_listAdapter->getRowHeight();

    m_colCount = m_listAdapter->getColumnCount();
    m_headHeight = m_listAdapter->GetHeaderHeight();

    m_tailSpace = m_listAdapter->bottomSpace();

    m_rowWidth = m_header->getTotalWidth();

    m_ceilMargin = m_listAdapter->getCeilMargins();
    m_ceilMargin.setLeft(std::max(1, m_ceilMargin.left()));
    m_ceilMargin.setTop(std::max(1, m_ceilMargin.top()));
    m_ceilMargin.setRight(std::max(1, m_ceilMargin.right()));
    m_ceilMargin.setBottom(std::max(1, m_ceilMargin.bottom()));

    m_header->setHeadMargin(m_listAdapter->getHeaderMargins());

    if (m_rowCount <= 0 || m_rowHeight <= 0)
    {
        m_contentHeight = 0;
    }
    else
    {
        m_contentHeight = m_rowHeight * m_rowCount + m_tailSpace;
    }

    m_header->setVisible(m_colCount > 0 && m_rowWidth > 0 && m_headHeight > 0);

    if (m_rowCount <= 0 || m_colCount <= 0 || m_rowHeight <= 0 || m_rowWidth <= 0 || m_contentHeight <= 0)
    {
        m_vsb->setVisible(false);
        m_hsb->setVisible(false);

        for (int i = 0; i < m_listCachedRows.size(); i++) {
            for (int j = 0; j < m_listCachedRows[i]->widgets.size(); j++) {
                m_listCachedRows[i]->widgets[j]->deleteLater();
            }
            delete m_listCachedRows[i];
        }
        m_listCachedRows.clear();
        return;
    }

    bool vsbShow = false;
    bool hsbShow = false;
    calcScrollBarShowStatus(&vsbShow, &hsbShow);

    if (vsbShow)
    {
        m_vscrollPos = fixScrollPos(m_vscrollPos, Qt::Vertical);
        int maxScrollPos = calcVScrollBarMax();
        m_vsb->show();
        m_vsb->setPageStep(calcVScrollbarPageStep());
        m_vsb->blockSignals(true);
        if (m_vsb->maximum() != maxScrollPos)
        {
            m_vsb->setMaximum(maxScrollPos);
        }
        m_vsb->setValue(m_vscrollPos);
        m_vsb->blockSignals(false);
    }
    else
    {
        m_vscrollPos = 0;
        m_vsb->hide();
    }

    if (hsbShow)
    {
        m_hscrollPos = fixScrollPos(m_hscrollPos, Qt::Horizontal);
        int maxScrollPos = calcHScrollBarMax(vsbShow);
        m_hsb->show();
        m_hsb->setPageStep(calcHScrollbarPageStep(vsbShow));
        m_hsb->blockSignals(true);
        if (m_hsb->maximum() != maxScrollPos)
        {
            m_hsb->setMaximum(maxScrollPos);
        }
        m_hsb->setValue(m_hscrollPos);
        m_hsb->blockSignals(false);

    }
    else
    {
        m_hscrollPos = 0;
        m_hsb->hide();
    }

    updateViewPort(true);
}

void MtRecyclerMultiColumnListView::updateViewPort(bool updateAll) {
    int headHideRowCount = 0;
    int remain = 0;

    headHideRowCount = m_vscrollPos / m_rowHeight;
    remain = m_vscrollPos % m_rowHeight;

    int bottomPart = height() - m_headHeight + remain;
    int visibleRowCount = bottomPart / m_rowHeight;
    if (bottomPart % m_rowHeight) {
        visibleRowCount++;
    }

    if ((visibleRowCount + headHideRowCount) > m_listAdapter->getRowCount()) {
        visibleRowCount = m_listAdapter->getRowCount() - headHideRowCount;
    }

    int firstIdx = headHideRowCount;

    QList<RowData*> itemRecycleBin;
    QMap<int, RowData*> index2Items;

    for (int i = 0; i < m_listCachedRows.size(); i++) {
        int idx = m_listCachedRows[i]->index;
        if (idx >= firstIdx && idx < (firstIdx + visibleRowCount)) {
            index2Items.insert(idx, m_listCachedRows[i]);
        } else {
            itemRecycleBin.append(m_listCachedRows[i]);
        }
    }
    m_listCachedRows.clear();

    QList<RowData *> newCachedItems;
    for (int i = 0; i < visibleRowCount; i++)
    {
        int idx = i + firstIdx;
        auto iter = index2Items.find(idx);
        if (iter != index2Items.end()) {
            RowData *data = iter.value();
            index2Items.erase(iter);
            newCachedItems.append(data);
            if (updateAll) {
                m_listAdapter->updateRow(data->index, data->widgets, m_colCount);
            }
        } else if (!itemRecycleBin.isEmpty()) {
            RowData *data = itemRecycleBin.back();
            itemRecycleBin.pop_back();
            data->index = idx;
            newCachedItems.append(data);
            m_listAdapter->updateRow(data->index, data->widgets, m_colCount);
        } else {
            RowData *data = new RowData;
            data->index = idx;
            m_listAdapter->createRow(idx, data->widgets);
            for (int j = 0; j < data->widgets.size(); j++) {
                data->widgets[j]->show();
            }
            newCachedItems.append(data);
        }
    }

    for (int i = 0; i < itemRecycleBin.size(); i++) {
        for (int j = 0; j < itemRecycleBin[i]->widgets.size(); j++) {
            itemRecycleBin[i]->widgets[j]->deleteLater();
        }
        delete itemRecycleBin[i];
    }

    m_listCachedRows.swap(newCachedItems);

    layoutListItems();
}

void MtRecyclerMultiColumnListView::layoutListItems()
{
    if (m_headHeight > 0) {
        m_header->setVisible(true);
        m_header->move(-m_hscrollPos, 0);
        m_header->raise();
    } else {
        m_header->setVisible(false);
    }

    for (int r = 0; r < m_listCachedRows.size(); r++)
    {
        auto& row = m_listCachedRows[r];
        int y = m_headHeight + row->index * m_rowHeight - m_vscrollPos;
        int x = -m_hscrollPos;
        for (int c = 0; c < row->widgets.size(); c++)
        {
            QWidget *widget = row->widgets[c];
            int width = m_header->getColumnWidth(c);
            int w = width - m_ceilMargin.left() - m_ceilMargin.right();
            int h = m_rowHeight - m_ceilMargin.top() - m_ceilMargin.bottom();
            QRect rc(x + m_ceilMargin.left(), y + m_ceilMargin.top(), w, h);
            widget->setGeometry(rc);
            x += width;
        }
    }

    bool vsbShow = (m_vsb && !m_vsb->isHidden());
    bool hsbShow = (m_hsb && !m_hsb->isHidden());

    if (vsbShow) {
        int sbw = m_listAdapter->scrollBarWidth();
        QMargins sbMargin = m_listAdapter->scrollBarMargin();
        int hsbWidth = hsbShow ? sbw : 0;
        m_vsb->setGeometry(width() - sbw - sbMargin.right(), sbMargin.top(), sbw, height() - sbMargin.top() - sbMargin.bottom() - hsbWidth);
        m_vsb->raise();
    }
    if (hsbShow) {
        int sbw = m_listAdapter->scrollBarWidth();
        QMargins sbMargin = m_listAdapter->scrollBarMargin();
        int vsbWidth = vsbShow ? sbw : 0;
        m_hsb->setGeometry(sbMargin.left(), height() - sbw - sbMargin.bottom(), width() - sbMargin.left() - sbMargin.right() - vsbWidth, sbw);
        m_hsb->raise();
    }

    if (m_isHover)
    {
        m_indexOfHover = calcIndexOfPoint(m_mousePoint);
    }

    repaint();
}

void MtRecyclerMultiColumnListView::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    // 填充白色作为背景色
    QPainter painter(this);

    QBrush brush(Qt::white);
    painter.fillRect(0, 0, width(), height(), brush);

    if (m_indexOfHover != -1)
    {
        RowData* rd = m_listCachedRows[m_indexOfHover];
        if (!rd->widgets.isEmpty())
        {
            QPoint ptA = rd->widgets.front()->geometry().topLeft();
            QPoint ptB = rd->widgets.back()->geometry().bottomRight();

            ptA.setX(ptA.x() - m_ceilMargin.left());
            ptA.setY(ptA.y() - m_ceilMargin.top());

            ptB.setX(ptB.x() + m_ceilMargin.right());
            ptB.setY(ptB.y() + m_ceilMargin.bottom());

            QRect rc(ptA, ptB);
            QBrush brush(QColor(0xFFF2FBFF));
            painter.fillRect(rc, brush);
        }
    }

    if (m_showGride)
    {
        QPen pen(QColor(0xE8E8E8), 1);
        painter.setPen(pen);
        for (int i = 0; i < m_listCachedRows.size(); i++)
        {
            auto row0 = m_listCachedRows[i]->widgets.front();
            auto row1 = m_listCachedRows[i]->widgets.back();
            int x1 = row0->geometry().left() - m_ceilMargin.left();
            int x2 = row1->geometry().right() + m_ceilMargin.right();
            int y = row0->geometry().bottom() + m_ceilMargin.bottom();

            painter.drawLine(QPoint(x1, y), QPoint(x2, y));
        }


        for (int i = 0; i < m_colCount; i++)
        {
            auto item1 = m_listCachedRows.front()->widgets[i];
            auto item2 = m_listCachedRows.back()->widgets[i];

            int x = item1->geometry().left() - m_ceilMargin.left();
            int y1 = item1->geometry().top() - m_ceilMargin.top();
            int y2 = item2->geometry().bottom() + m_ceilMargin.bottom();
            painter.drawLine(QPoint(x,y1), QPoint(x, y2));
        }

        if (m_colCount > 0)
        {
            auto item1 = m_listCachedRows[0]->widgets.back();
            auto item2 = m_listCachedRows.back()->widgets.back();

            int x = item1->geometry().right() + m_ceilMargin.right();
            int y1 = item1->geometry().top() - m_ceilMargin.top();
            int y2 = item2->geometry().bottom() + m_ceilMargin.bottom();
            painter.drawLine(QPoint(x,y1), QPoint(x, y2));
        }
    }
}

void MtRecyclerMultiColumnListView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    buildListItems();
}

void MtRecyclerMultiColumnListView::wheelEvent(QWheelEvent *event) {
    QWidget::wheelEvent(event);
    if (m_contentHeight <= (height() - m_headHeight))
        return;

    int delta = event->angleDelta().ry();
    delta = round((float)delta / (float)m_rowHeight) * m_rowHeight;

    scrollTo(m_vscrollPos - delta, Qt::Orientation::Vertical);
}

void MtRecyclerMultiColumnListView::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    m_mousePoint = event->pos();
    int indexOfHover = calcIndexOfPoint(event->pos());

    if (indexOfHover != m_indexOfHover)
    {
        m_indexOfHover = indexOfHover;
        repaint();
    }
}

void MtRecyclerMultiColumnListView::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    m_isHover = true;
}

void MtRecyclerMultiColumnListView::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    m_isHover = false;
    if (m_indexOfHover != -1)
    {
        m_indexOfHover = -1;
        repaint();
    }
}

void MtRecyclerMultiColumnListView::scrollTo(int pos, Qt::Orientation ori)
{
    auto updateSb = [this](QScrollBar* sb, int newVal) -> void {
        sb->blockSignals(true);
        sb->setValue(newVal);
        sb->blockSignals(false);

        updateViewPort(false);
    };

    if (ori == Qt::Orientation::Vertical)
    {
        int newScrollPos = fixScrollPos(pos, ori);
        if (newScrollPos != m_vscrollPos)
        {
            m_vscrollPos = newScrollPos;
            updateSb(m_vsb, m_vscrollPos);
        }
    }
    if (ori == Qt::Orientation::Horizontal)
    {
        int newScrollPos = fixScrollPos(pos, ori);
        if (newScrollPos != m_hscrollPos)
        {
            m_hscrollPos = newScrollPos;
            updateSb(m_hsb, m_hscrollPos);
        }
    }
}

int MtRecyclerMultiColumnListView::fixScrollPos(int newScrollPos,
                                                Qt::Orientation ori) {

    auto fixVal = [](int val, int minVal, int maxVal) -> int {
        if (val < minVal)
            return minVal;
        if (val > maxVal)
            return maxVal;
        return val;
    };

    if (ori == Qt::Vertical)
    {
        int maxScroVal = calcVScrollBarMax();
        return fixVal(newScrollPos, 0, maxScroVal);
    }
    else if (ori == Qt::Horizontal)
    {
        int maxScroVal = calcHScrollBarMax(m_vsb->isVisible());
        return fixVal(newScrollPos, 0, maxScroVal);
    }
    return newScrollPos;
}

MultiColumnListHeader *MtRecyclerMultiColumnListView::getHeader()
{
    return m_header;
}

void MtRecyclerMultiColumnListView::setGrideVisible(bool visible) {
    if (m_showGride != visible)
    {
        m_showGride = visible;
        repaint();
    }
}

bool MtRecyclerMultiColumnListView::getGrideVisible() {
    return m_showGride;
}

void MtRecyclerMultiColumnListView::notifyUpdate()
{
    if (!m_markAsDirty)
    {
        m_markAsDirty = true;
        emit sigUpdate();
    }
}

void MtRecyclerMultiColumnListView::update()
{
    m_markAsDirty = false;
    buildListItems();
}

void MtRecyclerMultiColumnListView::onVScrollBarChanged(int val) {
    scrollTo(val, Qt::Orientation::Vertical);
}

void MtRecyclerMultiColumnListView::onHScrollBarChanged(int val)
{
    scrollTo(val, Qt::Orientation::Horizontal);
}

void MtRecyclerMultiColumnListView::calcScrollBarShowStatus(bool *vScrollBarShow, bool *hScrollBarShow)
{
    bool vsbShow = m_contentHeight > calcVScrollbarPageStep();
    bool hsbShow = m_header->getTotalWidth() > calcHScrollbarPageStep(vsbShow);

    if (vScrollBarShow)
    {
        *vScrollBarShow = vsbShow;
    }
    if (hScrollBarShow)
    {
        *hScrollBarShow = hsbShow;
    }
}

int MtRecyclerMultiColumnListView::calcVScrollBarMax()
{
    int ret = m_contentHeight - calcVScrollbarPageStep();
    return ret > 0 ? ret : 0;
}

int MtRecyclerMultiColumnListView::calcVScrollbarPageStep()
{
    return (height() - m_headHeight);
}

int MtRecyclerMultiColumnListView::calcHScrollBarMax(bool isVsbShow)
{
    int ret = m_header->getTotalWidth() - calcHScrollbarPageStep(isVsbShow);
    return ret > 0 ? ret : 0;
}

int MtRecyclerMultiColumnListView::calcHScrollbarPageStep(bool isVsbShow)
{
    int visualWidth = width();
    if (isVsbShow)
    {
        QMargins m = m_listAdapter->scrollBarMargin();
        visualWidth -= m_listAdapter->scrollBarWidth() + m.left() + m.right();
    }
    return visualWidth;
}

int MtRecyclerMultiColumnListView::calcIndexOfPoint(QPoint pt)
{
    if (m_header->geometry().contains(pt) || m_listCachedRows.isEmpty())
    {
        return -1;
    }

    auto yStart = m_listCachedRows.front()->widgets.front()->geometry().top() - m_ceilMargin.top();
    auto yEnd = m_listCachedRows.back()->widgets.front()->geometry().bottom() + m_ceilMargin.bottom();
    if (pt.y() < yStart || pt.y() >= yEnd)
        return - 1;

    int index = (pt.y() - yStart) / m_rowHeight;
    if (m_listCachedRows.size() <= index)
        return - 1;
    return index;
}
