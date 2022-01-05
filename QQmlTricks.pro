
TEMPLATE = lib

QT += core gui qml quick svg

QML_IMPORT_PATH = $$PWD/imports

INCLUDEPATH += \
    $$PWD \
    $$PWD/core/macros \
    $$PWD/core/models \
    $$PWD/core/helpers \
    $$PWD/gui/containers \
    $$PWD/gui/helpers \
    $$PWD/gui/shapes

HEADERS += \
    $$PWD/QQmlTricks.h \
    $$PWD/core/helpers/QQmlFsSingleton.h \
    $$PWD/core/helpers/QQmlIntrospector.h \
    $$PWD/core/helpers/QQmlMimeIconsHelper.h \
    $$PWD/core/macros/QQmlListPropertyHelper.h \
    $$PWD/core/macros/QmlEnumHelpers.h \
    $$PWD/core/macros/QmlPropertyHelpers.h \
    $$PWD/core/models/QQmlFastObjectListModel.h \
    $$PWD/core/models/QQmlObjectListModel.h \
    $$PWD/core/models/QQmlVariantListModel.h \
    $$PWD/gui/containers/QQmlContainerEnums.h \
    $$PWD/gui/containers/QQuickAbstractContainerBase.h \
    $$PWD/gui/containers/QQuickColumnContainer.h \
    $$PWD/gui/containers/QQuickContainerAttachedObject.h \
    $$PWD/gui/containers/QQuickFastObjectListView.h \
    $$PWD/gui/containers/QQuickFormContainer.h \
    $$PWD/gui/containers/QQuickGridContainer.h \
    $$PWD/gui/containers/QQuickRowContainer.h \
    $$PWD/gui/containers/QQuickStackContainer.h \
    $$PWD/gui/containers/QQuickTableContainer.h \
    $$PWD/gui/helpers/QQuickExtraAnchors.h \
    $$PWD/gui/helpers/QQuickSvgIconHelper.h \
    $$PWD/gui/helpers/QQuickWindowIconHelper.h \
    $$PWD/gui/shapes/QQuickRoundedRectangleItem.h

SOURCES += \
    $$PWD/QQmlTricks.cpp \
    $$PWD/core/helpers/QQmlFsSingleton.cpp \
    $$PWD/core/helpers/QQmlIntrospector.cpp \
    $$PWD/core/helpers/QQmlMimeIconsHelper.cpp \
    $$PWD/gui/containers/QQuickAbstractContainerBase.cpp \
    $$PWD/gui/containers/QQuickColumnContainer.cpp \
    $$PWD/gui/containers/QQuickContainerAttachedObject.cpp \
    $$PWD/gui/containers/QQuickFastObjectListView.cpp \
    $$PWD/gui/containers/QQuickFormContainer.cpp \
    $$PWD/gui/containers/QQuickGridContainer.cpp \
    $$PWD/gui/containers/QQuickRowContainer.cpp \
    $$PWD/gui/containers/QQuickStackContainer.cpp \
    $$PWD/gui/containers/QQuickTableContainer.cpp \
    $$PWD/gui/helpers/QQuickExtraAnchors.cpp \
    $$PWD/gui/helpers/QQuickSvgIconHelper.cpp \
    $$PWD/gui/helpers/QQuickWindowIconHelper.cpp \
    $$PWD/gui/shapes/QQuickRoundedRectangleItem.cpp

RESOURCES += \
    $$PWD/qtqmltricks_svgicons_actions.qrc \
    $$PWD/qtqmltricks_svgicons_devices.qrc \
    $$PWD/qtqmltricks_svgicons_filetypes.qrc \
    $$PWD/qtqmltricks_svgicons_others.qrc \
    $$PWD/qtqmltricks_svgicons_services.qrc \
    $$PWD/qtqmltricks_uielements.qrc

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md
