
#include "QQuickTableContainer.h"

#include <QPoint>
#include <QHash>
#include <QtMath>

QQmlTableAttachedObject::QQmlTableAttachedObject (QObject * parent)
    : QObject  { parent }
    , m_row    { -1 }
    , m_col    { -1 }
    , m_rowPos { QQmlTableCellPosV::TOP }
    , m_colPos { QQmlTableCellPosH::LEFT }
{ }

QQmlTableAttachedObject::~QQmlTableAttachedObject (void) { }

QQmlTableAttachedObject * QQmlTableAttachedObject::qmlAttachedProperties (QObject * object) {
    return new QQmlTableAttachedObject { object };
}

QQmlTableDivision::QQmlTableDivision (QObject * parent)
    : QObject           { parent }
    , m_fixedSize       { -1 }
    , m_stretchFactor   { 0 }
    , m_implicitSize    { 0 }
    , m_optimalSize     { 0 }
    , m_actualSize      { 0 }
    , m_beginPos        { 0 }
    , m_endPos          { 0 }
    , m_tmpImplicitSize { 0 }
{ }

QQmlTableDivision::~QQmlTableDivision (void) { }

QQuickTableContainer::QQuickTableContainer (QQuickItem * parent)
    : QQuickItem   { parent }
    , m_rowSpacing { 0 }
    , m_colSpacing { 0 }
    , m_rows       { this, 10 }
    , m_cols       { this, 10 }
{ }

QQuickTableContainer::~QQuickTableContainer (void) { }

void QQuickTableContainer::classBegin (void) {
    QQuickItem::classBegin ();
}

void QQuickTableContainer::componentComplete (void) {
    QQuickItem::componentComplete ();
    connect (this, &QQuickTableContainer::widthChanged,      this, &QQuickTableContainer::polish);
    connect (this, &QQuickTableContainer::heightChanged,     this, &QQuickTableContainer::polish);
    connect (this, &QQuickTableContainer::visibleChanged,    this, &QQuickTableContainer::polish);
    connect (this, &QQuickTableContainer::rowSpacingChanged, this, &QQuickTableContainer::polish);
    connect (this, &QQuickTableContainer::colSpacingChanged, this, &QQuickTableContainer::polish);
    for (QQmlTableDivision * conf : m_rows) {
        connect (conf, &QQmlTableDivision::fixedSizeChanged,     this, &QQuickTableContainer::polish);
        connect (conf, &QQmlTableDivision::stretchFactorChanged, this, &QQuickTableContainer::polish);
    }
    for (QQmlTableDivision * conf : m_cols) {
        connect (conf, &QQmlTableDivision::fixedSizeChanged,     this, &QQuickTableContainer::polish);
        connect (conf, &QQmlTableDivision::stretchFactorChanged, this, &QQuickTableContainer::polish);
    }
    polish ();
}

void QQuickTableContainer::itemChange (ItemChange changeType, const ItemChangeData & changeData) {
    QQuickItem::itemChange (changeType, changeData);
    switch (int (changeType)) {
        case ItemChildAddedChange: {
            if (QQuickItem * child = { changeData.item }) {
                connect (child, &QQuickItem::visibleChanged,        this, &QQuickTableContainer::polish);
                connect (child, &QQuickItem::implicitWidthChanged,  this, &QQuickTableContainer::polish);
                connect (child, &QQuickItem::implicitHeightChanged, this, &QQuickTableContainer::polish);
                if (const QQmlTableAttachedObject * attached { getTableAttachedObject (child) }) {
                    connect (attached, &QQmlTableAttachedObject::rowChanged,    this, &QQuickTableContainer::polish);
                    connect (attached, &QQmlTableAttachedObject::colChanged,    this, &QQuickTableContainer::polish);
                    connect (attached, &QQmlTableAttachedObject::rowPosChanged, this, &QQuickTableContainer::polish);
                    connect (attached, &QQmlTableAttachedObject::colPosChanged, this, &QQuickTableContainer::polish);
                }
                polish ();
            }
            break;
        }
        case ItemChildRemovedChange: {
            if (QQuickItem * child = { changeData.item }) {
                disconnect (child, Q_NULLPTR, this, Q_NULLPTR);
                if (const QQmlTableAttachedObject * attached { getTableAttachedObject (child) }) {
                    disconnect (attached, Q_NULLPTR, this, Q_NULLPTR);
                }
                polish ();
            }
            break;
        }
    }
}

QQmlTableAttachedObject * QQuickTableContainer::getTableAttachedObject (QQuickItem * item) const {
    return qobject_cast<QQmlTableAttachedObject *> (qmlAttachedPropertiesObject<QQmlTableAttachedObject> (item, false));
}

void QQuickTableContainer::updatePolish (void) {
    QQuickItem::updatePolish ();
    const QList<QQuickItem *> & allItems { childItems () };
    QVector<QQmlTableAttachedObject *> attachedItems { };
    attachedItems.reserve (allItems.count ());
    for (QQmlTableDivision * col : m_cols) {
        col->beginComputeImplicitSize ();
    }
    for (QQmlTableDivision * row : m_rows) {
        row->beginComputeImplicitSize ();
    }
    for (QQuickItem * item : allItems) {
        if (item->isVisible ()) {
            if (QQmlTableAttachedObject * attached = { getTableAttachedObject (item) }) {
                if (QQmlTableDivision * col = m_cols [attached->get_col ()]) {
                    if (QQmlTableDivision * row = m_rows [attached->get_row ()]) {
                        col->doComputeImplicitSize (qCeil (item->implicitWidth  ()));
                        row->doComputeImplicitSize (qCeil (item->implicitHeight ()));
                        attachedItems.append (attached);
                    }
                }
            }
        }
    }
    if (!attachedItems.isEmpty () && m_cols.length () > 0 && m_rows.length () > 0) {
        const int totalSpacingCols { ((m_cols.length () -1) * m_colSpacing) };
        const int totalSpacingRows { ((m_rows.length () -1) * m_rowSpacing) };
        int totalWidthFixed    { 0 };
        int totalHeightFixed   { 0 };
        int totalWidthAuto     { 0 };
        int totalHeightAuto    { 0 };
        int totalWidthStretch  { 0 };
        int totalHeightStretch { 0 };
        int countStretchCols   { 0 };
        int countStretchRows   { 0 };
        for (QQmlTableDivision * col : m_cols) {
            col->endComputeImplicitSize ();
            if (col->get_fixedSize () > 0) {
                col->set_optimalSize (col->get_fixedSize ());
                totalWidthFixed += col->get_optimalSize ();
            }
            else {
                col->set_optimalSize (col->get_implicitSize ());
                if (col->get_stretchFactor () > 0) {
                    totalWidthStretch += col->get_optimalSize ();
                    countStretchCols += col->get_stretchFactor ();
                }
                else {
                    totalWidthAuto += col->get_optimalSize ();
                }
            }
        }
        for (QQmlTableDivision * row : m_rows) {
            row->endComputeImplicitSize ();
            if (row->get_fixedSize () > 0) {
                row->set_optimalSize (row->get_fixedSize ());
                totalHeightFixed += row->get_optimalSize ();
            }
            else {
                row->set_optimalSize (row->get_implicitSize ());
                if (row->get_stretchFactor () > 0) {
                    totalHeightStretch += row->get_optimalSize ();
                    countStretchRows += row->get_stretchFactor ();
                }
                else {
                    totalHeightAuto += row->get_optimalSize ();
                }
            }
        }
        qreal maxRatioX { 1.0 };
        qreal maxRatioY { 1.0 };
        for (QQmlTableDivision * col : m_cols) {
            if (col->get_stretchFactor () > 0) {
                const int theoricalWidth { qFloor (totalWidthStretch * qreal (col->get_stretchFactor ()) / qreal (countStretchCols)) };
                const qreal ratioX { (qreal (col->get_optimalSize ()) / qreal (theoricalWidth)) };
                if (ratioX > maxRatioX) {
                    maxRatioX = ratioX;
                }
            }
        }
        for (QQmlTableDivision * row : m_rows) {
            if (row->get_stretchFactor () > 0) {
                const int theoricalHeight { qFloor (totalHeightStretch * qreal (row->get_stretchFactor ()) / qreal (countStretchRows)) };
                const qreal ratioY { (qreal (row->get_optimalSize ()) / qreal (theoricalHeight)) };
                if (ratioY > maxRatioY) {
                    maxRatioY = ratioY;
                }
            }
        }
        setImplicitWidth  (totalWidthAuto  + totalWidthFixed  + (totalWidthStretch * maxRatioX)  + totalSpacingCols);
        setImplicitHeight (totalHeightAuto + totalHeightFixed + totalHeightStretch + totalSpacingRows);
        const qreal widthForStretched  { (width  () - totalWidthFixed  - totalWidthAuto  - totalSpacingCols) };
        const qreal heightForStretched { (height () - totalHeightFixed - totalHeightAuto - totalSpacingRows) };
        int posX { 0 };
        int posY { 0 };
        for (QQmlTableDivision * col : m_cols) {
            if (col->get_stretchFactor () > 0) {
                col->set_actualSize (qFloor (widthForStretched * qreal (col->get_stretchFactor ()) / qreal (countStretchCols)));
            }
            else {
                col->set_actualSize (col->get_optimalSize ());
            }
            if (posX > 0) {
                posX += m_colSpacing;
            }
            col->set_beginPos (posX);
            posX += col->get_actualSize ();
            col->set_endPos (posX);
        }
        for (QQmlTableDivision * row : m_rows) {
            if (row->get_stretchFactor () > 0) {
                row->set_actualSize (qFloor (heightForStretched * qreal (row->get_stretchFactor ()) / qreal (countStretchRows)));
            }
            else {
                row->set_actualSize (row->get_optimalSize ());
            }
            if (posY > 0) {
                posY += m_rowSpacing;
            }
            row->set_beginPos (posY);
            posY += row->get_actualSize ();
            row->set_endPos (posY);
        }
        for (QQmlTableAttachedObject * attached : attachedItems) {
            if (QQuickItem * item = qobject_cast<QQuickItem *> (attached->parent ())) {
                if (QQmlTableDivision * col = m_cols [attached->get_col ()]) {
                    if (QQmlTableDivision * row = m_rows [attached->get_row ()]) {
                        switch (attached->get_colPos ()) {
                            case QQmlTableCellPosH::LEFT: {
                                item->setWidth (item->implicitWidth ());
                                item->setX     (col->get_beginPos ());
                                break;
                            }
                            case QQmlTableCellPosH::RIGHT: {
                                item->setWidth (item->implicitWidth ());
                                item->setX     (col->get_endPos () - item->width ());
                                break;
                            }
                            case QQmlTableCellPosH::H_FILL: {
                                item->setWidth (col->get_actualSize ());
                                item->setX     (col->get_beginPos ());
                                break;
                            }
                            case QQmlTableCellPosH::H_CENTER: {
                                item->setWidth (item->implicitWidth ());
                                item->setX     (col->get_beginPos () + (col->get_actualSize () - item->width ()) / 2);
                                break;
                            }
                        }
                        switch (attached->get_rowPos ()) {
                            case QQmlTableCellPosV::TOP: {
                                item->setHeight (item->implicitHeight ());
                                item->setY      (row->get_beginPos ());
                                break;
                            }
                            case QQmlTableCellPosV::BOTTOM: {
                                item->setHeight (item->implicitHeight ());
                                item->setY      (row->get_endPos () - item->height ());
                                break;
                            }
                            case QQmlTableCellPosV::V_FILL: {
                                item->setHeight (row->get_actualSize ());
                                item->setY      (row->get_beginPos ());
                                break;
                            }
                            case QQmlTableCellPosV::V_CENTER: {
                                item->setHeight (item->implicitHeight ());
                                item->setY      (row->get_beginPos () + (row->get_actualSize () - item->height ()) / 2);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}
