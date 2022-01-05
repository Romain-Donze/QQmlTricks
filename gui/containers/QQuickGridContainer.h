#ifndef QQUICKGRIDCONTAINER_H
#define QQUICKGRIDCONTAINER_H

#include "QQuickAbstractContainerBase.h"

class QQuickGridContainer : public QQuickAbstractContainerBase {
    Q_OBJECT
    QML_WRITABLE_VAR_PROPERTY (cols,                int)
    QML_WRITABLE_VAR_PROPERTY (rows,                int)
    QML_WRITABLE_VAR_PROPERTY (capacity,            int)
    QML_WRITABLE_VAR_PROPERTY (verticalSpacing,     int)
    QML_WRITABLE_VAR_PROPERTY (horizontalSpacing,   int)
    QML_WRITABLE_VAR_PROPERTY (verticalDirection,   int) // VerticalDirections::Type
    QML_WRITABLE_VAR_PROPERTY (horizontalDirection, int) // HorizontalDirections::Type
    QML_WRITABLE_VAR_PROPERTY (flowDirection,       int) // FlowDirections::Type
    QML_WRITABLE_VAR_PROPERTY (fillEmpty,          bool)

    // COMPATIBILITY ALIASES
    Q_PROPERTY (int colSpacing READ get_horizontalSpacing WRITE set_horizontalSpacing NOTIFY horizontalSpacingChanged)
    Q_PROPERTY (int rowSpacing READ get_verticalSpacing   WRITE set_verticalSpacing   NOTIFY verticalSpacingChanged)

public:
    explicit QQuickGridContainer (QQuickItem * parent = Q_NULLPTR);

protected:
    void setupHandlers (void) Q_DECL_FINAL;
    void relayout      (void) Q_DECL_FINAL;
};

#endif // QQUICKGRIDCONTAINER_H
