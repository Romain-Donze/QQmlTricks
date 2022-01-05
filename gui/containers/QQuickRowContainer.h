#ifndef QQUICKROWCONTAINER_H
#define QQUICKROWCONTAINER_H

#include "QQuickAbstractContainerBase.h"

class QQuickRowContainer : public QQuickAbstractContainerBase {
    Q_OBJECT
    QML_WRITABLE_VAR_PROPERTY (horizontalSpacing,   int)
    QML_WRITABLE_VAR_PROPERTY (horizontalDirection, int) // HorizontalDirections::Type

    // COMPATIBILITY ALIASES
    Q_PROPERTY (int spacing READ get_horizontalSpacing WRITE set_horizontalSpacing NOTIFY horizontalSpacingChanged)

public:
    explicit QQuickRowContainer (QQuickItem * parent = Q_NULLPTR);
    virtual ~QQuickRowContainer (void);

protected:
    void setupHandlers (void) Q_DECL_FINAL;
    void relayout      (void) Q_DECL_FINAL;
};

#endif // QQUICKROWCONTAINER_H
