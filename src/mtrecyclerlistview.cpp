#include "mtrecyclerlistview.h"

#include <QWheelEvent>
#include <QScrollBar>
#include <QPainter>

#include "mtrecyclerlistviewadapter.h"

MtRecyclerListView::MtRecyclerListView(QWidget *parent) : QWidget(parent)
{
}

MtRecyclerListView::~MtRecyclerListView()
{
}

void MtRecyclerListView::setAdapter(MtRecyclerListViewAdapter *adapter)
{
    if (adapter == m_listAdapter)
        return;

    if (m_listAdapter || !adapter)
    {
        return;
    }

    m_listAdapter = adapter;
    m_listAdapter->setListView(this);
    if (m_sb)
    {
        m_sb->deleteLater();
    }

    m_layoutOrient = m_listAdapter->getOrientation();
    m_sb = new QScrollBar(m_layoutOrient, this);
    connect(m_sb, &QScrollBar::valueChanged, this, &MtRecyclerListView::onScrollBarChanged);

    buildListItems();
}

void MtRecyclerListView::updateItem(int index)
{
    if (m_listCachedItems.empty())
        return;

    if (m_listCachedItems.front()->index <= index && index <= m_listCachedItems.back()->index)
    {
        m_listAdapter->updateItem(m_listCachedItems[index - m_listCachedItems.front()->index]->widget, index);
    }
}

void MtRecyclerListView::updateList()
{
    buildListItems(m_scrollPos);
}

int MtRecyclerListView::getItemIndex(QWidget *item)
{
    for (int i = 0; i < m_listCachedItems.size(); i++)
    {
        if (m_listCachedItems[i]->widget == item)
        {
            return m_listCachedItems[i]->index;
        }
    }

    return -1;
}

void MtRecyclerListView::makeItemVisible(int index)
{
    if (!isBigEnoughForScroll())
        return;

    if (index >= m_itemCount)
        return;

    int cacheFirstIndex = m_listCachedItems.front()->index;
    int cacheLastIndex = m_listCachedItems.back()->index;

    if (index == 0)  // 滚到最前面
    {
        scrollTo(0);
    }
    else if (index == (m_itemCount - 1)) //滚到最后面
    {
        scrollTo(m_contentLength - getListGeometryLength());
    }
    else if (index <= cacheFirstIndex) //把第index个元素滚到可见区域的最前边
    {
        int newScrollPos = index * (m_itemLen + m_itemSpace) + m_layoutLeadSpace;
        scrollTo(newScrollPos);
    }
    else if (index >= cacheLastIndex) //把第index个元素滚到可见区域的末尾
    {
        int newScrollPos = m_layoutLeadSpace + (index + 1) * (m_itemLen + m_itemSpace) - m_itemSpace - getListGeometryLength();
        scrollTo(newScrollPos);
    }
}

void MtRecyclerListView::scrollTo(int pos)
{
    int newScrollPos = fixScrollPos(pos);
    if (newScrollPos != m_scrollPos)
    {
        m_scrollPos = newScrollPos;

        m_sb->blockSignals(true);
        m_sb->setValue(m_scrollPos);
        m_sb->blockSignals(false);

        updateViewPort(false);
    }
}

void MtRecyclerListView::updateViewPort(bool updateAll)
{
    int headHideCount = 0;
    int remain = 0;
    if (m_scrollPos < m_layoutLeadSpace)
    {
        headHideCount = 0;
    }
    else
    {
        headHideCount = (m_scrollPos - m_layoutLeadSpace) / (m_itemLen + m_itemSpace);
        remain = (m_scrollPos - m_layoutLeadSpace) % (m_itemLen + m_itemSpace);
    }

    if (remain >= m_itemLen)
    {
        headHideCount ++;
    }

    int rightPart =  getListGeometryLength();
    if (remain < m_itemLen)
    {
        rightPart += remain;
    }
    else
    {
        rightPart = rightPart + (m_itemLen + m_itemSpace - remain);
    }

    int visibleCount = rightPart / (m_itemLen + m_itemSpace);
    if (rightPart % (m_itemLen + m_itemSpace))
    {
        visibleCount++;
    }

    if ((visibleCount + headHideCount) > m_listAdapter->getCount())
    {
        visibleCount = m_listAdapter->getCount() - headHideCount;
    }

    int firstIdx = headHideCount;

    QList<ItemData*> itemRecycleBin;
    QMap<int, ItemData*> index2Items;

    for (int i = 0; i < m_listCachedItems.size(); i++)
    {
        int idx = m_listCachedItems[i]->index ;
        if (idx >= firstIdx && idx < (firstIdx + visibleCount))
        {
            index2Items.insert(idx, m_listCachedItems[i]);
        }
        else
        {
            itemRecycleBin.append(m_listCachedItems[i]);
        }
    }
    m_listCachedItems.clear();

    QList<ItemData*> newCachedItems;
    for (int i = 0; i < visibleCount; i++)
    {
        int idx = i + firstIdx;
        auto iter = index2Items.find(idx);
        if (iter != index2Items.end())
        {
            ItemData* data = iter.value();
            index2Items.erase(iter);
            newCachedItems.append(data);
            if(updateAll)
            {
                m_listAdapter->updateItem(data->widget, data->index);
            }
        }
        else if (!itemRecycleBin.isEmpty())
        {
            ItemData* data = itemRecycleBin.back();
            itemRecycleBin.pop_back();
            data->index = idx;
            newCachedItems.append(data);
            m_listAdapter->updateItem(data->widget, data->index);
        }
        else
        {
            ItemData* data = new ItemData;
            data->index = idx;
            data->widget = m_listAdapter->createListItem(idx, this);
            data->widget->setVisible(true);
            newCachedItems.append(data);
        }
    }

    for (int i = 0; i < itemRecycleBin.size(); i++)
    {
        itemRecycleBin[i]->widget->deleteLater();
        delete itemRecycleBin[i];
    }

    m_listCachedItems.swap(newCachedItems);

    layoutListItems();
}

bool MtRecyclerListView::isBigEnoughForScroll()
{
    return m_contentLength > getListGeometryLength();
}

void MtRecyclerListView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    buildListItems(m_scrollPos);
}

void MtRecyclerListView::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

#if 1
    // 填充白色作为背景色
    QPainter painter(this);

    QBrush brush(Qt::white);
    painter.fillRect(0, 0, width(), height(), brush);
#endif
}

void MtRecyclerListView::buildListItems(int defaultScrollPos)
{
    if (!m_listAdapter)
        return;

    m_itemCount = m_listAdapter->getCount();

    QMargins listMargin = m_listAdapter->margin();
    m_layoutLeadSpace = m_layoutOrient == Qt::Orientation::Horizontal ? listMargin.left() : listMargin.top();
    m_layoutTailSpace = m_layoutOrient == Qt::Orientation::Horizontal ? listMargin.right() : listMargin.bottom();

    QSize size = m_listAdapter->getItemSize();
    m_itemLen = m_layoutOrient == Qt::Orientation::Horizontal ? size.width(): size.height();

    m_itemSpace = m_listAdapter->getItemSpace();

    if (m_itemCount <= 0 || m_itemLen <= 0)
    {
        m_contentLength = 0;
    }
    else
    {
        m_contentLength = m_itemLen * m_itemCount + (m_itemCount - 1) * m_itemSpace + m_layoutLeadSpace + m_layoutTailSpace;
    }

    m_scrollPos = fixScrollPos(defaultScrollPos);
    {
        int geoLen = (m_layoutOrient  == Qt::Orientation::Horizontal) ? width() : height();
        int maxScrollPos = m_contentLength - geoLen;
        if (maxScrollPos <= 0)
        {
            m_scrollPos = 0;
            m_sb->hide();
        }
        else
        {
            m_sb->show();
            m_sb->blockSignals(true);
            if (m_sb->maximum() != maxScrollPos)
            {
                m_sb->setMaximum(maxScrollPos);
            }
            m_sb->setValue(m_scrollPos);
            m_sb->blockSignals(false);
        }
    }
    updateViewPort(true);
}

void MtRecyclerListView::layoutListItems()
{
    QMargins margins = m_listAdapter->margin();
    QSize itemSize = m_listAdapter->getItemSize();
    for (int i = 0; i < m_listCachedItems.size(); i++)
    {
        auto item = m_listCachedItems[i];
        int offset = m_layoutLeadSpace + item->index * (m_itemLen + m_itemSpace) - m_scrollPos;
        QWidget* widget = item->widget;
        if (m_layoutOrient == Qt::Orientation::Horizontal)
        {
            widget->setGeometry(offset, margins.top(), itemSize.width(), itemSize.height());
            offset += m_itemLen + m_itemSpace;
        }
        else
        {
            widget->setGeometry(margins.left(), offset, itemSize.width(), itemSize.height());
            offset += m_itemLen + m_itemSpace;
        }
    }

    if (m_sb && !m_sb->isHidden())
    {
        int sbw = m_listAdapter->scrollBarWidth();
        QMargins sbMargin = m_listAdapter->scrollBarMargin();

        if (m_layoutOrient == Qt::Orientation::Horizontal)
        {
            QRect rc(sbMargin.left(), height() - sbw - sbMargin.bottom(),
                     width() - sbMargin.left() - sbMargin.right(), sbw);
            m_sb->setGeometry(sbMargin.left(), height() - sbw - sbMargin.bottom(),
                              width() - sbMargin.left() - sbMargin.right(), sbw);
        }
        else
        {
            m_sb->setGeometry(width() - sbw - sbMargin.right(),
                              sbMargin.top(), sbw,
                              height() - sbMargin.top() - sbMargin.bottom());
        }
        m_sb->raise();
    }
}

void MtRecyclerListView::wheelEvent(QWheelEvent *event)
{
    QWidget::wheelEvent(event);

    if (!isBigEnoughForScroll())
        return;

    int delta = event->delta();
    int step = (m_itemSpace + m_itemLen);
    delta = round((float)delta /(float)step) * step;

    scrollTo(m_scrollPos - delta);
}

int MtRecyclerListView::getListGeometryLength()
{
    if (m_layoutOrient  == Qt::Orientation::Horizontal)
        return geometry().width();
    else
        return geometry().height();
}

int MtRecyclerListView::fixScrollPos(int newScrollPos)
{
    if (m_contentLength <= getListGeometryLength())
        return 0;

    int minScrollPos = 0;
    int maxScrollPos = m_contentLength  - getListGeometryLength();

    if (newScrollPos < minScrollPos)
        newScrollPos = minScrollPos;
    if (newScrollPos > maxScrollPos)
        newScrollPos = maxScrollPos;

    return newScrollPos;
}

void MtRecyclerListView::onScrollBarChanged(int val)
{
    scrollTo(val);
}
