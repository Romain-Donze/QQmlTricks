import QtQuick 2.6;
import QQmlTricks 3.0;

MouseArea {
    id: base;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: (layout.implicitWidth + layout.anchors.margins * 2);
    implicitHeight: (layout.height + layout.anchors.margins * 2);

    property alias image   : img.sourceComponent;
    property alias title   : lblTitle.text;
    property alias content : lblContent.text;

    Rectangle {
        id: rect;
        width: Math.round (parent.width);
        height: Math.round (parent.height);
        color: TricksStyle.colorBubble;
        radius: TricksStyle.roundness;
        antialiasing: radius;
        border {
            width: TricksStyle.lineSize;
            color: Qt.darker (color);
        }
        anchors.fill: parent;
    }
    ColumnContainer {
        id: layout;
        spacing: TricksStyle.spacingSmall;
        anchors.margins: TricksStyle.spacingNormal;
        ExtraAnchors.topDock: parent;

        TextLabel {
            id: lblTitle;
            visible: (text !== "");
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere;
            horizontalAlignment: Text.AlignJustify;
            emphasis: true;
            font.pixelSize: TricksStyle.fontSizeSmall;
            ExtraAnchors.horizontalFill: parent;
        }
        TextLabel {
            id: lblContent;
            visible: (text !== "");
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere;
            horizontalAlignment: Text.AlignJustify;
            font.pixelSize: TricksStyle.fontSizeSmall;
            ExtraAnchors.horizontalFill: parent;
        }
        Loader {
            id: img;
            enabled: base.enabled;
            visible: (sourceComponent !== null && item !== null);
            anchors {
                top: parent.top;
                horizontalCenter: (parent ? parent.horizontalCenter : undefined);
            }
            Container.verticalStretch: 1;
        }
    }
}
