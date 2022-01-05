
#include "QQuickContainerAttachedObject.h"

QQuickContainerAttachedObject::QQuickContainerAttachedObject (QObject * parent)
    : QObject             { parent }
    , m_forcedWidth       { 0 }
    , m_forcedHeight      { 0 }
    , m_verticalStretch   { 0 }
    , m_horizontalStretch { 0 }
    , m_ignored           { false }
{ }

QQuickContainerAttachedObject::~QQuickContainerAttachedObject (void) { }

QQuickContainerAttachedObject * QQuickContainerAttachedObject::qmlAttachedProperties (QObject * object) {
    return new QQuickContainerAttachedObject { object };
}
