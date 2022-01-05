import QtQuick 2.6;
import QQmlTricks 3.0;

FocusScope {
    id: base;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: Math.ceil (input.implicitWidth + padding * 2);
    implicitHeight: Math.ceil (Math.max (input.implicitHeight, holder.implicitHeight) + padding * 2);

    property int   padding      : TricksStyle.spacingNormal;
    property bool  allowNewLine : true;
    property alias text         : input.text;
    property alias readOnly     : input.readOnly;
    property alias textFont     : input.font;
    property alias textColor    : input.color;
    property alias textAlign    : input.horizontalAlignment;
    property alias textHolder   : holder.text;
    property alias rounding     : rect.radius;

    function selectAll () {
        input.selectAll ();
    }

    function clear () {
        input.text = "";
    }

    signal returnPressed ();

    Rectangle {
        id: rect;
        radius: TricksStyle.roundness;
        enabled: base.enabled;
        visible: !readOnly;
        antialiasing: radius;
        gradient: (enabled ? TricksStyle.gradientEditable () : TricksStyle.gradientDisabled ());
        border {
            width: (input.activeFocus ? TricksStyle.lineSize * 2 : TricksStyle.lineSize);
            color: (input.activeFocus ? TricksStyle.colorSelection : TricksStyle.colorBorder);
        }
        anchors.fill: parent;
    }
    Item {
        clip: (input.contentHeight > input.height);
        enabled: base.enabled;
        anchors {
            fill: rect;
            margins: rect.border.width;
        }

        TextEdit {
            id: input;
            focus: true;
            color: (enabled ? TricksStyle.colorForeground: TricksStyle.colorBorder);
            enabled: base.enabled;
            wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere;
            selectByMouse: true;
            selectionColor: TricksStyle.colorSelection;
            selectedTextColor: TricksStyle.colorEditable;
            activeFocusOnPress: true;
            font {
                family: TricksStyle.fontName;
                weight: (TricksStyle.useSlimFonts ? Font.Light : Font.Normal);
                pixelSize: TricksStyle.fontSizeNormal;
            }
            anchors {
                fill: parent;
                margins: base.padding;
            }
            Keys.onReturnPressed: {
                if (!allowNewLine) {
                    returnPressed ();
                }
                else {
                    event.accepted = false;
                }
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
            top: parent.top;
            left: parent.left;
            right: parent.right;
            margins: base.padding;
        }
    }
}
