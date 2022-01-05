import QtQuick 2.15;
import QtQuick.Window 2.15;
import QQmlTricks 3.0;

Group {
    id: base;
    width: implicitWidth;
    height: implicitHeight;
    visible: !detached;
    implicitWidth: (expanded ? size : minifiedSize);
    implicitHeight: (expanded ? size : minifiedSize);
    states: [
        State {
            when: (borderSide === Item.Top);

            AnchorChanges {
                target: handle;
                anchors {
                    top: base.top;
                    left: base.left;
                    right: base.right;
                    bottom: undefined;
                }
            }
            PropertyChanges {
                target: handle;
                height: handleSize;
                cursorShape: Qt.SizeVerCursor;
            }
            PropertyChanges {
                target: grip;
                width: handle.width;
                height: handle.height;
                rotation: 0;
            }
            AnchorChanges {
                target: line;
                anchors {
                    top: base.top;
                    left: base.left;
                    right: base.right;
                    bottom: undefined;
                }
            }
            AnchorChanges {
                target: minibar;
                anchors {
                    top: base.top;
                    left: base.left;
                    right: base.right;
                    bottom: undefined;
                }
            }
            PropertyChanges {
                target: minibar;
                height: minifiedSize;
            }
        },
        State {
            when: (borderSide === Item.Left);

            AnchorChanges {
                target: handle;
                anchors {
                    top: base.top;
                    left: base.left;
                    right: undefined;
                    bottom: base.bottom;
                }
            }
            PropertyChanges {
                target: handle;
                width: handleSize;
                cursorShape: Qt.SizeHorCursor;
            }
            PropertyChanges {
                target: grip;
                width: handle.height;
                height: handle.width;
                rotation: 270;
            }
            AnchorChanges {
                target: line;
                anchors {
                    top: base.top;
                    left: base.left;
                    bottom: base.bottom;
                    right: undefined;
                }
            }
            AnchorChanges {
                target: minibar;
                anchors {
                    top: base.top;
                    left: base.left;
                    bottom: base.bottom;
                    right: undefined;
                }
            }
            PropertyChanges {
                target: minibar;
                width: minifiedSize;
            }
        },
        State {
            when: (borderSide === Item.Right);

            AnchorChanges {
                target: handle;
                anchors {
                    top: base.top;
                    left: undefined;
                    right: base.right;
                    bottom: base.bottom;
                }
            }
            PropertyChanges {
                target: handle;
                width: handleSize;
                cursorShape: Qt.SizeHorCursor;
            }
            PropertyChanges {
                target: grip;
                width: handle.height;
                height: handle.width;
                rotation: 90;
            }
            AnchorChanges {
                target: line;
                anchors {
                    top: base.top;
                    left: undefined;
                    right: base.right;
                    bottom: base.bottom;
                }
            }
            AnchorChanges {
                target: minibar;
                anchors {
                    top: base.top;
                    left: undefined;
                    right: base.right;
                    bottom: base.bottom;
                }
            }
            PropertyChanges {
                target: minibar;
                width: minifiedSize;
            }
        },
        State {
            when: (borderSide === Item.Bottom);

            AnchorChanges {
                target: handle;
                anchors {
                    top: undefined;
                    left: base.left;
                    right: base.right;
                    bottom: base.bottom;
                }
            }
            PropertyChanges {
                target: handle;
                height: handleSize;
                cursorShape: Qt.SizeVerCursor;
            }
            PropertyChanges {
                target: grip;
                width: handle.width;
                height: handle.height;
                rotation: 180;
            }
            AnchorChanges {
                target: line;
                anchors {
                    top: undefined;
                    left: base.left;
                    right: base.right;
                    bottom: base.bottom;
                }
            }
            AnchorChanges {
                target: minibar;
                anchors {
                    top: undefined;
                    left: base.left;
                    right: base.right;
                    bottom: base.bottom;
                }
            }
            PropertyChanges {
                target: minibar;
                height: minifiedSize;
            }
        }
    ]

    property int size       : TricksStyle.realPixels (250);
    property int maxSize    : TricksStyle.realPixels (500);
    property int minSize    : TricksStyle.realPixels (100);

    property int handleSize : TricksStyle.spacingBig;

    property int borderSide : Item.Right;

    property bool resizable   : true;
    property bool detachable  : true;
    property bool collapsable : true;

    default property alias content : panel.data;

    readonly property bool detached  : (priv.subWindow !== null);
    readonly property bool expanded  : !priv.minified;
    readonly property bool collapsed : priv.minified;

    readonly property int headerSize   : (header.height);
    readonly property int minifiedSize : (info.implicitHeight + info.anchors.margins * 2);

    function detach () {
        if (priv.subWindow === null) {
            var rootItem = Introspector.window (base);
            var abspos = rootItem.contentItem.mapFromItem (base, 0 , 0);
            priv.subWindow = compoWindow.createObject (null, {
                                                           "x"      : (abspos.x + rootItem.x),
                                                           "y"      : (abspos.y + rootItem.y),
                                                           "width"  : container.width,
                                                           "height" : container.height,
                                                       });
            panel.parent = priv.subWindow.contentItem;
        }
    }

    function attach () {
        if (priv.subWindow !== null) {
            panel.parent = container;
            priv.subWindow.destroy ();
        }
    }

    function expand () {
        priv.minified = false;
    }

    function collapse () {
        priv.minified = true;
    }

    QtObject {
        id: priv;

        property bool minified : false;

        property Window subWindow : null;
    }
    Component {
        id: compoWindow;

        Window {
            color: rect.color;
            title: base.title;
            flags: Qt.Window;
            visible: true;
            modality: Qt.NonModal;
            onClosing: { attach (); }
        }
    }
    Rectangle {
        id: rect;
        color: TricksStyle.colorSecondary;
        anchors.fill: parent;
    }
    MouseArea {
        id: handle;
        visible: (expanded && resizable);
        onPressed: {
            var tmp = mapToItem (base.parent, mouse.x, mouse.y);
            originalPos  = Qt.point (tmp.x, tmp.y);
            originalSize = size;
        }
        onPositionChanged: {
            var absCurrPos = mapToItem (base.parent, mouse.x, mouse.y);
            var deltaX = (absCurrPos.x - originalPos.x);
            var deltaY = (absCurrPos.y - originalPos.y);
            var tmp = originalSize;
            switch (borderSide) {
            case Item.Top:
                tmp -= deltaY;
                break;
            case Item.Left:
                tmp -= deltaX;
                break;
            case Item.Right:
                tmp += deltaX;
                break;
            case Item.Bottom:
                tmp += deltaY;
                break;
            }
            size = Math.max (minSize, Math.min (maxSize, tmp));
        }

        property int   originalSize : 0;
        property point originalPos  : Qt.point (0,0);

        Rectangle {
            id: grip;
            gradient: (handle.pressed
                       ? TricksStyle.gradientShaded (TricksStyle.colorHighlight, TricksStyle.colorNone)
                       : TricksStyle.gradientShaded (TricksStyle.colorClickable, TricksStyle.colorNone));
            anchors.centerIn: parent;
        }
    }
    Item {
        id: minibar;
        visible: collapsed;

        RowContainer {
            id: info;
            spacing: TricksStyle.spacingNormal;
            rotation: {
                switch (borderSide) {
                case Item.Top:    return 0;
                case Item.Left:   return 90;
                case Item.Right:  return 270;
                case Item.Bottom: return 0;
                default:          return 0;
                }
            }
            anchors.margins: TricksStyle.spacingNormal;
            anchors.centerIn: parent;

            Stretcher { }
            Loader {
                enabled: base.enabled;
                visible: (sourceComponent !== null);
                rotation: -parent.rotation;
                sourceComponent: base.icon;
                anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
            }
            TextLabel {
                text: base.title;
                visible: (text !== "");
                font.pixelSize: TricksStyle.fontSizeBig;
                font.capitalization: Font.SmallCaps;
                anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
            }
            Stretcher { }
        }

        TextButton {
            flat: true;
            icon: SvgIconLoader {
                icon: {
                    switch (borderSide) {
                    case Item.Top:    return "actions/arrow-top";
                    case Item.Left:   return "actions/arrow-first";
                    case Item.Right:  return "actions/arrow-last";
                    case Item.Bottom: return "actions/arrow-bottom";
                    default:          return "";
                    }
                }
                size: TricksStyle.fontSizeBig;
                color: TricksStyle.colorForeground;
            }
            visible: collapsable;
            padding: (TricksStyle.lineSize * 2);
            anchors {
                top: (borderSide === Item.Left || borderSide === Item.Right ? parent.top : undefined);
                right: (borderSide === Item.Top || borderSide === Item.Bottom ? parent.right : undefined);
                verticalCenter: (borderSide === Item.Top || borderSide === Item.Bottom ? parent.verticalCenter : undefined);
                horizontalCenter: (borderSide === Item.Left || borderSide === Item.Right ? parent.horizontalCenter : undefined);
                topMargin: (minibar.width - width) / 2;
                rightMargin: (minibar.height - height) / 2;
            }
            onClicked: { expand (); }
        }
    }
    Item {
        id: header;
        height: (layout.implicitHeight + layout.anchors.margins * 2);
        visible: expanded && (title !== "" || icon !== null || detachable || collapsable);
        ExtraAnchors.topDock: parent;

        RowContainer {
            id: layout;
            spacing: TricksStyle.spacingNormal;
            anchors {
                margins: TricksStyle.spacingNormal;
                verticalCenter: (parent ? parent.verticalCenter : undefined);
            }
            ExtraAnchors.horizontalFill: parent;

            Loader {
                id: ico;
                enabled: base.enabled;
                visible: (sourceComponent !== null);
                sourceComponent: base.icon;
                anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
            }
            Stretcher {
                height: implicitHeight;
                implicitHeight: lbl.implicitHeight;
                anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);

                TextLabel {
                    id: lbl;
                    text: base.title;
                    clip: (contentWidth > width);
                    visible: (text !== "");
                    font.pixelSize: TricksStyle.fontSizeBig;
                    font.capitalization: Font.SmallCaps;
                    ExtraAnchors.horizontalFill: parent;
                }
            }
            TextButton {
                flat: true;
                icon: SvgIconLoader {
                    icon: "actions/fullscreen";
                    size: TricksStyle.fontSizeBig;
                    color: TricksStyle.colorForeground;
                }
                visible: detachable;
                padding: (TricksStyle.lineSize * 2);
                anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
                onClicked: { detach (); }
            }
            TextButton {
                flat: true;
                icon: SvgIconLoader {
                    icon: {
                        switch (borderSide) {
                        case Item.Top:    return "actions/arrow-bottom";
                        case Item.Left:   return "actions/arrow-last";
                        case Item.Right:  return "actions/arrow-first";
                        case Item.Bottom: return "actions/arrow-top";
                        default:          return "";
                        }
                    }
                    size: TricksStyle.fontSizeBig;
                    color: TricksStyle.colorForeground;
                }
                visible: collapsable;
                padding: (TricksStyle.lineSize * 2);
                anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
                onClicked: { collapse (); }
            }
        }
        Line { ExtraAnchors.bottomDock: parent; }
    }
    Item {
        id: container;
        visible: expanded;
        anchors {
            top: (header.visible ? header.bottom : parent.top);
            left: (borderSide === Item.Left && handle.visible ? handle.right : parent.left);
            right: (borderSide === Item.Right && handle.visible ? handle.left : parent.right);
            bottom: (borderSide === Item.Bottom && handle.visible ? handle.top : parent.bottom)
        }

        Item {
            id: panel;
            anchors.fill: parent;

            // CONTENT HERE
        }
    }
    Line { id: line; }
}
