import QtQuick 2.6;
import QQmlTricks 3.0;

Item {
    id: base;

    property alias background : rect.color;

    property int tabsSize : (TricksStyle.spacingBig * 2);

    property int extraPaddingBeforeTabs : 0;
    property int extraPaddingAfterTabs  : 0;

    property Group currentTab : null;

    default property alias content : container.children;

    Rectangle {
        id: rect;
        color: TricksStyle.colorSecondary;
        anchors.bottom: bar.bottom;
        ExtraAnchors.topDock: parent;
    }
    Line {
        anchors.bottom: bar.bottom;
        ExtraAnchors.horizontalFill: parent;
    }
    RowContainer {
        id: bar;
        clip: true;
        spacing: TricksStyle.spacingSmall;
        anchors {
            topMargin: bar.spacing;
            leftMargin: (bar.spacing + extraPaddingBeforeTabs);
            rightMargin: (bar.spacing + extraPaddingAfterTabs);
        }
        ExtraAnchors.topDock: parent;

        Repeater {
            model:  {
                var ret = [];
                for (var idx = 0; idx < base.content.length; idx++) {
                    var item = base.content [idx];
                    if (Introspector.isGroupItem (item)) {
                        ret.push (item);
                    }
                }
                return ret;
            }
            delegate: MouseArea {
                id: clicker;
                visible: (clicker.group && clicker.group.enabled);
                implicitHeight: tabsSize;
                states: [
                    State {
                        name: "text_and_icon";
                        when: (clicker.group.icon !== null && clicker.group.title !== "");

                        AnchorChanges {
                            target: lbl;
                            anchors {
                                left: ico.right;
                                right: parent.right;
                                verticalCenter: (parent ? parent.verticalCenter : undefined);
                            }
                        }
                        AnchorChanges {
                            target: ico;
                            anchors {
                                left: parent.left;
                                verticalCenter: (parent ? parent.verticalCenter : undefined);
                            }
                        }
                        PropertyChanges {
                            target: lbl;
                            visible: true;
                            horizontalAlignment: Text.AlignLeft;
                        }
                        PropertyChanges {
                            target: ico;
                            visible: true;
                        }
                    },
                    State {
                        name: "text_only";
                        when: (clicker.group.icon === null && clicker.group.title !== "");

                        AnchorChanges {
                            target: lbl;
                            anchors {
                                left: parent.left;
                                right: parent.right;
                                verticalCenter: (parent ? parent.verticalCenter : undefined);
                            }
                        }
                        PropertyChanges {
                            target: lbl;
                            visible: true;
                            horizontalAlignment: Text.AlignHCenter;
                        }
                        PropertyChanges {
                            target: ico;
                            visible: false;
                        }
                    },
                    State {
                        name: "icon_only";
                        when: (clicker.group.icon !== null && clicker.group.title === "");

                        AnchorChanges {
                            target: ico;
                            anchors {
                                verticalCenter: (parent ? parent.verticalCenter : undefined);
                                horizontalCenter: (parent ? parent.horizontalCenter : undefined);
                            }
                        }
                        PropertyChanges {
                            target: lbl;
                            visible: false;
                        }
                        PropertyChanges {
                            target: ico;
                            visible: true;
                        }
                    }
                ]
                Container.horizontalStretch: 1;
                onClicked: {
                    currentTab = modelData;
                }

                readonly property Group group : modelData;

                Rectangle {
                    color: TricksStyle.colorNone;
                    radius: TricksStyle.roundness;
                    gradient: (clicker.pressed
                               ? TricksStyle.gradientPressed ()
                               : (currentTab === clicker.group
                                  ? TricksStyle.gradientShaded ()
                                  : null));
                    antialiasing: radius;
                    border {
                        width: TricksStyle.lineSize;
                        color: TricksStyle.colorBorder;
                    }
                    states: State {
                        when: (clicker.group !== null);

                        PropertyChanges {
                            target: clicker.group;
                            visible: (currentTab === clicker.group);
                            anchors.fill: container;
                        }
                    }
                    anchors {
                        fill: parent;
                        bottomMargin: -radius;
                    }
                }
                Loader {
                    id: ico;
                    enabled: clicker.enabled;
                    sourceComponent: clicker.group.icon;
                    anchors.margins: TricksStyle.spacingNormal;
                }
                TextLabel {
                    id: lbl;
                    text: clicker.group.title;
                    clip: (contentWidth > width);
                    anchors.margins: TricksStyle.spacingNormal;
                }
                Line {
                    visible: (currentTab !== clicker.group);
                    ExtraAnchors.bottomDock: parent;
                }
            }
        }
    }
    FocusScope {
        id: container;
        anchors.top: bar.bottom;
        ExtraAnchors.bottomDock: parent;

        // NOTE : CONTENT HERE
    }
}
