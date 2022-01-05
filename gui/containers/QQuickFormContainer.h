#ifndef QQUICKFORMCONTAINER_H
#define QQUICKFORMCONTAINER_H

#include "QQuickAbstractContainerBase.h"

class QQuickFormContainer : public QQuickAbstractContainerBase {
    Q_OBJECT
    QML_WRITABLE_VAR_PROPERTY (verticalSpacing,   int)
    QML_WRITABLE_VAR_PROPERTY (horizontalSpacing, int)

    // COMPATIBILITY ALIASES
    Q_PROPERTY (int colSpacing READ get_horizontalSpacing WRITE set_horizontalSpacing NOTIFY horizontalSpacingChanged)
    Q_PROPERTY (int rowSpacing READ get_verticalSpacing   WRITE set_verticalSpacing   NOTIFY verticalSpacingChanged)

public:
    explicit QQuickFormContainer (QQuickItem * parent = Q_NULLPTR);

protected:
    void setupHandlers (void) Q_DECL_FINAL;
    void relayout      (void) Q_DECL_FINAL;
};

#endif // QQUICKFORMCONTAINER_H
