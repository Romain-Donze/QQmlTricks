
#include "QQuickFastObjectListView.h"
#include "QQmlFastObjectListModel.h"

#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlContext>
#include <QQmlEngine>
#include <QDebug>
#include <QtMath>
#include <QQmlProperty>

QQuickFastObjectListView::QQuickFastObjectListView (QQuickItem * parent)
    : QQuickItem    { parent }
    , m_current     { Q_NULLPTR }
    , m_behavior    { FREE_MOVE }
    , m_spaceBefore { 0 }
    , m_spaceAfter  { 0 }
{ }

QQuickFastObjectListView::~QQuickFastObjectListView (void) {
    qDeleteAll (m_holdersList);
}

void QQuickFastObjectListView::classBegin (void) { }

void QQuickFastObjectListView::componentComplete (void) {
    connect (this, &QQuickFastObjectListView::widthChanged,       this, &QQuickFastObjectListView::doMarkDirty);
    connect (this, &QQuickFastObjectListView::heightChanged,      this, &QQuickFastObjectListView::doMarkDirty);
    connect (this, &QQuickFastObjectListView::visibleChanged,     this, &QQuickFastObjectListView::doMarkDirty);
    connect (this, &QQuickFastObjectListView::modelChanged,       this, &QQuickFastObjectListView::doMarkDirty);
    connect (this, &QQuickFastObjectListView::delegateChanged,    this, &QQuickFastObjectListView::doMarkDirty);
    connect (this, &QQuickFastObjectListView::modelChanged,       this, &QQuickFastObjectListView::doMarkDirty);
    connect (this, &QQuickFastObjectListView::currentChanged ,    this, &QQuickFastObjectListView::doMarkDirty);
    connect (this, &QQuickFastObjectListView::behaviorChanged,    this, &QQuickFastObjectListView::doMarkDirty);
    connect (this, &QQuickFastObjectListView::spaceBeforeChanged, this, &QQuickFastObjectListView::doMarkDirty);
    connect (this, &QQuickFastObjectListView::spaceAfterChanged,  this, &QQuickFastObjectListView::doMarkDirty);
    QQuickItem * item { parentItem () };
    if (item != Q_NULLPTR) {
        if (QObject * anchors = { property (ANCHORS).value<QObject *> () }) {
            anchors->setProperty (TOP,   item->property (TOP));
            anchors->setProperty (LEFT,  item->property (LEFT));
            anchors->setProperty (RIGHT, item->property (RIGHT));
        }
    }
    while (item != Q_NULLPTR) {
        if (item->inherits (FLICKABLE)) {
            m_flickable = item;
            connect (m_flickable, &QQuickItem::widthChanged,  this, &QQuickFastObjectListView::doMarkDirty);
            connect (m_flickable, &QQuickItem::heightChanged, this, &QQuickFastObjectListView::doMarkDirty);
            QQmlProperty propertyContentY { m_flickable, CONTENT_Y };
            propertyContentY.connectNotifySignal (this, SLOT (doMarkDirty()));
            QQmlProperty propertyFlickingV { m_flickable, FLICKING_V };
            propertyFlickingV.connectNotifySignal (this, SLOT (onUserInteraction()));
            QQmlProperty propertyDraggingV { m_flickable, DRAGGING_V };
            propertyDraggingV.connectNotifySignal (this, SLOT (onUserInteraction()));
            break;
        }
        item = item->parentItem ();
    }
    if (!m_flickable) {
        qWarning () << "The FastObjectListView must be placed in a Flickable to work !";
    }
    QQuickItem::componentComplete ();
}

QQmlFastObjectListModelBase * QQuickFastObjectListView::getModel (void) const {
    return m_model;
}

QQmlComponent * QQuickFastObjectListView::getDelegate (void) const {
    return m_delegate;
}

void QQuickFastObjectListView::setModel (QQmlFastObjectListModelBase * model) {
    if (m_model != model) {
        doDetachFromModel ();
        m_model = model;
        doAttachToModel ();
        doResetInstances ();
        doPrepareInstances ();
        emit modelChanged ();
    }
}

void QQuickFastObjectListView::setDelegate (QQmlComponent * delegate) {
    if (m_delegate != delegate) {
        m_delegate = delegate;
        doResetInstances ();
        doPrepareInstances ();
        emit delegateChanged ();
    }
}

void QQuickFastObjectListView::doMarkDirty (void) {
    polish ();
}

void QQuickFastObjectListView::doResetInstances (void) {
    if (!m_holdersList.isEmpty ()) {
        for (Holder * holder : m_holdersList) {
            doEliminate (holder);
        }
        m_holdersList.clear ();
    }
}

void QQuickFastObjectListView::doPrepareInstances (void) {
    if (m_model != Q_NULLPTR && m_delegate != Q_NULLPTR) {
        const int nb { m_model->getCount () };
        m_holdersList.reserve (nb);
        for (int idx { 0 }; idx < nb; ++idx) {
            Holder * holder { new Holder };
            holder->modelItem = m_model->getItem (idx);
            m_holdersList.append (holder);
            doInstantiate (holder);
        }
    }
}


void QQuickFastObjectListView::doAttachToModel (void) {
    if (m_model != Q_NULLPTR) {
        connect (m_model, &QQmlFastObjectListModelBase::itemInserted, this, &QQuickFastObjectListView::onItemInserted);
        connect (m_model, &QQmlFastObjectListModelBase::itemRemoved,  this, &QQuickFastObjectListView::onItemRemoved);
        connect (m_model, &QQmlFastObjectListModelBase::itemsCleared, this, &QQuickFastObjectListView::onItemsCleared);
    }
}

void QQuickFastObjectListView::doDetachFromModel (void) {
    if (m_model != Q_NULLPTR) {
        disconnect (m_model, &QQmlFastObjectListModelBase::itemInserted, this, &QQuickFastObjectListView::onItemInserted);
        disconnect (m_model, &QQmlFastObjectListModelBase::itemRemoved,  this, &QQuickFastObjectListView::onItemRemoved);
        disconnect (m_model, &QQmlFastObjectListModelBase::itemsCleared, this, &QQuickFastObjectListView::onItemsCleared);
    }
}

void QQuickFastObjectListView::doInstantiate (Holder * holder) {
    if (holder != Q_NULLPTR) {
        if (QQmlContext * context = { qmlContext (this) }) {
            if (QQmlContext * subContext = { new QQmlContext { context, this } }) {
                subContext->setContextProperty (MODEL_ITEM, holder->modelItem);
                if (QObject * tmp = { m_delegate->create (subContext) }) {
                    if (QQuickItem * instance = { qobject_cast<QQuickItem *> (tmp) }) {
                        instance->setParent     (this);
                        instance->setParentItem (this);
                        if (QObject * tmp = { instance->property (ANCHORS).value<QObject *> () }) {
                            tmp->setProperty (LEFT,  this->property (LEFT));
                            tmp->setProperty (RIGHT, this->property (RIGHT));
                        }
                        connect (instance, &QQuickItem::heightChanged, this, &QQuickFastObjectListView::doMarkDirty);
                        holder->delegateInstance = instance;
                        holder->context = subContext;
                    }
                    else {
                        tmp->deleteLater ();
                        subContext->deleteLater ();
                    }
                }
            }
        }
    }
}

void QQuickFastObjectListView::doEliminate (Holder * holder) {
    if (holder != Q_NULLPTR) {
        if (holder->delegateInstance != Q_NULLPTR) {
            holder->delegateInstance->setParentItem (Q_NULLPTR);
            holder->delegateInstance->deleteLater ();
            holder->delegateInstance = Q_NULLPTR;
        }
        if (holder->context != Q_NULLPTR) {
            holder->context->deleteLater ();
            holder->context = Q_NULLPTR;
        }
    }
}

void QQuickFastObjectListView::onItemsCleared (void) {
    doResetInstances ();
}

void QQuickFastObjectListView::onUserInteraction (void) {
    if (m_flickable && (m_flickable->property (DRAGGING_V).toBool () || m_flickable->property (FLICKING_V).toBool ())) {
        set_behavior (FREE_MOVE);
        set_current (Q_NULLPTR);
    }
}

void QQuickFastObjectListView::onItemInserted (QObject * item, const int idx) {
    if (item != Q_NULLPTR) {
        Holder * holder { new Holder };
        holder->modelItem = item;
        m_holdersList.insert (idx, holder);
        doInstantiate (holder);
        polish ();
    }
}

void QQuickFastObjectListView::onItemRemoved (QObject * item, const int idx) {
    Q_UNUSED (item)
    if (idx >= 0 && idx < m_holdersList.count ()) {
        Holder * holder { m_holdersList.at (idx) };
        doEliminate (holder);
        delete m_holdersList.takeAt (idx);
    }
    polish ();
}

void QQuickFastObjectListView::updatePolish (void) {
    if (m_flickable != Q_NULLPTR) {
        const int viewportW { qFloor (m_flickable->width ()) };
        const int viewportH { qFloor (m_flickable->height ()) };
        const int contentY  { qRound (m_flickable->property (CONTENT_Y).toReal ()) };
        int endY { m_spaceBefore };
        QQuickItem * currentDelegate { Q_NULLPTR };
        for (Holder * holder : qAsConst (m_holdersList)) {
            if (holder->delegateInstance != Q_NULLPTR) {
                holder->delegateInstance->setY (endY);
                holder->delegateInstance->setVisible (!(((endY + holder->delegateInstance->height ()) < (contentY - viewportH)) || (endY > (contentY + viewportH * 2))));
                endY += qCeil (holder->delegateInstance->height ());
                if (m_current == holder->modelItem) {
                    currentDelegate = holder->delegateInstance;
                }
            }
        }
        endY += m_spaceAfter;
        setImplicitHeight (endY);
        m_flickable->setProperty (CONTENT_W, viewportW);
        m_flickable->setProperty (CONTENT_H, implicitHeight ());
        const int roomH { (viewportH - m_spaceBefore - m_spaceAfter) };
        switch (m_behavior) {
            case KEEP_AT_TOP: {
                if (currentDelegate) {
                    doChangePositionY (qCeil (currentDelegate->y ()) - m_spaceBefore);
                }
                break;
            }
            case KEEP_AT_BOTTOM: {
                if (currentDelegate) {
                    doChangePositionY (qFloor (currentDelegate->y () + currentDelegate->height () - viewportH) + m_spaceAfter);
                }
                break;
            }
            case KEEP_CENTERED: {
                if (currentDelegate) {
                    if (currentDelegate->height () < roomH) {
                        doChangePositionY (qRound (currentDelegate->y () - (roomH - currentDelegate->height ()) / 2) - m_spaceBefore);
                    }
                    else {
                        doChangePositionY (qCeil (currentDelegate->y ()) - m_spaceBefore);
                    }
                }
                break;
            }
            case FREE_MOVE: break;
            default: break;
        }
    }
}

void QQuickFastObjectListView::doChangePositionY (const int posY) {
    const int maxY { (qCeil (implicitHeight ()) - qFloor (m_flickable->height ())) };
    m_flickable->setProperty (CONTENT_Y, ((posY < 0) ? 0 : (posY > maxY ? maxY : posY)));
}
