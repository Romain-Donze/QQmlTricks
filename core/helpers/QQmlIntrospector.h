#ifndef QQMLINTROSPECTOR_H
#define QQMLINTROSPECTOR_H

#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQmlComponent>
#include <QQmlEngine>

#include "QmlPropertyHelpers.h"

class QQuickGroupItem : public QQuickItem {
    Q_OBJECT
    QML_WRITABLE_CSTREF_PROPERTY (title, QString)
    QML_WRITABLE_PTR_PROPERTY    (icon, QQmlComponent)

public:
    explicit QQuickGroupItem (QQuickItem * parent = Q_NULLPTR);
    virtual ~QQuickGroupItem (void);
};

class QQmlIntrospector : public QObject {
    Q_OBJECT

public:
    explicit QQmlIntrospector (QObject * parent = Q_NULLPTR);

    static QObject * qmlSingletonProvider (QQmlEngine * qmlEngine, QJSEngine * jsEngine);

    Q_INVOKABLE bool isGroupItem (QObject * object);
    Q_INVOKABLE QQuickWindow * window (QQuickItem * item);


    Q_INVOKABLE bool inherits (QObject * object, QObject * reference);

};

#endif // QQMLINTROSPECTOR_H
