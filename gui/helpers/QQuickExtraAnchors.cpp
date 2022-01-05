
#include "QQuickExtraAnchors.h"

#include <QQmlProperty>

QQuickExtraAnchors::QQuickExtraAnchors (QObject * parent)
    : QObject (parent)
    , m_topDock           { Q_NULLPTR }
    , m_leftDock          { Q_NULLPTR }
    , m_rightDock         { Q_NULLPTR }
    , m_bottomDock        { Q_NULLPTR }
    , m_verticalFill      { Q_NULLPTR }
    , m_horizontalFill    { Q_NULLPTR }
    , m_topLeftCorner     { Q_NULLPTR }
    , m_topRightCorner    { Q_NULLPTR }
    , m_bottomLeftCorner  { Q_NULLPTR }
    , m_bottomRightCorner { Q_NULLPTR }
    , m_anchors           { (parent ? QQmlProperty (parent, "anchors").read ().value<QObject *> () : Q_NULLPTR) }
{
    connect (this, &QQuickExtraAnchors::verticalFillChanged, [&] (void) {
        defineAnchorLine (m_verticalFill, TOP);
        defineAnchorLine (m_verticalFill, BOTTOM);
    });
    connect (this, &QQuickExtraAnchors::horizontalFillChanged, [&] (void) {
        defineAnchorLine (m_horizontalFill, LEFT);
        defineAnchorLine (m_horizontalFill, RIGHT);
    });
    connect (this, &QQuickExtraAnchors::topLeftCornerChanged, [&] (void) {
        defineAnchorLine (m_topLeftCorner, TOP);
        defineAnchorLine (m_topLeftCorner, LEFT);
    });
    connect (this, &QQuickExtraAnchors::topRightCornerChanged, [&] (void) {
        defineAnchorLine (m_topRightCorner, TOP);
        defineAnchorLine (m_topRightCorner, RIGHT);
    });
    connect (this, &QQuickExtraAnchors::bottomLeftCornerChanged, [&] (void) {
        defineAnchorLine (m_bottomLeftCorner, LEFT);
        defineAnchorLine (m_bottomLeftCorner, BOTTOM);
    });
    connect (this, &QQuickExtraAnchors::bottomRightCornerChanged, [&] (void) {
        defineAnchorLine (m_bottomRightCorner, RIGHT);
        defineAnchorLine (m_bottomRightCorner, BOTTOM);
    });
    connect (this, &QQuickExtraAnchors::topDockChanged, [&] (void) {
        defineAnchorLine (m_topDock, TOP);
        defineAnchorLine (m_topDock, LEFT);
        defineAnchorLine (m_topDock, RIGHT);
    });
    connect (this, &QQuickExtraAnchors::leftDockChanged, [&] (void) {
        defineAnchorLine (m_leftDock, TOP);
        defineAnchorLine (m_leftDock, LEFT);
        defineAnchorLine (m_leftDock, BOTTOM);
    });
    connect (this, &QQuickExtraAnchors::rightDockChanged, [&] (void) {
        defineAnchorLine (m_rightDock, TOP);
        defineAnchorLine (m_rightDock, RIGHT);
        defineAnchorLine (m_rightDock, BOTTOM);
    });
    connect (this, &QQuickExtraAnchors::bottomDockChanged, [&] (void) {
        defineAnchorLine (m_bottomDock, LEFT);
        defineAnchorLine (m_bottomDock, RIGHT);
        defineAnchorLine (m_bottomDock, BOTTOM);
    });
}

QQuickExtraAnchors * QQuickExtraAnchors::qmlAttachedProperties (QObject * object) {
    return new QQuickExtraAnchors { object };
}

void QQuickExtraAnchors::defineAnchorLine (QQuickItem * otherItem, const Sides side) {
    static const QVariant UNDEFINED { };
    if (m_anchors) {
        const QString lineName {
            [&] (void) -> QString {
                switch (side) {
                    case TOP:    return QStringLiteral ("top");
                    case LEFT:   return QStringLiteral ("left");
                    case RIGHT:  return QStringLiteral ("right");
                    case BOTTOM: return QStringLiteral ("bottom");
                }
                return "";
            } ()
        };
        if (!lineName.isEmpty ()) {
            QQmlProperty prop { m_anchors, lineName };
            if (otherItem != Q_NULLPTR) {
                QQmlProperty tmp { otherItem, lineName };
                prop.write (tmp.read ());
            }
            else {
                prop.write (UNDEFINED);
            }
        }
    }
}
