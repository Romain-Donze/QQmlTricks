import QtQuick 2.6;
import QQmlTricks 3.0;

Loader {
    id: base;
    onInstanceChanged: {
        if (instance !== null) {
            instance.size = Qt.binding (function () {
                return base.size;
            });
            instance.color = Qt.binding (function () {
                return (base.enabled ? base.color : TricksStyle.colorBorder);
            });
            instance.width = Qt.binding (function () {
                return (base.autoSize ? instance.implicitWidth : base.width);
            });
            instance.height = Qt.binding (function () {
                return (base.autoSize ? instance.implicitHeight : base.height);
            });
        }
    }

    property int   size     : TricksStyle.fontSizeNormal;
    property color color    : TricksStyle.colorForeground;
    property alias symbol   : base.sourceComponent;
    property bool  autoSize : true;

    readonly property AbstractSymbol instance : item;
}
