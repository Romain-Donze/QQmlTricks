import QtQuick 2.6;
import QQmlTricks 3.0;

FocusScope {
    id: base;
    anchors.fill: parent;
    Component.onCompleted: {
        var win = Introspector.window (this);
        if (win !== null) {
            priv.previouslyFocusedItem = win.activeFocusItem;
        }
        forceActiveFocus ();
    }

    property string title : "";

    property var message : undefined;

    property int buttons : (buttonOk | buttonCancel);

    property int minWidth : 400;
    property int maxWidth : 600;

    readonly property int buttonNone   : 0;
    readonly property int buttonOk     : (1 << 0);
    readonly property int buttonYes    : (1 << 1);
    readonly property int buttonNo     : (1 << 2);
    readonly property int buttonCancel : (1 << 3);
    readonly property int buttonAccept : (1 << 4);
    readonly property int buttonReject : (1 << 5);

    default property alias content : container.children;

    function hide () {
        if (priv.previouslyFocusedItem !== null) {
            priv.previouslyFocusedItem.forceActiveFocus ();
        }
        base.destroy ();
    }

    function shake () {
        animShake.start ();
    }

    signal buttonClicked (int buttonType);

    QtObject {
        id: priv;

        property Item previouslyFocusedItem : null;
    }
    MouseArea {
        id: blocker;
        hoverEnabled: TricksStyle.useHovering;
        anchors.fill: parent;
        onWheel: { }
        onPressed: { }
        onReleased: { }
    }
    Rectangle {
        id: dimmer;
        color: TricksStyle.colorEditable;
        opacity: 0.45;
        anchors.fill: parent;
    }
    ColumnContainer {
        id: layout;
        width: TricksStyle.clamp (implicitWidth, minWidth, maxWidth);
        spacing: TricksStyle.spacingBig;
        anchors.centerIn: parent;

        Rectangle {
            id: frame;
            color: TricksStyle.colorSecondary;
            radius: TricksStyle.roundness;
            antialiasing: radius;
            border {
                width: TricksStyle.lineSize;
                color: TricksStyle.colorSelection;
            }
            anchors {
                fill: parent;
                margins: -TricksStyle.spacingBig;
            }
            Container.ignored: true;
        }
        SequentialAnimation on anchors.horizontalCenterOffset {
            id: animShake;
            loops: 2;
            running: false;
            alwaysRunToEnd: true;

            PropertyAnimation {
                to: 30;
                duration: 40;
            }
            PropertyAnimation {
                to: -30;
                duration: 40;
            }
            PropertyAnimation {
                to: 0;
                duration: 40;
            }
        }
        TextLabel {
            id: lblTitle;
            text: base.title;
            visible: (text.trim () !== "");
            font.pixelSize: TricksStyle.fontSizeTitle;
            ExtraAnchors.horizontalFill: parent;
        }
        Line {
            visible: lblTitle.visible;
            ExtraAnchors.horizontalFill: parent;
        }
        TextLabel {
            id: lblMsg;
            text: {
                var ret = "";
                if (base.message !== undefined) {
                    if (Array.isArray (base.message)) {
                        ret = base.message.join ("\n");
                    }
                    else {
                        ret = base.message.toString ();
                    }
                }
                return ret;
            }
            visible: (text.trim () !== "");
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere;
            horizontalAlignment: Text.AlignJustify;
            ExtraAnchors.horizontalFill: parent;
        }
        ColumnContainer {
            id: container;
            visible: (children.length > 0);
            spacing: TricksStyle.spacingBig;
            ExtraAnchors.horizontalFill: parent;
        }
        Line {
            visible: buttons;
            ExtraAnchors.horizontalFill: parent;
        }
        GridContainer {
            cols: capacity;
            visible: buttons;
            capacity: repeater.entries.length;
            colSpacing: TricksStyle.spacingNormal;
            anchors.horizontalCenter: parent.horizontalCenter;

            Repeater {
                id: repeater;
                model: entries;
                delegate: TextButton {
                    text: (modelData ["label"] || "");
                    icon: SymbolLoader {
                        size: TricksStyle.fontSizeNormal;
                        color: TricksStyle.colorForeground;
                        symbol: (modelData ["symbol"] || null);
                    }
                    onClicked: { base.buttonClicked (modelData ["type"] || 0); }
                }

                readonly property var entries : {
                    var ret = [];
                    var tmp = [
                                { "type" : buttonCancel, "label" : qsTr ("Cancel"), "symbol" : TricksStyle.symbolCross },
                                { "type" : buttonNo,     "label" : qsTr ("No"),     "symbol" : TricksStyle.symbolCross },
                                { "type" : buttonReject, "label" : qsTr ("Reject"), "symbol" : TricksStyle.symbolCross },
                                { "type" : buttonAccept, "label" : qsTr ("Accept"), "symbol" : TricksStyle.symbolCheck },
                                { "type" : buttonYes,    "label" : qsTr ("Yes"),    "symbol" : TricksStyle.symbolCheck },
                                { "type" : buttonOk,     "label" : qsTr ("Ok"),     "symbol" : TricksStyle.symbolCheck },
                            ];
                    tmp.forEach (function (item) {
                        if (buttons & (item ["type"] || 0)) {
                            ret.push (item);
                        }
                    });
                    return ret;
                }
            }
        }
    }
}
