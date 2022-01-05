#ifndef QQUICKEXTRAANCHORS_H
#define QQUICKEXTRAANCHORS_H

#include <QObject>
#include <QString>
#include <QQuickItem>

#include "QmlPropertyHelpers.h"

class QQuickExtraAnchors : public QObject {
    Q_OBJECT
    /// side dock
    QML_WRITABLE_PTR_PROPERTY (topDock,           QQuickItem)
    QML_WRITABLE_PTR_PROPERTY (leftDock,          QQuickItem)
    QML_WRITABLE_PTR_PROPERTY (rightDock,         QQuickItem)
    QML_WRITABLE_PTR_PROPERTY (bottomDock,        QQuickItem)
    /// axis fill
    QML_WRITABLE_PTR_PROPERTY (verticalFill,      QQuickItem)
    QML_WRITABLE_PTR_PROPERTY (horizontalFill,    QQuickItem)
    /// corner position
    QML_WRITABLE_PTR_PROPERTY (topLeftCorner,     QQuickItem)
    QML_WRITABLE_PTR_PROPERTY (topRightCorner,    QQuickItem)
    QML_WRITABLE_PTR_PROPERTY (bottomLeftCorner,  QQuickItem)
    QML_WRITABLE_PTR_PROPERTY (bottomRightCorner, QQuickItem)

public:
    explicit QQuickExtraAnchors (QObject * parent = Q_NULLPTR);

    static QQuickExtraAnchors * qmlAttachedProperties (QObject * object);

protected:
    enum Sides {
        TOP,
        LEFT,
        RIGHT,
        BOTTOM,
    };

    void defineAnchorLine (QQuickItem * other, const Sides side);

private:
    QObject * m_anchors;
};

QML_DECLARE_TYPE     (QQuickExtraAnchors)
QML_DECLARE_TYPEINFO (QQuickExtraAnchors, QML_HAS_ATTACHED_PROPERTIES)

#endif // QQUICKEXTRAANCHORS_H
