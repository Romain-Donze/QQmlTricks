
#include "QQuickColumnContainer.h"
#include "QQuickContainerAttachedObject.h"

#include <QtMath>

QQuickColumnContainer::QQuickColumnContainer (QQuickItem * parent)
    : QQuickAbstractContainerBase { parent }
    , m_verticalSpacing           { 0 }
    , m_verticalDirection         { HorizontalDirections::LEFT_TO_RIGHT }
{ }

QQuickColumnContainer::~QQuickColumnContainer (void) { }

void QQuickColumnContainer::setupHandlers (void) {
    connect (this, &QQuickColumnContainer::verticalSpacingChanged,   this, &QQuickColumnContainer::doLayout);
    connect (this, &QQuickColumnContainer::verticalDirectionChanged, this, &QQuickColumnContainer::doLayout);
}

void QQuickColumnContainer::relayout (void) {
    int maxImplicitWidth             { 0 };
    int totalFixedImplicitHeight     { 0 };
    int totalStretchedImplicitHeight { 0 };
    int countFixedItems              { 0 };
    int countStretchedItems          { 0 };
    const QList<QQuickItem *> childItemsList { childItems () };
    QVector<QQuickItem *> layoutItemsList { };
    layoutItemsList.reserve (childItemsList.count ());
    for (QQuickItem * childItem : childItemsList) {
        const QQuickContainerAttachedObject * attached { getContainerAttachedObject (childItem) };
        const int  stretch { (attached ? attached->get_verticalStretch () : 0) };
        const int  forced  { (attached ? attached->get_forcedHeight ()    : 0) };
        const bool ignored { (attached && attached->get_ignored ()) };
        if (!childItem->inherits (REPEATER_CLASSNAME)) {
            if (!ignored) {
                if (childItem->isVisible ()) {
                    layoutItemsList.append (childItem);
                    if (childItem->implicitWidth () > maxImplicitWidth) {
                        maxImplicitWidth = qCeil (childItem->implicitWidth ());
                    }
                    if (forced > 0) {
                        totalFixedImplicitHeight += forced;
                        ++countFixedItems;
                    }
                    else if (stretch > 0) {
                        if (childItem->implicitHeight () > 0) {
                            totalStretchedImplicitHeight += qCeil (childItem->implicitHeight ());
                        }
                        countStretchedItems += stretch;
                    }
                    else {
                        if (childItem->implicitHeight () > 0) {
                            totalFixedImplicitHeight += qCeil (childItem->implicitHeight ());
                        }
                        ++countFixedItems;
                    }
                }
            }
        }
    }
    const int totalItemsCount { (countFixedItems + countStretchedItems) };
    const int totalSpacing    { (totalItemsCount > 1 ? ((totalItemsCount -1) * m_verticalSpacing) : 0) };
    setImplicitHeight (totalFixedImplicitHeight + totalStretchedImplicitHeight + totalSpacing);
    setImplicitWidth  (maxImplicitWidth);
    const int stretchedItemHeight { qFloor (qreal (qCeil (height ()) - totalFixedImplicitHeight - totalSpacing) / qreal (countStretchedItems)) };
    int currentPosY { (m_verticalDirection == VerticalDirections::BOTTOM_TO_TOP ? qFloor (height ()) : 0) };
    for (QQuickItem * childItem : layoutItemsList) {
        const QQuickContainerAttachedObject * attached { getContainerAttachedObject (childItem) };
        const int stretch { (attached ? attached->get_verticalStretch () : 0) };
        const int forced  { (attached ? attached->get_forcedHeight ()    : 0) };
        if (attached && (forced > 0)) {
            childItem->setHeight (forced);
        }
        else if (attached && (stretch > 0)) {
            childItem->setHeight ((stretchedItemHeight * stretch) + (m_verticalSpacing * (stretch -1)));
        }
        else {
            childItem->setHeight (qCeil (childItem->implicitHeight ()));
        }
        switch (m_verticalDirection) {
            case VerticalDirections::TOP_TO_BOTTOM: {
                childItem->setY (currentPosY);
                currentPosY += childItem->height ();
                currentPosY += m_verticalSpacing;
                break;
            }
            case VerticalDirections::BOTTOM_TO_TOP: {
                currentPosY -= childItem->height ();
                childItem->setY (currentPosY);
                currentPosY -= m_verticalSpacing;
                break;
            }
        }
        childItem->setX (0);
    }
    set_layoutItemsCount (layoutItemsList.count ());
}
