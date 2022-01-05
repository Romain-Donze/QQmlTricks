import QtQuick 2.1;
import QQmlTricks 3.0;

Item {
    id: accordion;

    property Group currentTab : null;

    property alias background : rect.color;

    property int padding : TricksStyle.spacingNormal;

    property int tabSize : (TricksStyle.fontSizeNormal + padding * 2);

    readonly property int paneSize : (height - accordion.tabs.length * (tabSize + TricksStyle.lineSize));

    readonly property var tabs : {
        var ret = [];
        for (var idx = 0; idx < content.length; idx++) {
            var item = content [idx];
            if (Introspector.inherits (item, testGroup)) {
                ret.push (item);
            }
        }
        return ret;
    }

    default property alias content : container.children;

    Group { id: testGroup; }
    Rectangle {
        id: rect;
        color: TricksStyle.colorSecondary;
        anchors.fill: parent;
    }
    Item {
        id: container;
        height: paneSize;
        anchors.topMargin: ((tabSize + TricksStyle.lineSize) * (accordion.tabs.indexOf (currentTab) +1));
        ExtraAnchors.topDock: parent;

        // NOTE : accordion.tabs content here
    }
    Column {
        anchors.fill: parent;

        Repeater {
            model: accordion.tabs;
            delegate: Column {
                id: col;
                states: [
                    State {
                        name: "text_and_icon";
                        when: (col.group.icon !== null);

                        AnchorChanges {
                            target: lbl;
                            anchors {
                                left: ico.right;
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
                    },
                    State {
                        name: "text_only";
                        when: (col.group.icon === null);

                        AnchorChanges {
                            target: lbl;
                            anchors {
                                verticalCenter: (parent ? parent.verticalCenter : undefined);
                                horizontalCenter: (parent ? parent.horizontalCenter : undefined);
                            }
                        }
                    }
                ]
                ExtraAnchors.horizontalFill: parent;

                readonly property Group group : modelData;

                MouseArea {
                    height: tabSize;
                    ExtraAnchors.horizontalFill: parent;
                    onClicked: { currentTab = (currentTab !== col.group ? col.group : null); }

                    Rectangle {
                        gradient: (col.group === currentTab
                                   ? TricksStyle.gradientShaded (TricksStyle.colorHighlight, TricksStyle.colorSecondary)
                                   : (parent.pressed
                                      ? TricksStyle.gradientPressed ()
                                      : TricksStyle.gradientIdle ()));
                        anchors.fill: parent;
                    }
                    Loader {
                        id: ico;
                        enabled: col.enabled;
                        sourceComponent: col.group.icon;
                        anchors.margins: padding;
                    }
                    TextLabel {
                        id: lbl;
                        text: col.group.title;
                        anchors.margins: padding;
                    }
                }
                Binding {
                    target: col.group ["anchors"];
                    property: "fill";
                    value: container;
                }
                Binding {
                    target: col.group;
                    property: "visible";
                    value: (col.group === currentTab);
                }
                Stretcher {
                    id: placeholder;
                    height: paneSize;
                    visible: (col.group === currentTab);
                    ExtraAnchors.horizontalFill: parent;
                }
                Line { ExtraAnchors.horizontalFill: parent; }
            }
        }
    }
}
