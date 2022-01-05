import QtQuick 2.6;
import QQmlTricks 3.0;

Rectangle {
    id: toolbar;
    height: (layout.height + layout.anchors.margins * 2);
    gradient: TricksStyle.gradientIdle (TricksStyle.colorWindow);
    ExtraAnchors.topDock: parent;

    property int padding : TricksStyle.spacingNormal;

    default property alias content : layout.data;

    Line { ExtraAnchors.bottomDock: parent; }
    RowContainer {
        id: layout;
        spacing: toolbar.padding;
        anchors.margins: toolbar.padding;
        anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
        ExtraAnchors.horizontalFill: parent;

        // NOTE : CONTENT GOES HERE
    }
}
