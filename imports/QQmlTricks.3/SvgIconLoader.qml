import QtQuick 2.6;
import QQmlTricks 3.0;

Item {
    id: base;
    visible: (icon !== "");
    implicitWidth: helper.size;
    implicitHeight: helper.size;

    property int   size   : TricksStyle.iconSize (1);
    property bool  dimmed : !base.enabled;
    property color color  : TricksStyle.colorNone;
    property alias icon   : helper.icon;

    Image {
        id: img;
        cache: true;
        smooth: false;
        opacity: 1.0 //(dimmed ? 0.65 : 1.0);
        fillMode: Image.Pad;
        antialiasing: false;
        asynchronous: true;
        anchors.centerIn: parent;
        anchors.alignWhenCentered: true;

        SvgIconHelper on source {
            id: helper;
            size: base.size;
            color: base.color //(base.dimmed ? TricksStyle.colorBorder : base.color);
        }
    }
}
