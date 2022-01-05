
#include "QQmlTricks.h"

#include "QQmlContainerEnums.h"
#include "QQmlFastObjectListModel.h"
#include "QQmlObjectListModel.h"
#include "QQuickColumnContainer.h"
#include "QQuickContainerAttachedObject.h"
#include "QQuickExtraAnchors.h"
#include "QQuickFastObjectListView.h"
#include "QQuickFormContainer.h"
#include "QQuickGridContainer.h"
#include "QQuickRowContainer.h"
#include "QQuickStackContainer.h"
#include "QQuickTableContainer.h"
#include "QQuickSvgIconHelper.h"
#include "QQuickRoundedRectangleItem.h"
#include "QQmlMimeIconsHelper.h"
#include "QQuickWindowIconHelper.h"
#include "QQmlIntrospector.h"
#include "QQmlFsSingleton.h"
#include "QQmlVariantListModel.h"

#include <QtQml>
void QQmlTricks::registerComponents (QQmlEngine * engine) {
    static const QString ERR_ENUM_CLASS    { QStringLiteral ("Enum-class !") };
    static const QString ERR_ATTACHED_OBJ  { QStringLiteral ("Attached-object class !") };
    static const QString ERR_ABSTRACT_BASE { QStringLiteral ("Abstract base class !") };
    qmlRegisterType<QQuickColumnContainer>                    ("QQmlTricks", 3, 0, "ColumnContainer");
    qmlRegisterType<QQuickGridContainer>                      ("QQmlTricks", 3, 0, "GridContainer");
    qmlRegisterType<QQuickStackContainer>                     ("QQmlTricks", 3, 0, "StackContainer");
    qmlRegisterType<QQuickFormContainer>                      ("QQmlTricks", 3, 0, "FormContainer");
    qmlRegisterType<QQuickRowContainer>                       ("QQmlTricks", 3, 0, "RowContainer");
    qmlRegisterType<QQuickTableContainer>                     ("QQmlTricks", 3, 0, "TableContainer");
    qmlRegisterType<QQmlTableDivision>                        ("QQmlTricks", 3, 0, "TableDiv");
    qmlRegisterUncreatableType<QQmlTableCellPosH>             ("QQmlTricks", 3, 0, "TableCellPosH",            ERR_ENUM_CLASS);
    qmlRegisterUncreatableType<QQmlTableCellPosV>             ("QQmlTricks", 3, 0, "TableCellPosV",            ERR_ENUM_CLASS);
    qmlRegisterUncreatableType<QQmlTableAttachedObject>       ("QQmlTricks", 3, 0, "TableCell",                ERR_ATTACHED_OBJ);
    qmlRegisterUncreatableType<VerticalDirections>            ("QQmlTricks", 3, 0, "VerticalDirections",       ERR_ENUM_CLASS);
    qmlRegisterUncreatableType<HorizontalDirections>          ("QQmlTricks", 3, 0, "HorizontalDirections",     ERR_ENUM_CLASS);
    qmlRegisterUncreatableType<FlowDirections>                ("QQmlTricks", 3, 0, "FlowDirections",           ERR_ENUM_CLASS);
    qmlRegisterUncreatableType<QQuickExtraAnchors>            ("QQmlTricks", 3, 0, "ExtraAnchors",             ERR_ATTACHED_OBJ);
    qmlRegisterUncreatableType<QQuickContainerAttachedObject> ("QQmlTricks", 3, 0, "Container",                ERR_ATTACHED_OBJ);
    qmlRegisterUncreatableType<QQmlObjectListModelBase>       ("QQmlTricks", 3, 0, "ObjectListModel",          ERR_ABSTRACT_BASE);
    qmlRegisterUncreatableType<QQmlVariantListModelBase>      ("QQmlTricks", 3, 0, "VariantListModel",         ERR_ABSTRACT_BASE);
    qmlRegisterUncreatableType<QQmlFastObjectListModelBase>   ("QQmlTricks", 3, 0, "FastObjectListModel",      ERR_ABSTRACT_BASE);
    qmlRegisterUncreatableType<QQmlAbstractMaterial>          ("QQmlTricks", 3, 0, "AbstractMaterial",         ERR_ABSTRACT_BASE);
    qmlRegisterUncreatableType<QQmlAbstractGradientMaterial>  ("QQmlTricks", 3, 0, "AbstractGradientMaterial", ERR_ABSTRACT_BASE);
    qmlRegisterType<QQuickFastObjectListView>                 ("QQmlTricks", 3, 0, "FastObjectListView");
    qmlRegisterType<QQmlFlatColorMaterial>                    ("QQmlTricks", 3, 0, "FlatColorMaterial");
    qmlRegisterType<QQmlVerticalGradientMaterial>             ("QQmlTricks", 3, 0, "VerticalGradientMaterial");
    qmlRegisterType<QQmlHorizontalGradientMaterial>           ("QQmlTricks", 3, 0, "HorizontalGradientMaterial");
    qmlRegisterType<QQuickRoundedRectangleItem>               ("QQmlTricks", 3, 0, "RoundedRectangle");
    qmlRegisterType<QQuickSvgIconHelper>                      ("QQmlTricks", 3, 0, "SvgIconHelper");
    qmlRegisterType<QQmlMimeIconsHelper>                      ("QQmlTricks", 3, 0, "MimeIconsHelper");
    qmlRegisterType<QQuickGroupItem>                          ("QQmlTricks", 3, 0, "Group");
    qmlRegisterType<QQuickWindowIconHelper>                   ("QQmlTricks", 3, 0, "WindowIconHelper");
    qmlRegisterType<QQmlFileSystemModelEntry>                 ("QQmlTricks", 3, 0, "FileSystemModelEntry");
    qmlRegisterSingletonType<QQmlFileSystemSingleton>         ("QQmlTricks", 3, 0, "FileSystem",   &QQmlFileSystemSingleton::qmlSingletonProvider);
    qmlRegisterSingletonType<QQmlIntrospector>                ("QQmlTricks", 3, 0, "Introspector", &QQmlIntrospector::qmlSingletonProvider);
    QQuickSvgIconHelper::setBasePath (":/QQmlTricks/icons");
    if (engine != Q_NULLPTR) {
        engine->addImportPath ("qrc:///imports");
    }
    else {
        qWarning () << "You didn't pass a QML engine to the register function,"
                    << "some features (mostly plain QML components, and icon theme provider) won't work !";
    }
}
