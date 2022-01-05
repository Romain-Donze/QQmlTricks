import QtQuick 2.6;
import QQmlTricks 3.0;

Item {
    id: self;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: contentWidth;
    implicitHeight: contentHeight;
    states: [
        State {
            name: "icon_and_text";
            when: (ico.visible && lbl.visible && !invertLayout);

            PropertyChanges {
                target: self;
                contentWidth: Math.ceil (ico.width + lbl.contentWidth + self.padding * 3);
                contentHeight: Math.ceil (ico.height > lbl.contentHeight
                                          ? ico.height + self.padding * 2
                                          : lbl.contentHeight + self.padding * 2);
            }
            AnchorChanges {
                target: ico;
                anchors {
                    left: parent.left;
                    verticalCenter: (ico && ico.parent ? parent.verticalCenter : undefined);
                }
            }
            AnchorChanges {
                target: lbl;
                anchors {
                    left: ico.right;
                    right: parent.right;
                    verticalCenter: (lbl && lbl.parent ? parent.verticalCenter : undefined);
                }
            }
        },
        State {
            name: "text_and_icon";
            when: (ico.visible && lbl.visible && invertLayout);

            PropertyChanges {
                target: self;
                contentWidth: Math.ceil (ico.width + lbl.contentWidth + self.padding * 3);
                contentHeight: Math.ceil (ico.height > lbl.contentHeight
                                          ? ico.height + self.padding * 2
                                          : lbl.contentHeight + self.padding * 2);
            }
            AnchorChanges {
                target: lbl;
                anchors {
                    left: parent.left;
                    right: ico.left;
                    verticalCenter: (lbl && lbl.parent ? parent.verticalCenter : undefined);
                }
            }
            AnchorChanges {
                target: ico;
                anchors {
                    right: parent.right;
                    verticalCenter: (ico && ico.parent ? parent.verticalCenter : undefined);
                }
            }
        },
        State {
            name: "text_only";
            when: (!ico.visible && lbl.visible);

            PropertyChanges {
                target: self;
                contentWidth: Math.ceil (Math.max (lbl.contentWidth + self.padding * 2, contentHeight));
                contentHeight: Math.ceil (lbl.contentHeight + self.padding * 2);
            }
            AnchorChanges {
                target: lbl;
                anchors {
                    verticalCenter: (lbl && lbl.parent ? parent.verticalCenter : undefined);
                    horizontalCenter: (lbl && lbl.parent ? parent.horizontalCenter : undefined);
                }
            }
        },
        State {
            name: "icon_only";
            when: (ico.visible && !lbl.visible);

            PropertyChanges {
                target: self;
                contentWidth: Math.ceil (ico.width + self.padding * 2);
                contentHeight: Math.ceil (ico.height + self.padding * 2);
            }
            AnchorChanges {
                target: ico;
                anchors {
                    verticalCenter: (ico && ico.parent ? parent.verticalCenter : undefined);
                    horizontalCenter: (ico && ico.parent ? parent.horizontalCenter : undefined);
                }
            }
        },
        State {
            name: "empty";
            when: (!ico.visible && !lbl.visible);

            PropertyChanges {
                target: self;
                contentWidth: 0;
                contentHeight: 0;
            }
        }
    ]

    property int   padding        : TricksStyle.spacingNormal;
    property bool  flat           : false;
    property bool  checked        : false;
    property bool  clickable      : true;
    property bool  autoColorIcon  : true;
    property bool  invertLayout   : false;
    property alias text           : lbl.text;
    property alias textFont       : lbl.font;
    property alias rounding       : rect.radius;
    property alias icon           : ico.sourceComponent;
    property alias hovered        : clicker.containsMouse;
    property alias autoRepeat     : clicker.autoRepeat;
    property alias repeatDelay    : clicker.repeatDelay;
    property alias repeatInterval : clicker.repeatInterval;
    property alias sensitiveHalo  : clicker.sensitiveHalo;
    property int   contentWidth   : 0;
    property int   contentHeight  : 0;
    property color backColor      : TricksStyle.colorClickable;
    property color textColor      : (TricksStyle.useDarkTheme !== TricksStyle.isDark (backColor)
                                     ? TricksStyle.colorInverted
                                     : TricksStyle.colorForeground);

    function click (isAutoRepeat) {
        if (enabled) {
            clicked (isAutoRepeat);
        }
    }

    signal clicked (bool isRepeated);

    AutoRepeatableClicker {
        id: clicker;
        visible: self.clickable;
        enabled: self.enabled;
        hoverEnabled: TricksStyle.useHovering;
        anchors.fill: parent;
        onClicked: {
            self.clicked (isRepeated);
        }
    }
    Binding {
        target: ico.item;
        when: (self.autoColorIcon && ico.item && "color" in ico.item);
        property: "color";
        value: self.textColor;
    }
    Rectangle {
        id: rect;
        width: Math.round (self.width);
        height: Math.round (self.height);
        enabled: self.enabled;
        radius: TricksStyle.roundness;
        visible: self.clickable;
        antialiasing: radius;
        gradient: (self.enabled
                   ? (self.checked
                      ? TricksStyle.gradientChecked ()
                      : (self.pressed
                         ? TricksStyle.gradientPressed (TricksStyle.opacify (self.backColor, self.flat ? 0.35 : 1.0))
                         : TricksStyle.gradientIdle (self.flat ? TricksStyle.colorNone : Qt.lighter (self.backColor, self.hovered ? 1.15 : 1.0))))
                   : TricksStyle.gradientDisabled (self.flat ? TricksStyle.colorNone : TricksStyle.colorClickable));
        border {
            width: (!self.flat || self.pressed || self.checked || self.hovered ? TricksStyle.lineSize : 0);
            color: (self.checked ? TricksStyle.colorSelection : TricksStyle.colorBorder);
        }
        anchors.fill: parent;
    }
    Loader {
        id: ico;
        active: (sourceComponent !== null);
        enabled: self.enabled;
        visible: (item !== null);
        anchors.margins: self.padding;
    }
    TextLabel {
        id: lbl;
        color: (self.enabled
                ? (self.checked
                   ? (TricksStyle.useDarkTheme
                      ? Qt.lighter (TricksStyle.colorSelection)
                      : Qt.darker  (TricksStyle.colorSelection))
                   : self.textColor)
                : TricksStyle.colorBorder);
        enabled: self.enabled;
        visible: (text !== "");
        horizontalAlignment: (ico.visible ? Text.AlignLeft : Text.AlignHCenter);
        font {
            family: TricksStyle.fontName;
            pixelSize: TricksStyle.fontSizeNormal;
        }
        anchors.margins: self.padding;
    }
}
