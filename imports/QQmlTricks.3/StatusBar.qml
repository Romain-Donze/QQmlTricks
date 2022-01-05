import QtQuick 2.6;
import QQmlTricks 3.0;

Rectangle {
    id: statusbar;
    height: (layout.height + layout.anchors.margins * 2);
    gradient: TricksStyle.gradientIdle (TricksStyle.colorWindow);
    ExtraAnchors.bottomDock: parent;

    default property alias content : layout.data;

    Line { ExtraAnchors.topDock: parent; }
    RowContainer {
        id: layout;
        spacing: TricksStyle.spacingNormal;
        anchors.margins: TricksStyle.spacingNormal;
        anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
        ExtraAnchors.horizontalFill: parent;

        // NOTE : CONTENT GOES HERE
    }
}
