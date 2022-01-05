import QtQuick 2.6;
import QQmlTricks 3.0;

FocusScope {
    id: base;
    implicitWidth: clicker.width;
    implicitHeight: clicker.height;
    Keys.onSpacePressed: { toggle (); }

    property bool value : false;
    property int  size  : (TricksStyle.spacingNormal * 2.5);

    signal edited ();

    function toggle () {
        if (enabled) {
            forceActiveFocus ();
            value = !value;
            edited ();
        }
    }

    MouseArea {
        id: clicker;
        width: size;
        height: size;
        enabled: base.enabled;
        anchors.centerIn: parent;
        onClicked: { base.toggle (); }

        Rectangle {
            id: rect;
            width: Math.round (parent.width);
            height: Math.round (parent.height);
            radius: TricksStyle.roundness;
            enabled: base.enabled;
            antialiasing: radius;
            gradient: (enabled ? TricksStyle.gradientEditable () : TricksStyle.gradientDisabled ());
            border {
                width: (base.activeFocus ? TricksStyle.lineSize * 2 : TricksStyle.lineSize);
                color: (base.activeFocus ? TricksStyle.colorSelection : TricksStyle.colorBorder);
            }
            anchors.fill: parent;
        }
        SymbolLoader {
            id: shape;
            size: base.size;
            color: (base.enabled ? TricksStyle.colorForeground : TricksStyle.colorBorder);
            symbol: TricksStyle.symbolCheck;
            visible: base.value;
            enabled: base.enabled;
            autoSize: false;
            anchors.fill: parent;
        }
    }
}
