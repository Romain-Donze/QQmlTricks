
#include "QQuickRowContainer.h"
#include "QQuickContainerAttachedObject.h"

#include <QtMath>

QQuickRowContainer::QQuickRowContainer (QQuickItem * parent)
    : QQuickAbstractContainerBase { parent }
    , m_horizontalSpacing         { 0 }
    , m_horizontalDirection       { HorizontalDirections::LEFT_TO_RIGHT }
{ }

QQuickRowContainer::~QQuickRowContainer (void) { }

void QQuickRowContainer::setupHandlers (void) {
    connect (this, &QQuickRowContainer::horizontalSpacingChanged,   this, &QQuickRowContainer::doLayout);
    connect (this, &QQuickRowContainer::horizontalDirectionChanged, this, &QQuickRowContainer::doLayout);
}

void QQuickRowContainer::relayout (void) {
    int maxImplicitHeight            { 0 };
    int totalFixedImplicitWidth      { 0 };
    int totalStretchedImplicitWidth  { 0 };
    int countFixedItems              { 0 };
    int countStretchedItems          { 0 };
    const QList<QQuickItem *> childItemsList { childItems () };
    QVector<QQuickItem *> layoutItemsList { };
    layoutItemsList.reserve (childItemsList.count ());
    for (QQuickItem * childItem : childItemsList) {
        const QQuickContainerAttachedObject * attached { getContainerAttachedObject (childItem) };
        const int  stretch { (attached ? attached->get_horizontalStretch () : 0) };
        const int  forced  { (attached ? attached->get_forcedWidth ()       : 0) };
        const bool ignored { (attached && attached->get_ignored ()) };
        if (!childItem->inherits (REPEATER_CLASSNAME)) {
            if (!ignored) {
                if (childItem->isVisible ()) {
                    layoutItemsList.append (childItem);
                    if (childItem->implicitHeight () > maxImplicitHeight) {
                        maxImplicitHeight = qCeil (childItem->implicitHeight ());
                    }
                    if (forced > 0) {
                        totalFixedImplicitWidth += forced;
                        ++countFixedItems;
                    }
                    else if (stretch > 0) {
                        if (childItem->implicitWidth () > 0) {
                            totalStretchedImplicitWidth += qCeil (childItem->implicitWidth ());
                        }
                        countStretchedItems += stretch;
                    }
                    else {
                        if (childItem->implicitWidth () > 0) {
                            totalFixedImplicitWidth += qCeil (childItem->implicitWidth ());
                        }
                        ++countFixedItems;
                    }
                }
            }
        }
    }
    const int totalItemsCount { (countFixedItems + countStretchedItems) };
    const int totalSpacing    { (totalItemsCount > 1 ? ((totalItemsCount -1) * m_horizontalSpacing) : 0) };
    setImplicitWidth  (totalFixedImplicitWidth + totalStretchedImplicitWidth + totalSpacing);
    setImplicitHeight (maxImplicitHeight);
    const int stretchedItemWidth { qFloor (qreal (qCeil (width ()) - totalFixedImplicitWidth - totalSpacing) / qreal (countStretchedItems)) };
    int currentPosX { (m_horizontalDirection == HorizontalDirections::RIGHT_TO_LEFT ? qFloor (width ()) : 0) };
    for (QQuickItem * childItem : layoutItemsList) {
        const QQuickContainerAttachedObject * attached { getContainerAttachedObject (childItem) };
        const int stretch { (attached ? attached->get_horizontalStretch () : 0) };
        const int forced  { (attached ? attached->get_forcedWidth ()       : 0) };
        if (forced > 0) {
            childItem->setWidth (forced);
        }
        else if (stretch > 0) {
            childItem->setWidth ((stretchedItemWidth * stretch) + (m_horizontalSpacing * (stretch -1)));
        }
        else {
            childItem->setWidth (qCeil (childItem->implicitWidth ()));
        }
        switch (m_horizontalDirection) {
            case HorizontalDirections::LEFT_TO_RIGHT: {
                childItem->setX (currentPosX);
                currentPosX += childItem->width ();
                currentPosX += m_horizontalSpacing;
                break;
            }
            case HorizontalDirections::RIGHT_TO_LEFT: {
                currentPosX -= childItem->width ();
                childItem->setX (currentPosX);
                currentPosX -= m_horizontalSpacing;
                break;
            }
        }
        childItem->setY (0);
    }
    set_layoutItemsCount (layoutItemsList.count ());
}
