#ifndef MULTICOLUMNLISTHEADER_H
#define MULTICOLUMNLISTHEADER_H

#include <QWidget>
#include <QStringList>
#include <QMargins>
#include <QList>
#include <QMouseEvent>

class MultiColumnListHeader: public QWidget
{
    Q_OBJECT
public:
    explicit MultiColumnListHeader(QWidget *parent);

    void SetHeaderText(QStringList headList);
    void setColumnWidth(int column, int width, bool update = true);
    int getColumnWidth(int column);
    int getTotalWidth();

    void setHeaderHeight(int height);
    int getDefaultWidth();
    int getMinWidth(int nIndex);
    void setHeadMargin(QMargins margins);

    void paintEvent(QPaintEvent* e) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void onWidthChanged();

private:
    QStringList m_headList;
    QList<int> m_itemWidth;
    QMargins m_margins;

    int m_indexOfResize = -1;

    bool m_resizeMode = false;
    bool m_isSizeCursor = false;
};

#endif // MULTICOLUMNLISTHEADER_H
