#include "multicolumnlistheader.h"
#include <QPainter>


MultiColumnListHeader::MultiColumnListHeader(QWidget *parent) : QWidget(parent) {}

void MultiColumnListHeader::SetHeaderText(QStringList headList) {
    m_headList = headList;
    while (m_itemWidth.size() < m_headList.size()) {
        m_itemWidth.push_back(getDefaultWidth());
    }

    while (m_itemWidth.size() > m_headList.size()) {
        m_itemWidth.pop_back();
    }

    int totalW = 0;
    for (int i = 0; i < m_itemWidth.size(); i++) {
        totalW += m_itemWidth[i];
    }
    resize(totalW, height());

    setAttribute(Qt::WA_MouseTracking);
}

void MultiColumnListHeader::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);

    QRect rc(QPoint(), size());

    QPen penGrid(QColor(0xFFC2C9CC), 1);
    QPen penText(QColor(0xFF111111));

    QFont font("Microsoft YaHei");
    font.setPixelSize(14);
    painter.setFont(font);

    QBrush bgBrush(QColor(0xFFF2FBFF));
    painter.fillRect(rc, bgBrush);

    int x = 0;
    for (int i = 0; i < m_headList.size(); i++)
    {
        QRectF rect(x, 0, m_itemWidth[i], height());
        QRectF txtRect(x + m_margins.left(), 0, m_itemWidth[i] - m_margins.left() - m_margins.right(), height());
        painter.setPen(penText);
        painter.drawText(txtRect, Qt::AlignVCenter|Qt::AlignLeft, m_headList[i]);
        painter.setPen(penGrid);

        QPointF ptStart = rect.topRight();
        ptStart.setY(ptStart.y() + 10.0f);
        QPointF ptEnd = rect.bottomRight();
        ptEnd.setY(ptEnd.y() - 10.0f);
        painter.drawLine(ptStart, ptEnd);
        x += m_itemWidth[i];
    }
    //x = width() - 1;
    //painter.drawLine(x, 10.0, width() -1, height() - 10.0f);
}

void MultiColumnListHeader::setColumnWidth(int column, int width, bool update)
{
    bool bHadUpdated = false;
    int index = column;
    if (index >= 0 && index < m_itemWidth.size())
    {
        if (m_itemWidth[index] != width)
        {
            bHadUpdated = true;
        }
        m_itemWidth[index] = width;
    }

    if (bHadUpdated && update)
    {
        repaint();
        emit onWidthChanged();
    }
}

int MultiColumnListHeader::getColumnWidth(int column) {
    if (column >= 0 && column < m_itemWidth.size())
        return m_itemWidth[column];
    else
        return getDefaultWidth();
}

int MultiColumnListHeader::getTotalWidth()
{
    int sum = 0;
    std::for_each(m_itemWidth.begin(), m_itemWidth.end(), [&sum](int val)->void {
        sum += val;
    });

    return sum;
}

void MultiColumnListHeader::setHeaderHeight(int height)
{
    resize(width(), height);
}

int MultiColumnListHeader::getDefaultWidth() {
    return 240;
}

int MultiColumnListHeader::getMinWidth(int nIndex)
{
    Q_UNUSED(nIndex);
    return 80;
}

void MultiColumnListHeader::setHeadMargin(QMargins margins)
{
    m_margins.setLeft(std::max(margins.left(), 1));
    m_margins.setRight(std::max(margins.right(), 1));
}

void MultiColumnListHeader::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    QPoint pt = event->pos();

    if (m_resizeMode)
    {
        if (m_indexOfResize < 0 || m_indexOfResize > m_itemWidth.size())
        {
            m_resizeMode = false;
        }

        int left = 0;
        for (int i = 0; i < m_indexOfResize; i++)
        {
            left += m_itemWidth[i];
        }
        m_itemWidth[m_indexOfResize] = std::max(pt.x() - left, getMinWidth(m_indexOfResize));
        resize(getTotalWidth(), height());
        emit onWidthChanged();
        return;
    }

    int x = 0;
    bool isMouseInSplit = false;
    for (int i = 0; i < m_itemWidth.size(); i++)
    {
        x += m_itemWidth[i];
        int xStart = x - 2;
        int xEnd = x + 2;
        if (xStart <= pt.x() && pt.x() < xEnd)
        {
            m_indexOfResize = i;
            isMouseInSplit = true;
            break;
        }
    }

    if(isMouseInSplit != m_isSizeCursor)
    {
        setCursor(isMouseInSplit ? Qt::CursorShape::SizeHorCursor : Qt::CursorShape::ArrowCursor);
        m_isSizeCursor = isMouseInSplit;
    }
}

void MultiColumnListHeader::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    if (m_isSizeCursor)
    {
        m_resizeMode = true;
    }
}

void MultiColumnListHeader::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    m_resizeMode = false;
}
