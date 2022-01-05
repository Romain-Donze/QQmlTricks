#ifndef QQUICKSTACKCONTAINER_H
#define QQUICKSTACKCONTAINER_H

#include "QQuickAbstractContainerBase.h"

class QQuickStackContainer : public QQuickAbstractContainerBase {
    Q_OBJECT

public:
    explicit QQuickStackContainer (QQuickItem * parent = Q_NULLPTR);

protected:
    void setupHandlers (void) Q_DECL_FINAL;
    void relayout      (void) Q_DECL_FINAL;
};

#endif // QQUICKSTACKCONTAINER_H
