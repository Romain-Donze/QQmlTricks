#ifndef QQUICKROUNDEDRECTANGLEITEM_H
#define QQUICKROUNDEDRECTANGLEITEM_H

#include <QObject>
#include <QColor>
#include <QVariantList>
#include <QQuickItem>
#include <QSGGeometry>

#include "QmlPropertyHelpers.h"

class QQmlAbstractMaterial : public QObject {
    Q_OBJECT

public:
    explicit QQmlAbstractMaterial (QObject * parent = Q_NULLPTR);

    virtual QSGNode * createNodes (QSGGeometry * geometryNode, const QRectF & boundingRect) = 0;

signals:
    void updated (void);
};

class QQmlFlatColorMaterial : public QQmlAbstractMaterial {
    Q_OBJECT
    QML_WRITABLE_CSTREF_PROPERTY (color, QColor)

public:
    explicit QQmlFlatColorMaterial (QObject * parent = Q_NULLPTR);

    QSGNode * createNodes (QSGGeometry * geometryNode, const QRectF & boundingRect) Q_DECL_FINAL;
};

class QQmlAbstractGradientMaterial : public QQmlAbstractMaterial {
    Q_OBJECT
    QML_WRITABLE_CSTREF_PROPERTY (gradientStops, QVariantList)

public:
    explicit QQmlAbstractGradientMaterial (QObject * parent = Q_NULLPTR);

    QSGNode * createNodes (QSGGeometry * geometryNode, const QRectF & boundingRect) Q_DECL_FINAL;

protected:
    virtual void setPointEvenXY (QSGGeometry::ColoredPoint2D * point, const qreal ratio, const QRectF & boundingRect) = 0;
    virtual void setPointOddXY  (QSGGeometry::ColoredPoint2D * point, const qreal ratio, const QRectF & boundingRect) = 0;
};

class QQmlVerticalGradientMaterial : public QQmlAbstractGradientMaterial {
    Q_OBJECT

public:
    explicit QQmlVerticalGradientMaterial (QObject * parent = Q_NULLPTR);

protected:
    void setPointEvenXY (QSGGeometry::ColoredPoint2D * point, const qreal ratio, const QRectF & boundingRect) Q_DECL_FINAL;
    void setPointOddXY  (QSGGeometry::ColoredPoint2D * point, const qreal ratio, const QRectF & boundingRect) Q_DECL_FINAL;
};

class QQmlHorizontalGradientMaterial : public QQmlAbstractGradientMaterial {
    Q_OBJECT

public:
    explicit QQmlHorizontalGradientMaterial (QObject * parent = Q_NULLPTR);

protected:
    void setPointEvenXY (QSGGeometry::ColoredPoint2D * point, const qreal ratio, const QRectF & boundingRect) Q_DECL_FINAL;
    void setPointOddXY  (QSGGeometry::ColoredPoint2D * point, const qreal ratio, const QRectF & boundingRect) Q_DECL_FINAL;
};

class QQuickRoundedRectangleItem : public QQuickItem {
    Q_OBJECT
    QML_WRITABLE_VAR_PROPERTY (border,                    qreal)
    QML_WRITABLE_VAR_PROPERTY (topBorder,                 qreal)
    QML_WRITABLE_VAR_PROPERTY (leftBorder,                qreal)
    QML_WRITABLE_VAR_PROPERTY (rightBorder,               qreal)
    QML_WRITABLE_VAR_PROPERTY (bottomBorder,              qreal)
    QML_WRITABLE_VAR_PROPERTY (radius,                    qreal)
    QML_WRITABLE_VAR_PROPERTY (topLeftRadius,             qreal)
    QML_WRITABLE_VAR_PROPERTY (topRightRadius,            qreal)
    QML_WRITABLE_VAR_PROPERTY (bottomLeftRadius,          qreal)
    QML_WRITABLE_VAR_PROPERTY (bottomRightRadius,         qreal)
    QML_WRITABLE_PTR_PROPERTY (background, QQmlAbstractMaterial)
    QML_WRITABLE_PTR_PROPERTY (foreground, QQmlAbstractMaterial)

public:
    explicit QQuickRoundedRectangleItem (QQuickItem * parent = Q_NULLPTR);

protected:
    void classBegin (void) Q_DECL_FINAL;
    void componentComplete (void) Q_DECL_FINAL;
    QSGNode * updatePaintNode (QSGNode * oldNode, UpdatePaintNodeData * nodeData) Q_DECL_FINAL;
};

#endif // QQUICKROUNDEDRECTANGLEITEM_H
