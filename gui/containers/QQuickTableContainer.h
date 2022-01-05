#ifndef QQUICKTABLECONTAINER_H
#define QQUICKTABLECONTAINER_H

#include <QObject>
#include <QQuickItem>

#include "QmlEnumHelpers.h"
#include "QmlPropertyHelpers.h"
#include "QQmlListPropertyHelper.h"

QML_ENUM_CLASS (QQmlTableCellPosV,
                TOP,
                BOTTOM,
                V_FILL,
                V_CENTER,
                )

QML_ENUM_CLASS (QQmlTableCellPosH,
                LEFT,
                RIGHT,
                H_FILL,
                H_CENTER,
                )

class QQmlTableAttachedObject : public QObject {
    Q_OBJECT
    QML_WRITABLE_VAR_PROPERTY (row,    int) // idx
    QML_WRITABLE_VAR_PROPERTY (col,    int) // idx
    QML_WRITABLE_VAR_PROPERTY (rowPos, int) // enum QQmlTableCellPosV::Type
    QML_WRITABLE_VAR_PROPERTY (colPos, int) // enum QQmlTableCellPosH::Type

public:
    explicit QQmlTableAttachedObject (QObject * parent = nullptr);
    virtual ~QQmlTableAttachedObject (void);

    static QQmlTableAttachedObject * qmlAttachedProperties (QObject * object);
};

class QQmlTableDivision : public QObject {
    Q_OBJECT
    QML_WRITABLE_VAR_PROPERTY (fixedSize,     int) // px
    QML_WRITABLE_VAR_PROPERTY (stretchFactor, int) // int factor
    QML_READONLY_VAR_PROPERTY (implicitSize,  int) // px
    QML_READONLY_VAR_PROPERTY (optimalSize,   int) // px
    QML_READONLY_VAR_PROPERTY (actualSize,    int) // px
    QML_READONLY_VAR_PROPERTY (beginPos,      int) // px
    QML_READONLY_VAR_PROPERTY (endPos,        int) // px

public:
    explicit QQmlTableDivision (QObject * parent = nullptr);
    virtual ~QQmlTableDivision (void);

    inline void beginComputeImplicitSize (void) {
        m_tmpImplicitSize = 0;
    }
    inline void doComputeImplicitSize (const int size) {
        if (size > m_tmpImplicitSize) {
            m_tmpImplicitSize = size;
        }
    }
    inline void endComputeImplicitSize (void) {
        set_implicitSize (m_tmpImplicitSize);
    }

private:
    int m_tmpImplicitSize;
};

class QQuickTableContainer : public QQuickItem {
    Q_OBJECT
    QML_WRITABLE_VAR_PROPERTY (rowSpacing, int)
    QML_WRITABLE_VAR_PROPERTY (colSpacing, int)
    QML_LIST_PROPERTY         (rows, QQmlTableDivision)
    QML_LIST_PROPERTY         (cols, QQmlTableDivision)

public:
    explicit QQuickTableContainer (QQuickItem * parent = nullptr);
    virtual ~QQuickTableContainer (void);

protected:
    void classBegin        (void) Q_DECL_FINAL;
    void componentComplete (void) Q_DECL_FINAL;
    void updatePolish      (void) Q_DECL_FINAL;
    void itemChange        (ItemChange changeType, const ItemChangeData & changeData) Q_DECL_FINAL;

private:
    QQmlTableAttachedObject * getTableAttachedObject (QQuickItem * item) const;
};

QML_DECLARE_TYPE     (QQmlTableAttachedObject)
QML_DECLARE_TYPEINFO (QQmlTableAttachedObject, QML_HAS_ATTACHED_PROPERTIES)

#endif // QQUICKTABLECONTAINER_H
