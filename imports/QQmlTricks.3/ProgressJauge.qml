import QtQuick 2.6;
import QQmlTricks 3.0;

Item {
    id: base;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: 200;
    implicitHeight: barSize;

    property real value      : 0;
    property real minValue   : 0;
    property real maxValue   : 100;
    property real splitValue : (maxValue + minValue) / 2;
    property bool useSplit   : false;
    property int  barSize    : TricksStyle.spacingBig;
    property int  divisions  : 0;

    QtObject {
        id: priv;

        readonly property int posBegin : 0;
        readonly property int posEnd   : groove.width;
        readonly property int posValue : TricksStyle.convert (value, minValue, maxValue, posBegin, posEnd);
        readonly property int posSplit : TricksStyle.convert (splitValue, minValue, maxValue, posBegin, posEnd);
    }
    Item {
        id: bar;
        height: barSize;
        anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
        ExtraAnchors.horizontalFill: parent;

        Rectangle {
            id: groove;
            color: (base.enabled ? TricksStyle.colorEditable : TricksStyle.colorWindow);
            radius: (TricksStyle.roundness - TricksStyle.lineSize);
            enabled: base.enabled;
            antialiasing: radius;
            anchors {
                fill: parent;
                margins: TricksStyle.lineSize;
            }

            Rectangle {
                id: rect;
                x: startX;
                width: (endX - startX);
                radius: (TricksStyle.roundness - TricksStyle.lineSize * 2);
                enabled: base.enabled;
                antialiasing: radius;
                gradient: (base.enabled
                           ? TricksStyle.gradientChecked ()
                           : TricksStyle.gradientDisabled (TricksStyle.colorBorder));
                ExtraAnchors.verticalFill: parent;

                readonly property real pos1 : priv.posValue;
                readonly property real pos2 : (useSplit ? priv.posSplit : priv.posBegin);

                readonly property real startX : Math.min (pos1, pos2);
                readonly property real endX   : Math.max (pos1, pos2);
            }
        }
        Repeater {
            model: (divisions > 1 ? divisions -1 : 0);
            delegate: Line {
                x: ((parent.width / divisions) * (model.index +1));
                color: TricksStyle.colorForeground;
                opacity: 0.15;
                ExtraAnchors.verticalFill: parent;
            }
        }
        Rectangle {
            id: frame;
            color: TricksStyle.colorNone;
            radius: TricksStyle.roundness;
            enabled: base.enabled;
            antialiasing: radius;
            border {
                width: TricksStyle.lineSize;
                color: TricksStyle.colorBorder;
            }
            anchors.fill: parent;
        }
        Line {
            x: priv.posSplit;
            visible: useSplit;
            ExtraAnchors.verticalFill: parent;
        }
    }
}
