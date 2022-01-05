#ifndef QQUICKABSTRACTCONTAINERBASE_H
#define QQUICKABSTRACTCONTAINERBASE_H

#include <QQuickItem>

#include "QmlPropertyHelpers.h"

#include "QQmlContainerEnums.h"

class QQuickContainerAttachedObject;

class QQuickAbstractContainerBase : public QQuickItem {
    Q_OBJECT
    QML_READONLY_VAR_PROPERTY (layoutItemsCount, int)

public:
    explicit QQuickAbstractContainerBase (QQuickItem * parent = Q_NULLPTR);
    virtual ~QQuickAbstractContainerBase (void);

    Q_INVOKABLE void doLayout (void);

signals:
    void layoutDone (void);

protected:
    virtual void setupHandlers (void) = 0;
    virtual void relayout      (void) = 0;

    static const char * REPEATER_CLASSNAME;

    QQuickContainerAttachedObject * getContainerAttachedObject (QQuickItem * item) const;

private:
    void classBegin        (void) Q_DECL_FINAL;
    void componentComplete (void) Q_DECL_FINAL;
    void updatePolish      (void) Q_DECL_FINAL;
    void itemChange        (ItemChange change, const ItemChangeData & value) Q_DECL_FINAL;
};

#endif // QQUICKABSTRACTCONTAINERBASE_H
