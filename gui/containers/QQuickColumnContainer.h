#ifndef QQUICKCOLUMNCONTAINER_H
#define QQUICKCOLUMNCONTAINER_H

#include "QQuickAbstractContainerBase.h"

class QQuickColumnContainer : public QQuickAbstractContainerBase {
    Q_OBJECT
    QML_WRITABLE_VAR_PROPERTY (verticalSpacing,   int)
    QML_WRITABLE_VAR_PROPERTY (verticalDirection, int) // VerticalDirections::Type

    // COMPATIBILITY ALIASES
    Q_PROPERTY (int spacing READ get_verticalSpacing WRITE set_verticalSpacing NOTIFY verticalSpacingChanged)

public:
    explicit QQuickColumnContainer (QQuickItem * parent = Q_NULLPTR);
    virtual ~QQuickColumnContainer (void);

protected:
    void setupHandlers (void) Q_DECL_FINAL;
    void relayout      (void) Q_DECL_FINAL;
};

#endif // QQUICKCOLUMNCONTAINER_H
