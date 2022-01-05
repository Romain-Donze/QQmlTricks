
#include "QQuickFormContainer.h"
#include "QQuickContainerAttachedObject.h"

#include <QtMath>

struct Line {
    QQuickItem * label     { Q_NULLPTR };
    QQuickItem * field     { Q_NULLPTR };
    int          forcedW   { 0 };
    int          forcedH   { 0 };
    int          computedH { 0 };
    bool         stretched { false };
};

QQuickFormContainer::QQuickFormContainer (QQuickItem * parent)
    : QQuickAbstractContainerBase { parent }
    , m_verticalSpacing           { 0 }
    , m_horizontalSpacing         { 0 }
{ }

void QQuickFormContainer::setupHandlers (void) {
    connect (this, &QQuickFormContainer::verticalSpacingChanged,   this, &QQuickFormContainer::doLayout);
    connect (this, &QQuickFormContainer::horizontalSpacingChanged, this, &QQuickFormContainer::doLayout);
}

void QQuickFormContainer::relayout (void) {
    int itemIdx { 0 };
    int totalItemsHeight { 0 };
    int maxLeftImplicitWidth  { 0 };
    int maxRightImplicitWidth { 0 };
    QVector<Line> layoutLines { };
    const QList<QQuickItem *> childItemsList { childItems () };
    layoutLines.reserve (childItemsList.count ());
    for (QQuickItem * childItem : childItemsList) {
        if (!childItem->inherits (REPEATER_CLASSNAME)) {
            const QQuickContainerAttachedObject * attached { getContainerAttachedObject (childItem) };
            const bool ignored { (attached != Q_NULLPTR && attached->get_ignored ()) };
            if (!ignored && childItem->isVisible ()) {
                if (itemIdx % 2 == 0) {
                    layoutLines.append (Line { });
                    layoutLines.last ().label = childItem;
                    if (maxLeftImplicitWidth < qCeil (childItem->implicitWidth ())) {
                        maxLeftImplicitWidth = qCeil (childItem->implicitWidth ());
                    }
                }
                else {
                    layoutLines.last ().field     = childItem;
                    layoutLines.last ().stretched = (attached != Q_NULLPTR && attached->get_horizontalStretch () > 0);
                    layoutLines.last ().forcedW   = (attached != Q_NULLPTR ? attached->get_forcedWidth  () : 0);
                    layoutLines.last ().forcedH   = (attached != Q_NULLPTR ? attached->get_forcedHeight () : 0);
                    layoutLines.last ().computedH = qMax (qCeil (layoutLines.last ().label->implicitHeight ()),
                                                          qCeil (layoutLines.last ().field->implicitHeight ()));
                    totalItemsHeight += layoutLines.last ().computedH;
                    if (maxRightImplicitWidth < qCeil (childItem->implicitWidth ())) {
                        maxRightImplicitWidth = qCeil (childItem->implicitWidth ());
                    }
                }
                ++itemIdx;
            }
        }
    }
    if (!layoutLines.isEmpty ()) {
        const int totalVerticalSpacing { (layoutLines.count () > 1 ? ((layoutLines.count () -1) * m_verticalSpacing) : 0) };
        setImplicitWidth  (maxLeftImplicitWidth + maxRightImplicitWidth + m_horizontalSpacing);
        setImplicitHeight (totalItemsHeight + totalVerticalSpacing);
        const int strechedRightItemsWidth { (qFloor (width ()) - maxLeftImplicitWidth - m_horizontalSpacing) };
        int currentY { 0 };
        for (const Line & line : layoutLines) {
            if (line.label != nullptr) {
                line.label->setX      (0);
                line.label->setY      (currentY);
                line.label->setWidth  (maxLeftImplicitWidth);
                line.label->setHeight (line.computedH);
            }
            if (line.field != nullptr) {
                line.field->setX      (maxLeftImplicitWidth + m_horizontalSpacing);
                line.field->setY      (currentY);
                line.field->setHeight (line.computedH);
                if (line.stretched) {
                    line.field->setWidth (strechedRightItemsWidth);
                }
                else if (line.forcedW > 0) {
                    line.field->setWidth (line.forcedW);
                }
                else {
                    line.field->setWidth (line.field->implicitWidth ());
                }
            }
            currentY += line.computedH;
            currentY += m_verticalSpacing;
        }
        set_layoutItemsCount (layoutLines.count () * 2);
    }
    else {
        setImplicitWidth  (0);
        setImplicitHeight (0);
        set_layoutItemsCount (0);
    }
}
