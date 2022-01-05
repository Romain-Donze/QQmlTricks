
#include "QQuickStackContainer.h"
#include "QQuickContainerAttachedObject.h"

#include <QtMath>

QQuickStackContainer::QQuickStackContainer (QQuickItem * parent)
    : QQuickAbstractContainerBase { parent }
{ }

void QQuickStackContainer::setupHandlers (void) { }

void QQuickStackContainer::relayout (void) {
    int maxChildWidth    { 0 };
    int maxChildHeight   { 0 };
    int layoutItemsCount { 0 };
    const QList<QQuickItem *> & childrenList { childItems () };
    for (QQuickItem * childItem : childrenList) {
        if (!childItem->inherits (REPEATER_CLASSNAME)) {
            if (childItem->isVisible ()) {
                const QQuickContainerAttachedObject * attached { getContainerAttachedObject (childItem) };
                const bool ignored { (attached && attached->get_ignored ()) };
                if (!ignored) {
                    if (childItem->implicitWidth () > maxChildWidth) {
                        maxChildWidth = qCeil (childItem->implicitWidth ());
                    }
                    if (childItem->implicitHeight () > maxChildHeight) {
                        maxChildHeight = qCeil (childItem->implicitHeight ());
                    }
                    ++layoutItemsCount;
                }
            }
        }
    }
    setImplicitWidth     (maxChildWidth);
    setImplicitHeight    (maxChildHeight);
    set_layoutItemsCount (layoutItemsCount);
}
