#ifndef QQUICKCONTAINERATTACHEDOBJECT_H
#define QQUICKCONTAINERATTACHEDOBJECT_H

#include <QQuickItem>

#include "QmlPropertyHelpers.h"

class QQuickContainerAttachedObject : public QObject {
    Q_OBJECT
    QML_WRITABLE_VAR_PROPERTY (forcedWidth,       int)
    QML_WRITABLE_VAR_PROPERTY (forcedHeight,      int)
    QML_WRITABLE_VAR_PROPERTY (verticalStretch,   int)
    QML_WRITABLE_VAR_PROPERTY (horizontalStretch, int)
    QML_WRITABLE_VAR_PROPERTY (ignored,          bool)

public:
    explicit QQuickContainerAttachedObject (QObject * parent = Q_NULLPTR);
    virtual ~QQuickContainerAttachedObject (void);

    static QQuickContainerAttachedObject * qmlAttachedProperties (QObject * object);
};

QML_DECLARE_TYPE     (QQuickContainerAttachedObject)
QML_DECLARE_TYPEINFO (QQuickContainerAttachedObject, QML_HAS_ATTACHED_PROPERTIES)

#endif // QQUICKCONTAINERATTACHEDOBJECT_H
