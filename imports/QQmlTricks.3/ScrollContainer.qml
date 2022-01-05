import QtQuick 2.6;
import QQmlTricks 3.0;

FocusScope {
    id: base;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: -1;
    implicitHeight: -1;

    property bool      stayAtBottom  : false;
    property bool      showBorder    : true;
    property bool      indicatorOnly : stayAtBottom;
    property bool      arrowButtons  : TricksStyle.arrowButtons;
    property alias     placeholder   : lbl.text;
    property alias     background    : rect.color;
    property alias     headerItem    : loaderHeader.sourceComponent;
    property alias     footerItem    : loaderFooter.sourceComponent;
    property int       pageSizeX     : (flickableItem ? flickableItem.width  / 5 : 100);
    property int       pageSizeY     : (flickableItem ? flickableItem.height / 5 : 100);
    property Flickable flickableItem : null;
    property int       rounding      : TricksStyle.roundness;

    default property alias content : base.flickableItem;

    readonly property bool canScrollX : (flickableItem && flickableItem.contentWidth  > flickableItem.width);
    readonly property bool canScrollY : (flickableItem && flickableItem.contentHeight > flickableItem.height);

    readonly property bool canGoUp     : (canScrollY && flickableItem && flickableItem.contentY > minContentY);
    readonly property bool canGoLeft   : (canScrollX && flickableItem && flickableItem.contentX > minContentX);
    readonly property bool canGoRight  : (canScrollX && flickableItem && flickableItem.contentX < maxContentX);
    readonly property bool canGoDown   : (canScrollY && flickableItem && flickableItem.contentY < maxContentY);

    readonly property real minContentX : 0;
    readonly property real minContentY : 0;
    readonly property real maxContentX : (canScrollX ? flickableItem.contentWidth  - flickableItem.width  : 0);
    readonly property real maxContentY : (canScrollY ? flickableItem.contentHeight - flickableItem.height : 0);

    readonly property real ratioContentX : (canScrollX ? flickableItem.contentX / maxContentX : 0.0);
    readonly property real ratioContentY : (canScrollY ? flickableItem.contentY / maxContentY : 0.0);
    readonly property real ratioContentW : (canScrollX ? flickableItem.width  / flickableItem.contentWidth  : 0.0);
    readonly property real ratioContentH : (canScrollY ? flickableItem.height / flickableItem.contentHeight : 0.0);

    function ensureVisible (item) {
        if (item && flickableItem) {
            // position of the object's center against the flickable viewport top-left origin
            var relPos = item.mapToItem (flickableItem, (item.width / 2), (item.height / 2));
            // position of the object's center against the flickable content scene top-left origin
            var scenePosX = (flickableItem.contentX + relPos.x);
            var scenePosY = (flickableItem.contentY + relPos.y);
            // compute ideal flickable content scene position to have object's center in viewport center
            var idealContentX = (scenePosX - (flickableItem.width  / 2));
            var idealContentY = (scenePosY - (flickableItem.height / 2));
            // move flickable content scene, in respect of boundaries
            flickableItem.contentX = clamp (idealContentX, minContentX, maxContentX);
            flickableItem.contentY = clamp (idealContentY, minContentY, maxContentY);
        }
    }

    function clamp (val, min, max) {
        return (val > max ? max : (val < min ? min : val));
    }

    Binding {
        target: flickableItem;
        property: "contentY";
        value: (flickableItem ? Math.max (0, flickableItem.contentHeight - flickableItem.height) : 0);
        when: stayAtBottom;
    }
    Rectangle {
        id: rect;
        color: TricksStyle.colorEditable;
        border {
            color: TricksStyle.colorBorder;
            width: (showBorder ? TricksStyle.lineSize : 0);
        }
        anchors {
            fill: parent;
            topMargin: (headerItem ? loaderHeader.height - TricksStyle.lineSize : 0);
            bottomMargin: (footerItem ? loaderFooter.height - TricksStyle.lineSize : 0);
        }
    }
    Loader {
        id: loaderHeader;
        clip: true;
        visible: item;
        ExtraAnchors.topDock: parent;

        Rectangle {
            z: -1;
            width: Math.round (parent.width);
            radius: rounding;
            antialiasing: radius;
            gradient: Gradient {
                GradientStop { position: 0.0; color: TricksStyle.colorWindow; }
                GradientStop { position: 1.0; color: background; }
            }
            border {
                color: TricksStyle.colorBorder;
                width: (showBorder ? TricksStyle.lineSize : 0);
            }
            anchors.bottomMargin: -radius;
            ExtraAnchors.leftDock: parent;
        }
    }
    Loader {
        id: loaderFooter;
        clip: true;
        visible: item;
        ExtraAnchors.bottomDock: parent;

        Rectangle {
            z: -1;
            width: Math.round (parent.width);
            radius: rounding;
            antialiasing: radius;
            gradient: Gradient {
                GradientStop { position: 0.0; color: background; }
                GradientStop { position: 1.0; color: TricksStyle.colorWindow; }
            }
            border {
                color: TricksStyle.colorBorder;
                width: (showBorder ? TricksStyle.lineSize : 0);
            }
            anchors.topMargin: -radius;
            ExtraAnchors.leftDock: parent;
        }
    }
    Item {
        id: container;
        clip: true;
        anchors {
            top: (loaderHeader && loaderHeader.item ? loaderHeader.bottom : parent.top);
            bottom: (loaderFooter && loaderFooter.item ? loaderFooter.top : parent.bottom);
            margins: rect.border.width;
        }
        ExtraAnchors.horizontalFill: parent;

        Binding {
            target: (flickableItem ? flickableItem.anchors : null);
            property: "fill";
            value: viewport;
        }
        Binding {
            target: flickableItem;
            property: "boundsBehavior";
            value: Flickable.StopAtBounds;
        }
        Binding {
            target: flickableItem;
            property: "interactive";
            value: true;
        }
        Binding {
            target: flickableItem;
            property: "pixelAligned";
            value: true;
        }
        Item {
            id: viewport;
            children: flickableItem;
            anchors {
                fill: parent;
                rightMargin: (scrollbarY && scrollbarY.visible ? scrollbarY.width : 0);
                bottomMargin: (scrollbarX && scrollbarX.visible ? scrollbarX.height : 0);
            }

            // CONTENT HERE
        }
        TextLabel {
            id: lbl;
            color: TricksStyle.colorBorder;
            font.pixelSize: TricksStyle.fontSizeBig;
            verticalAlignment: Text.AlignVCenter;
            horizontalAlignment: Text.AlignHCenter;
            anchors {
                fill: parent;
                margins: TricksStyle.spacingBig;
            }
        }
        Item {
            id: scrollbarX;
            height: (indicatorOnly ? TricksStyle.spacingSmall : TricksStyle.spacingBig);
            visible: (flickableItem && flickableItem.flickableDirection !== Flickable.VerticalFlick);
            opacity: (flickableItem && flickableItem.contentWidth > flickableItem.width ? 1.0 : 0.35);
            anchors.rightMargin: (scrollbarY && scrollbarY.visible ? scrollbarY.width : 0);
            ExtraAnchors.bottomDock: parent;

            Rectangle {
                id: backBottom;
                color: TricksStyle.colorGroove;
                anchors.fill: parent;
            }
            TextButton {
                id: btnLeft;
                width: height;
                enabled: canGoLeft;
                visible: (arrowButtons && canScrollX);
                ExtraAnchors.leftDock: parent;
                onClicked: {
                    if (flickableItem) {
                        flickableItem.contentX = Math.max ((flickableItem.contentX - pageSizeX), minContentX);
                    }
                }
            }
            TextButton {
                id: btnRight;
                width: height;
                enabled: canGoRight;
                visible: (arrowButtons && canScrollX);
                ExtraAnchors.rightDock: parent;
                onClicked: {
                    if (flickableItem) {
                        flickableItem.contentX = Math.min ((flickableItem.contentX + pageSizeX), maxContentX);
                    }
                }
            }
            SymbolLoader {
                id: arrowLeft;
                size: TricksStyle.spacingNormal;
                color: (flickableItem && !flickableItem.atXBeginning ? TricksStyle.colorForeground : TricksStyle.colorBorder);
                width: height;
                symbol: TricksStyle.symbolArrowLeft;
                visible: !indicatorOnly;
                autoSize: false;
                ExtraAnchors.leftDock: parent;
            }
            SymbolLoader {
                id: arrowRight;
                size: TricksStyle.spacingNormal;
                color: (flickableItem && !flickableItem.atXEnd ? TricksStyle.colorForeground : TricksStyle.colorBorder);
                width: height;
                symbol: TricksStyle.symbolArrowRight;
                visible: !indicatorOnly;
                autoSize: false;
                ExtraAnchors.rightDock: parent;
            }
            MouseArea {
                id: grooveHoriz;
                clip: true;
                enabled: (canScrollX && !indicatorOnly);
                drag {
                    axis: Drag.XAxis;
                    target: handleHoriz;
                    minimumX: 0;
                    maximumX: (grooveHoriz.width - handleHoriz.width);
                }
                anchors {
                    fill: parent;
                    leftMargin: (!indicatorOnly ? height : 0);
                    rightMargin: (!indicatorOnly ? height : 0);
                }
                onPositionChanged: {
                    flickableItem.contentX = ((flickableItem.contentWidth - flickableItem.width) * handleHoriz.x / grooveHoriz.drag.maximumX);
                }

                Item {
                    id: handleHoriz;
                    visible: canScrollX;
                    ExtraAnchors.verticalFill: parent;

                    Binding on x {
                        when: (flickableItem && !grooveHoriz.pressed);
                        value: (grooveHoriz.drag.maximumX * ratioContentX);
                    }
                    Binding on width {
                        when: (flickableItem && !grooveHoriz.pressed);
                        value: Math.max (grooveHoriz.width * ratioContentW, 40);
                    }
                    Rectangle {
                        id: rectHoriz;
                        color: (grooveHoriz.pressed ? TricksStyle.colorSecondary : TricksStyle.colorClickable);
                        width: Math.round (parent.width);
                        height: Math.round (parent.height);
                        radius: (indicatorOnly ? TricksStyle.lineSize : TricksStyle.roundness);
                        antialiasing: radius;
                        border {
                            width: TricksStyle.lineSize;
                            color: TricksStyle.colorSecondary;
                        }
                        anchors {
                            fill: parent;
                            margins: TricksStyle.lineSize;
                        }
                    }
                }
            }
        }
        Item {
            id: scrollbarY;
            width: (indicatorOnly ? TricksStyle.spacingSmall : TricksStyle.spacingBig);
            visible: (flickableItem && flickableItem.flickableDirection !== Flickable.HorizontalFlick);
            opacity: (flickableItem && flickableItem.contentHeight > flickableItem.height ? 1.0 : 0.35);
            anchors.bottomMargin: (scrollbarX.visible ? scrollbarX.height : 0);
            ExtraAnchors.rightDock: parent;

            Rectangle {
                id: backRight;
                color: TricksStyle.colorGroove;
                anchors.fill: parent;
            }
            TextButton {
                id: btnUp;
                height: width;
                enabled: canGoUp;
                visible: (arrowButtons && canScrollY);
                ExtraAnchors.topDock: parent;
                onClicked: {
                    if (flickableItem) {
                        flickableItem.contentY = Math.max ((flickableItem.contentY - pageSizeY), minContentY);
                    }
                }
            }
            TextButton {
                id: btnDown;
                height: width;
                enabled: canGoDown;
                visible: (arrowButtons && canScrollY);
                ExtraAnchors.bottomDock: parent;
                onClicked: {
                    if (flickableItem) {
                        flickableItem.contentY = Math.min ((flickableItem.contentY + pageSizeY), maxContentY);
                    }
                }
            }
            SymbolLoader {
                id: arrowUp;
                size: TricksStyle.spacingNormal;
                color: (flickableItem && !flickableItem.atYBeginning ? TricksStyle.colorForeground : TricksStyle.colorBorder);
                symbol: TricksStyle.symbolArrowUp;
                height: width;
                visible: !indicatorOnly;
                autoSize: false;
                ExtraAnchors.topDock: parent;
            }
            SymbolLoader {
                id: arrowDown;
                size: TricksStyle.spacingNormal;
                color: (flickableItem && !flickableItem.atYEnd ? TricksStyle.colorForeground : TricksStyle.colorBorder);
                height: width;
                symbol: TricksStyle.symbolArrowDown;
                visible: !indicatorOnly;
                autoSize: false;
                ExtraAnchors.bottomDock: parent;
            }
            MouseArea {
                id: grooveVertic;
                clip: true;
                enabled: (canScrollY && !indicatorOnly);
                drag {
                    axis: Drag.YAxis;
                    target: handleVertic;
                    minimumY: 0;
                    maximumY: (grooveVertic.height - handleVertic.height);
                }
                anchors {
                    fill: parent;
                    topMargin: (!indicatorOnly ? width : 0);
                    bottomMargin: (!indicatorOnly ? width : 0);
                }
                onPositionChanged: {
                    flickableItem.contentY = ((flickableItem.contentHeight - flickableItem.height) * handleVertic.y / grooveVertic.drag.maximumY);
                }

                Item {
                    id: handleVertic;
                    visible: canScrollY;
                    ExtraAnchors.horizontalFill: parent;

                    Binding on y {
                        when: (flickableItem && !grooveVertic.pressed);
                        value: (grooveVertic.drag.maximumY * ratioContentY);
                    }
                    Binding on height {
                        when: (flickableItem && !grooveVertic.pressed);
                        value: Math.max (grooveVertic.height * ratioContentH, 40);
                    }
                    Rectangle {
                        id: rectVertic;
                        color: (grooveVertic.pressed ? TricksStyle.colorSecondary : TricksStyle.colorClickable);
                        width: Math.round (parent.width);
                        height: Math.round (parent.height);
                        radius: (indicatorOnly ? TricksStyle.lineSize : TricksStyle.roundness);
                        antialiasing: radius;
                        border {
                            width: TricksStyle.lineSize;
                            color: TricksStyle.colorSecondary;
                        }
                        anchors {
                            fill: parent;
                            margins: TricksStyle.lineSize;
                        }
                    }
                }
            }
        }
        Rectangle {
            color: TricksStyle.colorGroove;
            width: scrollbarY.width;
            height: scrollbarX.height;
            visible: (scrollbarX.visible && scrollbarY.visible);
            ExtraAnchors.bottomRightCorner: parent;
        }
    }
}
