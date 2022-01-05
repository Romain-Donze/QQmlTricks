
#include "QQuickRoundedRectangleItem.h"

#include <QSGNode>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QSGMaterial>
#include <QtMath>
#include <QQuickWindow>
#include <QSGVertexColorMaterial>
#include <QSGClipNode>

QQuickRoundedRectangleItem::QQuickRoundedRectangleItem (QQuickItem * parent)
    : QQuickItem          { parent }
    , m_border            { Q_SNAN }
    , m_topBorder         { Q_SNAN }
    , m_leftBorder        { Q_SNAN }
    , m_rightBorder       { Q_SNAN }
    , m_bottomBorder      { Q_SNAN }
    , m_radius            { Q_SNAN }
    , m_topLeftRadius     { Q_SNAN }
    , m_topRightRadius    { Q_SNAN }
    , m_bottomLeftRadius  { Q_SNAN }
    , m_bottomRightRadius { Q_SNAN }
    , m_background        { Q_NULLPTR }
    , m_foreground        { Q_NULLPTR }
{
    setFlag (QQuickItem::ItemHasContents);
}

void QQuickRoundedRectangleItem::classBegin (void) { }

void QQuickRoundedRectangleItem::componentComplete (void) {
    connect (this, &QQuickRoundedRectangleItem::widthChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::heightChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::visibleChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::borderChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::topBorderChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::leftBorderChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::rightBorderChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::bottomBorderChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::radiusChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::topLeftRadiusChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::topRightRadiusChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::bottomLeftRadiusChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::bottomRightRadiusChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::backgroundChanged,
             this, &QQuickRoundedRectangleItem::update);
    connect (this, &QQuickRoundedRectangleItem::foregroundChanged,
             this, &QQuickRoundedRectangleItem::update);
    if (m_background != nullptr) {
        connect (m_background, &QQmlAbstractMaterial::updated,
                 this, &QQuickRoundedRectangleItem::update);
    }
    if (m_foreground != nullptr) {
        connect (m_foreground, &QQmlAbstractMaterial::updated,
                 this, &QQuickRoundedRectangleItem::update);
    }
    update ();
}

inline bool isValid (const qreal val) {
    return (!qIsNaN (val) && val >= 0.0);
}

inline bool isSignificant (const qreal val) {
    return (!qIsNaN (val) && val > 0.0);
}

inline qreal bestOf (const qreal specific, const qreal generic, const qreal fallback = 0.0) {
    return (isValid (specific) ? specific : (isValid (generic) ? generic : fallback));
}

inline qreal convert (const qreal ratio, const qreal dstMin, const qreal dstMax) {
    return (dstMin + (dstMax - dstMin) * ratio);
}

static void generateCorner (const qreal radius,
                            const qreal centerX, const qreal centerY,
                            const qreal strokeX, const qreal strokeY,
                            const qreal angleStart, const qreal angleEnd,
                            QVector<QPointF> & strokePointsList, QVector<QPointF> & fillPointsList) {
    const int stepsCount { qRound (radius +1) };
    for (int stepIdx { 0 }; stepIdx < stepsCount; ++stepIdx) {
        const qreal stepRatio { (qreal (stepIdx) / qreal (stepsCount -1)) };
        const qreal stepAngle { convert (stepRatio, angleStart, angleEnd) };
        strokePointsList.append (QPointF {
                                     (centerX + radius * qCos (qDegreesToRadians (stepAngle))),
                                     (centerY + radius * qSin (qDegreesToRadians (stepAngle))),
                                 });
        fillPointsList.append (QPointF {
                                   (centerX + (radius - strokeX) * qCos (qDegreesToRadians (stepAngle))),
                                   (centerY + (radius - strokeY) * qSin (qDegreesToRadians (stepAngle))),
                               });
    }
}

static QVector<QPair<qreal,QColor>> parseGradient (const QVariantList & list) {
    QVector<QPair<qreal, QColor>> ret { };
    static const QString COLOR    { "color" };
    static const QString POSITION { "position" };
    for (const QVariant & val : list) {
        const QVariantMap object { val.toMap () };
        const QVariant position { object.value (POSITION) };
        if (position.canConvert<double> ()) {
            const QVariant color { object.value (COLOR) };
            if (color.canConvert<QColor> ()) {
                ret.append ({
                                qreal { position.value<double> () },
                                QColor { color.value<QColor> () },
                            });
            }
            else if (color.canConvert<QString> ()) {
                ret.append ({
                                qreal { position.value<double> () },
                                QColor { color.toString () },
                            });
            }
            else { }
        }
        else { }
    }
    return ret;
}

QQmlAbstractMaterial::QQmlAbstractMaterial (QObject * parent) : QObject { parent } { }

QQmlFlatColorMaterial::QQmlFlatColorMaterial (QObject * parent) : QQmlAbstractMaterial { parent } {
    connect (this, &QQmlFlatColorMaterial::colorChanged, this, &QQmlFlatColorMaterial::updated);
}

QSGNode * QQmlFlatColorMaterial::createNodes (QSGGeometry * geometryNode, const QRectF & boundingRect) {
    Q_UNUSED (boundingRect)
    QSGGeometryNode * ret { new QSGGeometryNode };
    QSGFlatColorMaterial * fillMaterial { new QSGFlatColorMaterial };
    fillMaterial->setColor (m_color);
    ret->setGeometry (geometryNode);
    ret->setMaterial (fillMaterial);
    ret->setFlag (QSGGeometryNode::OwnsGeometry);
    ret->setFlag (QSGGeometryNode::OwnsMaterial);
    ret->setFlag (QSGGeometryNode::OwnedByParent);
    return ret;
}

QQmlAbstractGradientMaterial::QQmlAbstractGradientMaterial (QObject * parent) : QQmlAbstractMaterial { parent } {
    connect (this, &QQmlAbstractGradientMaterial::gradientStopsChanged, this, &QQmlAbstractGradientMaterial::updated);
}

QSGNode * QQmlAbstractGradientMaterial::createNodes (QSGGeometry * geometryNode, const QRectF & boundingRect) {
    QSGClipNode * ret { new QSGClipNode };
    QSGGeometryNode * gradientNode { new QSGGeometryNode };
    const QVector<QPair<qreal, QColor>> gradientStopsList { parseGradient (m_gradientStops) };
    QSGGeometry * gradientGeometry { new QSGGeometry { QSGGeometry::defaultAttributes_ColoredPoint2D (), (gradientStopsList.count () * 2) } };
    gradientGeometry->setDrawingMode (QSGGeometry::DrawTriangleStrip);
    QSGGeometry::ColoredPoint2D * coloredVertex { gradientGeometry->vertexDataAsColoredPoint2D () };
    for (const QPair<qreal, QColor> & gradientStop : qAsConst (gradientStopsList)) {
        setPointEvenXY (coloredVertex, gradientStop.first, boundingRect);
        coloredVertex->r = quint8 (gradientStop.second.red ());
        coloredVertex->g = quint8 (gradientStop.second.green ());
        coloredVertex->b = quint8 (gradientStop.second.blue ());
        coloredVertex->a = quint8 (gradientStop.second.alpha ());
        ++coloredVertex;
        setPointOddXY (coloredVertex, gradientStop.first, boundingRect);
        coloredVertex->r = quint8 (gradientStop.second.red ());
        coloredVertex->g = quint8 (gradientStop.second.green ());
        coloredVertex->b = quint8 (gradientStop.second.blue ());
        coloredVertex->a = quint8 (gradientStop.second.alpha ());
        ++coloredVertex;
    }
    QSGVertexColorMaterial * gradientMaterial { new QSGVertexColorMaterial };
    gradientNode->setMaterial (gradientMaterial);
    gradientNode->setGeometry (gradientGeometry);
    gradientNode->setMaterial (gradientMaterial);
    gradientNode->setFlag (QSGGeometryNode::OwnsGeometry);
    gradientNode->setFlag (QSGGeometryNode::OwnsMaterial);
    gradientNode->setFlag (QSGGeometryNode::OwnedByParent);
    ret->appendChildNode (gradientNode);
    ret->setGeometry (geometryNode);
    ret->setFlag (QSGGeometryNode::OwnsGeometry);
    ret->setFlag (QSGGeometryNode::OwnsMaterial);
    ret->setFlag (QSGGeometryNode::OwnedByParent);
    return ret;
}

QQmlVerticalGradientMaterial::QQmlVerticalGradientMaterial (QObject * parent) : QQmlAbstractGradientMaterial { parent } { }

void QQmlVerticalGradientMaterial::setPointEvenXY (QSGGeometry::ColoredPoint2D * point, const qreal ratio, const QRectF & boundingRect) {
    point->x = float (boundingRect.left ());
    point->y = float (convert (ratio, boundingRect.top (), boundingRect.bottom ()));
}

void QQmlVerticalGradientMaterial::setPointOddXY (QSGGeometry::ColoredPoint2D * point, const qreal ratio, const QRectF & boundingRect) {
    point->x = float (boundingRect.right ());
    point->y = float (convert (ratio, boundingRect.top (), boundingRect.bottom ()));
}

QQmlHorizontalGradientMaterial::QQmlHorizontalGradientMaterial (QObject * parent) : QQmlAbstractGradientMaterial { parent } { }

void QQmlHorizontalGradientMaterial::setPointEvenXY (QSGGeometry::ColoredPoint2D * point, const qreal ratio, const QRectF & boundingRect) {
    point->x = float (convert (ratio, boundingRect.left (), boundingRect.right ()));
    point->y = float (boundingRect.top ());
}

void QQmlHorizontalGradientMaterial::setPointOddXY (QSGGeometry::ColoredPoint2D * point, const qreal ratio, const QRectF & boundingRect) {
    point->x = float (convert (ratio, boundingRect.left (), boundingRect.right ()));
    point->y = float (boundingRect.bottom ());
}

QSGNode * QQuickRoundedRectangleItem::updatePaintNode (QSGNode * oldNode, UpdatePaintNodeData * nodeData) {
    Q_UNUSED (nodeData)
    delete oldNode;
    /// SHAPES
    const qreal radiusTopLeft     { bestOf (m_topLeftRadius, m_radius) };
    const qreal radiusTopRight    { bestOf (m_topRightRadius, m_radius) };
    const qreal radiusBottomLeft  { bestOf (m_bottomLeftRadius, m_radius) };
    const qreal radiusBottomRight { bestOf (m_bottomRightRadius, m_radius) };
    const qreal strokeTop         { bestOf (m_topBorder, m_border) };
    const qreal strokeLeft        { bestOf (m_leftBorder, m_border) };
    const qreal strokeRight       { bestOf (m_rightBorder, m_border) };
    const qreal strokeBottom      { bestOf (m_bottomBorder, m_border) };
    const qreal outerMinX         { 0.0 };
    const qreal outerMinY         { 0.0 };
    const qreal outerMaxX         { width  () };
    const qreal outerMaxY         { height () };
    const qreal innerMinX         { (outerMinX + strokeLeft) };
    const qreal innerMinY         { (outerMinY + strokeTop) };
    const qreal innerMaxX         { (outerMaxX - strokeRight) };
    const qreal innerMaxY         { (outerMaxY - strokeBottom) };
    QVector<QPointF> fillPointsList   { };
    QVector<QPointF> strokePointsList { };
    if (radiusTopLeft > 0.0) {
        generateCorner (radiusTopLeft,
                        (outerMinX + radiusTopLeft), (outerMinY + radiusTopLeft),
                        strokeLeft, strokeTop,
                        180, 270,
                        strokePointsList, fillPointsList);
    }
    else {
        strokePointsList.append (QPointF { outerMinX, outerMinY });
        fillPointsList.append   (QPointF { innerMinX, innerMinY });
    }
    if (radiusTopRight > 0.0) {
        generateCorner (radiusTopRight,
                        (outerMaxX - radiusTopRight), (outerMinY + radiusTopRight),
                        strokeRight, strokeTop,
                        270, 360,
                        strokePointsList, fillPointsList);
    }
    else {
        strokePointsList.append (QPointF { outerMaxX, outerMinY });
        fillPointsList.append   (QPointF { innerMaxX, innerMinY });
    }
    if (radiusBottomRight > 0.0) {
        generateCorner (radiusBottomRight,
                        (outerMaxX - radiusBottomRight), (outerMaxY - radiusBottomRight),
                        strokeRight, strokeBottom,
                        0, 90,
                        strokePointsList, fillPointsList);
    }
    else {
        strokePointsList.append (QPointF { outerMaxX, outerMaxY });
        fillPointsList.append   (QPointF { innerMaxX, innerMaxY });
    }
    if (radiusBottomLeft > 0.0) {
        generateCorner (radiusBottomLeft,
                        (outerMinX + radiusBottomLeft), (outerMaxY - radiusBottomLeft),
                        strokeLeft, strokeBottom,
                        90, 180,
                        strokePointsList, fillPointsList);
    }
    else {
        strokePointsList.append (QPointF { outerMinX, outerMaxY });
        fillPointsList.append   (QPointF { innerMinX, innerMaxY });
    }
    QSGNode * ret { new QSGNode };
    /// STROKE GEOMETRY
    if (m_foreground != nullptr) {
        if (isSignificant (width ()) ||
            isSignificant (height ()) ||
            isSignificant (strokeTop) ||
            isSignificant (strokeLeft) ||
            isSignificant (strokeRight) ||
            isSignificant (strokeBottom)) {
            QSGGeometry * strokeGeometry { new QSGGeometry { QSGGeometry::defaultAttributes_Point2D (), (fillPointsList.count () + strokePointsList.count () + 2) } };
            strokeGeometry->setDrawingMode (QSGGeometry::DrawTriangleStrip);
            QSGGeometry::Point2D * strokeVertex (strokeGeometry->vertexDataAsPoint2D ());
            for (QVector<QPointF>::const_iterator itOut { strokePointsList.constBegin () }, itIn { fillPointsList.constBegin () }, endOut { strokePointsList.constEnd () }, endIn { fillPointsList.constEnd () }; itOut != endOut && itIn != endIn; ++itOut, ++itIn) {
                strokeVertex->set (float (itOut->x ()), float (itOut->y ()));
                ++strokeVertex;
                strokeVertex->set (float (itIn->x ()), float (itIn->y ()));
                ++strokeVertex;
            }
            strokeVertex->set (float (strokePointsList.constFirst ().x ()), float (strokePointsList.constFirst ().y ()));
            ++strokeVertex;
            strokeVertex->set (float (fillPointsList.constFirst ().x ()), float (fillPointsList.constFirst ().y ()));
            ++strokeVertex;
            ret->appendChildNode (m_foreground->createNodes (strokeGeometry, QRectF {
                                                                 QPointF { outerMinX, outerMinY },
                                                                 QPointF { outerMaxX, outerMaxY },
                                                             }));
        }
    }
    /// FILL
    if (m_background != nullptr) {
        if (isSignificant (width ()) ||
            isSignificant (height ())) {
            QSGGeometry * fillGeometry { new QSGGeometry { QSGGeometry::defaultAttributes_Point2D (), fillPointsList.count () } };
            fillGeometry->setDrawingMode (QSGGeometry::DrawTriangleFan);
            QSGGeometry::Point2D * fillVertex (fillGeometry->vertexDataAsPoint2D ());
            for (const QPointF & point : qAsConst (fillPointsList)) {
                fillVertex->set (float (point.x ()), float (point.y ()));
                ++fillVertex;
            }
            ret->appendChildNode (m_background->createNodes (fillGeometry, QRectF {
                                                                 QPointF { innerMinX, innerMinY },
                                                                 QPointF { innerMaxX, innerMaxY },
                                                             }));
        }
    }
    return ret;
}
