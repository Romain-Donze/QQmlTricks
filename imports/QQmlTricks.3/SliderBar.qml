import QtQuick 2.6;
import QQmlTricks 3.0;

ProgressJauge {
    id: base;
    width: implicitWidth;
    height: implicitHeight;
    barSize: TricksStyle.spacingNormal;
    implicitWidth: 200;
    implicitHeight: handle.height;

    property int decimals : 0;

    property int handleSize : (TricksStyle.spacingBig * 2);

    property bool editable : true;

    property bool showTooltipWhenMoved : true;

    readonly property real ratio : Math.pow (10, decimals);

    signal edited ();

    MouseArea {
        visible: editable;
        anchors.fill: parent;
        onClicked: {
            var tmp = TricksStyle.convert (mouse.x,
                                     0,
                                     width,
                                     minValue,
                                     maxValue);
            value = (Math.round (tmp * ratio) / ratio);
            edited ();
        }
    }
    Rectangle {
        id: handle;
        width: handleSize;
        height: handleSize;
        radius: (handleSize / 2);
        visible: editable;
        enabled: base.enabled;
        antialiasing: radius;
        gradient: (enabled
                   ? (clicker.pressed
                      ? TricksStyle.gradientPressed ()
                      : TricksStyle.gradientIdle (Qt.lighter (TricksStyle.colorClickable, clicker.containsMouse ? 1.15 : 1.0)))
                   : TricksStyle.gradientDisabled ());
        border {
            width: TricksStyle.lineSize;
            color: TricksStyle.colorBorder;
        }
        anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);

        Binding on x {
            when: !clicker.pressed;
            value: TricksStyle.convert (base.value,
                                  base.minValue,
                                  base.maxValue,
                                  clicker.drag.minimumX,
                                  clicker.drag.maximumX);
        }
        MouseArea {
            id: clicker;
            drag {
                target: handle;
                minimumX: 0;
                maximumX: (base.width - handle.width);
                minimumY: 0;
                maximumY: 0;
            }
            enabled: base.enabled;
            hoverEnabled: TricksStyle.useHovering;
            anchors.fill: parent;
            onPressed: {
                if (tooltip === null && showTooltipWhenMoved) {
                    tooltip = compoTooltip.createObject (Introspector.window (base));
                }
            }
            onReleased: {
                if (tooltip !== null && showTooltipWhenMoved) {
                    tooltip.destroy ();
                    tooltip = null;
                }
            }
            onPositionChanged: {
                if (pressed) {
                    var tmp = TricksStyle.convert (handle.x,
                                             clicker.drag.minimumX,
                                             clicker.drag.maximumX,
                                             minValue,
                                             maxValue);
                    value = (Math.round (tmp * ratio) / ratio);
                    edited ();
                }
            }

            property Item tooltip : null;

            Component {
                id: compoTooltip;

                Rectangle {
                    id: rect;
                    x: (handleTopCenterAbsPos.x - width / 2);
                    y: (handleTopCenterAbsPos.y - height - TricksStyle.spacingNormal);
                    z: 9999999;
                    width: Math.ceil (lblTooltip.implicitWidth + lblTooltip.anchors.margins * 2);
                    height: Math.ceil (lblTooltip.implicitHeight + lblTooltip.anchors.margins * 2);
                    color: TricksStyle.colorBubble;
                    radius: TricksStyle.roundness;
                    antialiasing: radius;
                    border {
                        width: TricksStyle.lineSize;
                        color: Qt.darker (color);
                    }

                    readonly property var handleTopCenterAbsPos : base.mapToItem (parent,
                                                                                  (handle.x + handle.width / 2),
                                                                                  (handle.y));

                    TextLabel {
                        id: lblTooltip;
                        text: base.value.toFixed (decimals);
                        font.pixelSize: TricksStyle.fontSizeSmall;
                        anchors.margins: TricksStyle.spacingSmall;
                        anchors.centerIn: parent;
                    }
                }
            }
        }
    }
}
