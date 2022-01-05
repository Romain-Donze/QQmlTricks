#ifndef QQUICKFASTOBJECTLISTVIEW_H
#define QQUICKFASTOBJECTLISTVIEW_H

#include <QQuickItem>
#include <QQmlComponent>
#include <QDateTime>
#include <QPropertyAnimation>
#include <QEasingCurve>

#include "QmlPropertyHelpers.h"

class QQmlFastObjectListModelBase;

class QQuickFastObjectListView : public QQuickItem {
    Q_OBJECT
    QML_WRITABLE_PTR_PROPERTY (current, QObject)
    QML_WRITABLE_VAR_PROPERTY (behavior,    int)
    QML_WRITABLE_VAR_PROPERTY (spaceBefore, int)
    QML_WRITABLE_VAR_PROPERTY (spaceAfter,  int)

    Q_PROPERTY (QQmlFastObjectListModelBase * model    READ getModel    WRITE setModel    NOTIFY modelChanged)
    Q_PROPERTY (QQmlComponent               * delegate READ getDelegate WRITE setDelegate NOTIFY delegateChanged)

    struct Holder {
        QObject     * modelItem        { Q_NULLPTR };
        QQuickItem  * delegateInstance { Q_NULLPTR };
        QQmlContext * context          { Q_NULLPTR };
    };

public:
    explicit QQuickFastObjectListView (QQuickItem * parent = Q_NULLPTR);
    virtual ~QQuickFastObjectListView (void);

    enum Behavior {
        FREE_MOVE = 0,
        KEEP_AT_TOP,
        KEEP_AT_BOTTOM,
        KEEP_CENTERED,
    };
    Q_ENUM (Behavior)

    QQmlFastObjectListModelBase * getModel    (void) const;
    QQmlComponent               * getDelegate (void) const;

    void setModel    (QQmlFastObjectListModelBase * model);
    void setDelegate (QQmlComponent               * delegate);

signals:
    void modelChanged    (void);
    void delegateChanged (void);

protected slots:
    void doMarkDirty        (void);
    void doResetInstances   (void);
    void doPrepareInstances (void);
    void doAttachToModel    (void);
    void doDetachFromModel  (void);
    void doChangePositionY  (const int posY);
    void doInstantiate      (Holder * holder);
    void doEliminate        (Holder * holder);
    void onItemInserted     (QObject * item, const int idx);
    void onItemRemoved      (QObject * item, const int idx);
    void onItemsCleared     (void);
    void onUserInteraction  (void);

protected:
    void classBegin        (void) Q_DECL_FINAL;
    void componentComplete (void) Q_DECL_FINAL;
    void updatePolish      (void) Q_DECL_FINAL;

private:
    const QString MODEL_ITEM { "modelItem" };
    const char *  FLICKABLE  { "QQuickFlickable" };
    const char *  FLICKING_V { "flickingVertically" };
    const char *  DRAGGING_V { "draggingVertically" };
    const char *  CONTENT_Y  { "contentY" };
    const char *  CONTENT_W  { "contentWidth" };
    const char *  CONTENT_H  { "contentHeight" };
    const char *  ANCHORS    { "anchors" };
    const char *  TOP        { "top" };
    const char *  LEFT       { "left" };
    const char *  RIGHT      { "right" };

    QQuickItem                   * m_flickable   { Q_NULLPTR };
    QQmlComponent                * m_delegate    { Q_NULLPTR };
    QQmlFastObjectListModelBase  * m_model       { Q_NULLPTR };
    QList<Holder *>                m_holdersList { };
};

#endif // QQUICKFASTOBJECTLISTVIEW_H
