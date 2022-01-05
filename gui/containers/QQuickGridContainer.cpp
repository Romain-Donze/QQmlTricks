
#include "QQuickGridContainer.h"
#include "QQuickContainerAttachedObject.h"

#include <QtMath>

QQuickGridContainer::QQuickGridContainer (QQuickItem * parent)
    : QQuickAbstractContainerBase { parent }
    , m_cols                      { 1 }
    , m_rows                      { 1 }
    , m_capacity                  { -1 }
    , m_verticalSpacing           { 0 }
    , m_horizontalSpacing         { 0 }
    , m_verticalDirection         { VerticalDirections::TOP_TO_BOTTOM }
    , m_horizontalDirection       { HorizontalDirections::LEFT_TO_RIGHT }
    , m_flowDirection             { FlowDirections::HORIZONTAL }
    , m_fillEmpty                 { true }
{ }

void QQuickGridContainer::setupHandlers (void) {
    connect (this, &QQuickGridContainer::colsChanged,                this, &QQuickGridContainer::doLayout);
    connect (this, &QQuickGridContainer::rowsChanged,                this, &QQuickGridContainer::doLayout);
    connect (this, &QQuickGridContainer::capacityChanged,            this, &QQuickGridContainer::doLayout);
    connect (this, &QQuickGridContainer::verticalSpacingChanged,     this, &QQuickGridContainer::doLayout);
    connect (this, &QQuickGridContainer::horizontalSpacingChanged,   this, &QQuickGridContainer::doLayout);
    connect (this, &QQuickGridContainer::verticalDirectionChanged,   this, &QQuickGridContainer::doLayout);
    connect (this, &QQuickGridContainer::horizontalDirectionChanged, this, &QQuickGridContainer::doLayout);
    connect (this, &QQuickGridContainer::flowDirectionChanged,       this, &QQuickGridContainer::doLayout);
    connect (this, &QQuickGridContainer::fillEmptyChanged,           this, &QQuickGridContainer::doLayout);
}

void QQuickGridContainer::relayout (void) {
    int maxChildWidth { 0 }, maxChildHeight { 0 };
    QVector<QQuickItem *> layoutItemsList { };
    const QList<QQuickItem *> childrenList { childItems () };
    layoutItemsList.reserve (childrenList.count ());
    for (QQuickItem * childItem : childrenList) {
        if (!childItem->inherits (REPEATER_CLASSNAME)) {
            if (childItem->isVisible () || !m_fillEmpty) {
                const QQuickContainerAttachedObject * attached { getContainerAttachedObject (childItem) };
                const bool ignored { (attached != Q_NULLPTR && attached->get_ignored ()) };
                if (!ignored) {
                    layoutItemsList.append (childItem);
                    if (qCeil (childItem->implicitWidth ()) > maxChildWidth) {
                        maxChildWidth = qCeil (childItem->implicitWidth ());
                    }
                    if (qCeil (childItem->implicitHeight ()) > maxChildHeight) {
                        maxChildHeight = qCeil (childItem->implicitHeight ());
                    }
                }
            }
        }
    }
    switch (m_flowDirection) {
        case FlowDirections::HORIZONTAL: {
            if (m_cols > 0) {
                if (m_capacity > 0) {
                    set_rows (qCeil (qreal (m_capacity) / qreal (m_cols)));
                }
                else if (layoutItemsList.count () > 0) {
                    set_rows (qCeil (qreal (layoutItemsList.count ()) / qreal (m_cols)));
                }
                else {
                    set_rows (0);
                }
            }
            break;
        }
        case FlowDirections::VERTICAL: {
            if (m_rows > 0) {
                if (m_capacity > 0) {
                    set_cols (qCeil (qreal (m_capacity) / qreal (m_rows)));
                }
                else if (layoutItemsList.count () > 0) {
                    set_cols (qCeil (qreal (layoutItemsList.count ()) / qreal (m_rows)));
                }
                else {
                    set_cols (0);
                }
            }
            break;
        }
    }
    setImplicitWidth  ((m_cols * maxChildWidth)  + ((m_cols -1) * m_horizontalSpacing));
    setImplicitHeight ((m_rows * maxChildHeight) + ((m_rows -1) * m_verticalSpacing));
    if (m_rows > 0 && m_cols > 0) {
        const qreal itemWidth  { (qreal (width  () - (m_horizontalSpacing * (m_cols -1))) / qreal (m_cols)) };
        const qreal itemHeight { (qreal (height () - (m_verticalSpacing   * (m_rows -1))) / qreal (m_rows)) };
        int currentIdx { 0 };
        for (QQuickItem * childItem : layoutItemsList) {
            const int currentRow { (m_flowDirection == FlowDirections::HORIZONTAL ? (currentIdx / m_cols) : (currentIdx % m_rows)) };
            const int currentCol { (m_flowDirection == FlowDirections::VERTICAL   ? (currentIdx / m_rows) : (currentIdx % m_cols)) };
            childItem->setWidth  (itemWidth);
            childItem->setHeight (itemHeight);
            switch (m_horizontalDirection) {
                case HorizontalDirections::LEFT_TO_RIGHT: {
                    childItem->setX (currentCol * (itemWidth + m_horizontalSpacing));
                    break;
                }
                case HorizontalDirections::RIGHT_TO_LEFT: {
                    childItem->setX (width () - (currentCol * (itemWidth + m_horizontalSpacing)) - itemWidth);
                    break;
                }
            }
            switch (m_verticalDirection) {
                case VerticalDirections::TOP_TO_BOTTOM: {
                    childItem->setY (currentRow * (itemHeight + m_verticalSpacing));
                    break;
                }
                case VerticalDirections::BOTTOM_TO_TOP: {
                    childItem->setY (height () - (currentRow * (itemHeight + m_verticalSpacing)) - itemHeight);
                    break;
                }
            }
            ++currentIdx;
        }
    }
    set_layoutItemsCount (layoutItemsList.count ());
}
