
#include "QQmlIntrospector.h"

#include <QMetaObject>

QQuickGroupItem::QQuickGroupItem (QQuickItem * parent)
    : QQuickItem { parent }
    , m_title    { }
    , m_icon     { Q_NULLPTR }
{ }

QQuickGroupItem::~QQuickGroupItem (void) { }

QQmlIntrospector::QQmlIntrospector (QObject * parent) : QObject (parent) { }

QObject * QQmlIntrospector::qmlSingletonProvider (QQmlEngine * qmlEngine, QJSEngine * jsEngine) {
    Q_UNUSED (qmlEngine)
    Q_UNUSED (jsEngine)
    return new QQmlIntrospector;
}

bool QQmlIntrospector::isGroupItem (QObject * object) {
    return (qobject_cast<QQuickGroupItem *> (object) != Q_NULLPTR);
}

QQuickWindow * QQmlIntrospector::window (QQuickItem * item) {
    return (item != Q_NULLPTR ? item->window () : Q_NULLPTR);
}

bool QQmlIntrospector::inherits (QObject * object, QObject * reference) {
    bool ret = false;
    if (object != Q_NULLPTR && reference != Q_NULLPTR) {
        const QString objectClass    = QString::fromLatin1 (object->metaObject ()->className ());
        const QString referenceClass = QString::fromLatin1 (reference->metaObject ()->className ());
        ret = (objectClass == referenceClass);
        if (!ret) {
            ret = object->inherits (reference->metaObject ()->className ());
        }
    }
    return ret;
}
