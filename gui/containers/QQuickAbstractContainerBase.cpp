
#include "QQuickAbstractContainerBase.h"
#include "QQuickContainerAttachedObject.h"

const char * QQuickAbstractContainerBase::REPEATER_CLASSNAME { "QQuickRepeater" };

QQuickAbstractContainerBase::QQuickAbstractContainerBase (QQuickItem * parent)
    : QQuickItem         { parent }
    , m_layoutItemsCount { 0 }
{ }

QQuickAbstractContainerBase::~QQuickAbstractContainerBase (void) { }

void QQuickAbstractContainerBase::doLayout (void) {
    polish ();
}

QQuickContainerAttachedObject * QQuickAbstractContainerBase::getContainerAttachedObject (QQuickItem * item) const {
    return qobject_cast<QQuickContainerAttachedObject *> (qmlAttachedPropertiesObject<QQuickContainerAttachedObject> (item, false));
}

void QQuickAbstractContainerBase::classBegin (void) {
    QQuickItem::classBegin ();
}

void QQuickAbstractContainerBase::componentComplete (void) {
    QQuickItem::componentComplete ();
    connect (this, &QQuickAbstractContainerBase::widthChanged,   this, &QQuickAbstractContainerBase::polish);
    connect (this, &QQuickAbstractContainerBase::heightChanged,  this, &QQuickAbstractContainerBase::polish);
    connect (this, &QQuickAbstractContainerBase::visibleChanged, this, &QQuickAbstractContainerBase::polish);
    setupHandlers ();
    polish ();
}

void QQuickAbstractContainerBase::itemChange (ItemChange change, const ItemChangeData & value) {
    QQuickItem::itemChange (change, value);
    switch (int (change)) {
        case ItemChildAddedChange: {
            if (QQuickItem * child { value.item }) {
                connect (child, &QQuickItem::visibleChanged,        this, &QQuickAbstractContainerBase::polish);
                connect (child, &QQuickItem::implicitWidthChanged,  this, &QQuickAbstractContainerBase::polish);
                connect (child, &QQuickItem::implicitHeightChanged, this, &QQuickAbstractContainerBase::polish);
                if (const QQuickContainerAttachedObject * attached { getContainerAttachedObject (child) }) {
                    connect (attached, &QQuickContainerAttachedObject::ignoredChanged,           this, &QQuickAbstractContainerBase::polish);
                    connect (attached, &QQuickContainerAttachedObject::forcedWidthChanged,       this, &QQuickAbstractContainerBase::polish);
                    connect (attached, &QQuickContainerAttachedObject::forcedHeightChanged,      this, &QQuickAbstractContainerBase::polish);
                    connect (attached, &QQuickContainerAttachedObject::verticalStretchChanged,   this, &QQuickAbstractContainerBase::polish);
                    connect (attached, &QQuickContainerAttachedObject::horizontalStretchChanged, this, &QQuickAbstractContainerBase::polish);
                }
                polish ();
            }
            break;
        }
        case ItemChildRemovedChange: {
            if (QQuickItem * child { value.item }) {
                disconnect (child, Q_NULLPTR, this, Q_NULLPTR);
                if (const QQuickContainerAttachedObject * attached { getContainerAttachedObject (child) }) {
                    disconnect (attached, Q_NULLPTR, this, Q_NULLPTR);
                }
                polish ();
            }
            break;
        }
    }
}

void QQuickAbstractContainerBase::updatePolish (void) {
    if (isVisible ()) {
        QQuickItem::updatePolish ();
        relayout ();
        emit layoutDone ();
    }
}
