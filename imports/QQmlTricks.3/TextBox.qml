import QtQuick 2.6;
import QQmlTricks 3.0;

FocusScope {
    id: base;
    implicitWidth: Math.ceil (input.implicitWidth + padding * 2);
    implicitHeight: Math.ceil (Math.max (input.implicitHeight, holder.implicitHeight) + padding * 2);

    property int   padding    : TricksStyle.spacingNormal;
    property bool  hasClear   : false;
    property bool  isPassword : false;
    property bool  emphasis   : false;
    property color backColor  : TricksStyle.colorEditable;
    property color textColor  : TricksStyle.colorForeground;
    property alias text       : input.text;
    property alias readOnly   : input.readOnly;
    property alias textFont   : input.font;
    property alias textAlign  : input.horizontalAlignment;
    property alias textHolder : holder.text;
    property alias inputMask  : input.inputMask;
    property alias validator  : input.validator;
    property alias acceptable : input.acceptableInput;
    property alias rounding   : rect.radius;
    property alias value      : input.text;

    readonly property bool isEmpty : (text.trim () === "");

    signal accepted ();

    function selectAll () {
        input.selectAll ();
    }

    function clear () {
        input.text = "";
    }

    Rectangle {
        id: rect;
        radius: TricksStyle.roundness;
        enabled: base.enabled;
        visible: !readOnly;
        antialiasing: radius;
        gradient: (enabled ? TricksStyle.gradientEditable (backColor) : TricksStyle.gradientDisabled ());
        border {
            width: (input.activeFocus && !readOnly ? TricksStyle.lineSize * 2 : TricksStyle.lineSize);
            color: (input.activeFocus ? TricksStyle.colorSelection : TricksStyle.colorBorder);
        }
        anchors.fill: parent;
    }
    Item {
        clip: (input.contentWidth > input.width);
        enabled: base.enabled;
        anchors {
            fill: parent;
            margins: rect.border.width;
        }

        TextInput {
            id: input;
            focus: true;
            color: (enabled ? textColor : TricksStyle.colorBorder);
            enabled: base.enabled;
            selectByMouse: true;
            selectionColor: TricksStyle.colorSelection;
            selectedTextColor: TricksStyle.colorEditable;
            activeFocusOnPress: true;
            echoMode: (isPassword ? TextInput.Password : TextInput.Normal);
            font {
                family: TricksStyle.fontName;
                weight: (emphasis ? Font.Bold : (TricksStyle.useSlimFonts ? Font.Light : Font.Normal));
                pixelSize: TricksStyle.fontSizeNormal;
            }
            anchors {
                margins: base.padding;
                verticalCenter: (parent ? parent.verticalCenter : undefined);
            }
            ExtraAnchors.horizontalFill: parent;
            onAccepted: { base.accepted (); }
        }
        MouseArea {
            enabled: base.enabled;
            visible: (input.text !== "" && hasClear);
            implicitWidth: height;
            ExtraAnchors.rightDock: parent;
            onClicked: {
                base.focus = false;
                clear ();
            }

            Rectangle {
                rotation: -90;
                implicitWidth: (parent.width)
                implicitHeight: (parent.height * 2);
                gradient: Gradient {
                    GradientStop { position: 0.0; color: TricksStyle.colorNone;  }
                    GradientStop { position: 0.5; color: backColor; }
                    GradientStop { position: 1.0; color: backColor; }
                }
                anchors {
                    verticalCenter: (parent ? parent.verticalCenter : undefined);
                    horizontalCenter: (parent ? parent.left : undefined);
                }
            }
            SymbolLoader {
                id: cross;
                size: TricksStyle.fontSizeNormal;
                color: (enabled ? TricksStyle.colorForeground : TricksStyle.colorBorder);
                symbol: TricksStyle.symbolCross;
                enabled: base.enabled;
                anchors.centerIn: parent;
            }
        }
    }
    TextLabel {
        id: holder;
        color: TricksStyle.opacify (TricksStyle.colorBorder, 0.85);
        enabled: base.enabled;
        visible: (!input.activeFocus && input.text.trim ().length === 0 && !readOnly);
        horizontalAlignment: input.horizontalAlignment;
        font {
            weight: Font.Normal;
            family: input.font.family;
            pixelSize: input.font.pixelSize;
        }
        anchors {
            margins: base.padding;
            verticalCenter: (parent ? parent.verticalCenter : undefined);
        }
        ExtraAnchors.horizontalFill: parent;
    }
}
