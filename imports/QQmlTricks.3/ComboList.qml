import QtQuick 2.6;
import QQmlTricks 3.0;

Item {
    id: base;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: (stackControls.implicitWidth + arrow.width + base.padding * 3);
    implicitHeight: (stackControls.implicitHeight + base.padding * 2);

    property int    padding     : TricksStyle.spacingNormal;
    property bool   filterable  : false;
    property alias  rounding    : rect.radius;
    property alias  backColor   : rect.color;
    property string placeholder : "";

    property var       model               : undefined;
    property Component delegateForControl  : ComboListDelegateForControl {
        text: modelData;
        value: modelData;

        TextLabel {
            text: parent.text;
        }
    }
    property Component delegateForDropdown : ComboListDelegateForDropdown {
        text: modelData;
        value: modelData;

        TextLabel {
            text: parent.text;
            emphasis: parent.active;
        }
    }

    readonly property int count : repeater.count;

    property var value : undefined;

    readonly property string text : {
        var ret = "";
        for (var idx = 0; idx < repeater.count; ++idx) {
            var item = repeater.itemAt (idx);
            if (item ["value"] === value) {
                ret = item ["text"];
                break;
            }
        }
        return ret;
    }

    signal edited ();

    Rectangle {
        id: rect;
        radius: TricksStyle.roundness;
        enabled: base.enabled;
        antialiasing: radius;
        gradient: (enabled
                   ? (clicker.pressed ||
                      clicker.dropdownItem
                      ? TricksStyle.gradientPressed ()
                      : TricksStyle.gradientIdle (Qt.lighter (TricksStyle.colorClickable, clicker.containsMouse ? 1.15 : 1.0)))
                   : TricksStyle.gradientDisabled ());
        border {
            width: TricksStyle.lineSize;
            color: TricksStyle.colorBorder;
        }
        anchors.fill: parent;
    }
    StackContainer {
        id: stackControls;
        clip: true;
        anchors {
            left: (parent ? parent.left : undefined);
            right: arrow.left;
            margins: base.padding;
            verticalCenter: (parent ? parent.verticalCenter : undefined);
        }

        Repeater {
            id: repeater;
            model: base.model;
            delegate: Loader {
                id: loaderDumb;
                enabled: (isCurrent && base.enabled);
                opacity: (isCurrent ? 1.0 : 0.0);
                sourceComponent: base.delegateForControl;
                onInstanceChanged: {
                    if (instance !== null) {
                        instance ["model"]     = model;
                        instance ["modelData"] = model.modelData;
                    }
                }

                readonly property ComboListDelegateForControl instance : item;

                readonly property var    value     : (instance ? instance.value : undefined);
                readonly property string text      : (instance ? instance.text  : "");
                readonly property bool   isCurrent : (value === base.value);
            }
        }
    }
    MouseArea {
        id: clicker;
        enabled: base.enabled;
        hoverEnabled: TricksStyle.useHovering;
        anchors.fill: parent;
        onClicked: {
            if (dropdownItem) {
                destroyDropdown ();
            }
            else {
                createDropdown ();
            }
        }
        Component.onDestruction: {
            destroyDropdown ();
        }

        property Item dropdownItem : null;

        function createDropdown () {
            dropdownItem = compoDropdown.createObject (Introspector.window (base), { });
        }

        function destroyDropdown () {
            if (dropdownItem) {
                dropdownItem.destroy ();
                dropdownItem = null;
            }
        }
    }
    SymbolLoader {
        id: arrow;
        size: TricksStyle.fontSizeNormal;
        color: (enabled ? TricksStyle.colorForeground : TricksStyle.colorBorder);
        symbol: TricksStyle.symbolArrowDown;
        enabled: base.enabled;
        anchors {
            right: (parent ? parent.right : undefined);
            margins: base.padding;
            verticalCenter: (parent ? parent.verticalCenter : undefined);
        }
    }
    Component {
        id: compoDropdown;

        MouseArea {
            id: dimmer;
            z: 999999999;
            anchors.fill: parent;
            onWheel: { }
            onPressed: { clicker.destroyDropdown (); }
            onReleased: { }

            Item {
                id: mirror;
                x:      ref ["x"];
                y:      ref ["y"];
                width:  ref ["width"];
                height: ref ["height"];

                readonly property rect ref : (dimmer.width && dimmer.height
                                              ? base.mapToItem (parent, 0, 0, base.width, base.height)
                                              : Qt.rect (0,0,0,0));
            }
            Item {
                id: placeholderAbove;
                anchors {
                    top: dimmer.top;
                    left: mirror.left;
                    right: mirror.right;
                    bottom: mirror.top;
                    topMargin: TricksStyle.spacingNormal;
                    bottomMargin: -TricksStyle.lineSize;
                }
            }
            Item {
                id: placeholderUnder;
                anchors {
                    top: mirror.bottom;
                    left: mirror.left;
                    right: mirror.right;
                    bottom: dimmer.bottom;
                    topMargin: -TricksStyle.lineSize;
                    bottomMargin: TricksStyle.spacingNormal;
                }
            }
            Item {
                anchors.fill: frame.place;

                Component {
                    id: compoFilter;

                    TextBox {
                        id: inputFilter;
                        hasClear: true;
                        textHolder: qsTr ("Filter...");
                        ExtraAnchors.horizontalFill: parent;
                        Component.onCompleted: { forceActiveFocus (); }

                        Binding {
                            target: frame;
                            property: "filter";
                            value: inputFilter.text.toLowerCase ();
                        }
                    }
                }

                ScrollContainer {
                    id: frame;
                    y: (place === placeholderAbove ? (parent.height - (height * scale)) : 0);
                    width: Math.ceil (base.width);
                    height: (parent.height >= actualSize ? actualSize : parent.height);
                    scale: (mirror.width / base.width);
                    showBorder: true;
                    background: TricksStyle.colorWindow;
                    headerItem: (filterable ? compoFilter : null);
                    placeholder: (!repeaterDropdown.count ? qsTr ("Nothing here") : "");
                    transformOrigin: Item.TopLeft;

                    property string filter : "";

                    readonly property int itemSize    : (TricksStyle.fontSizeNormal + padding * 2);
                    readonly property int contentSize : (layout.height  + TricksStyle.lineSize * 2);
                    readonly property int minimumSize : ((itemSize * 3) + TricksStyle.lineSize * 2);
                    readonly property int actualSize  : Math.max (contentSize, minimumSize);

                    readonly property Item place : {
                        if (placeholderUnder.height >= actualSize) {
                            return placeholderUnder;
                        }
                        else if (placeholderAbove.height >= actualSize) {
                            return placeholderAbove;
                        }
                        else if (placeholderUnder.height >= minimumSize) {
                            return placeholderUnder;
                        }
                        else if (placeholderAbove.height >= minimumSize) {
                            return placeholderAbove;
                        }
                        else {
                            return placeholderUnder;
                        }
                    }

                    function matches (str) {
                        return (filter === "" || (str.toLowerCase ().indexOf (filter) >= 0));
                    }

                    Flickable {
                        contentHeight: layout.height;
                        flickableDirection: Flickable.VerticalFlick;

                        ColumnContainer {
                            id: layout;
                            ExtraAnchors.topDock: parent;

                            Repeater {
                                id: repeaterDropdown;
                                model: base.model;
                                delegate: MouseArea {
                                    id: dlg;
                                    visible: frame.matches (loader.value);
                                    hoverEnabled: TricksStyle.useHovering;
                                    implicitWidth: (loader.implicitWidth + padding * 2);
                                    implicitHeight: (loader.implicitHeight + padding * 2);
                                    onClicked: {
                                        base.value = loader.value;
                                        base.edited ();
                                        clicker.destroyDropdown ();
                                    }
                                    ExtraAnchors.horizontalFill: parent;

                                    Rectangle {
                                        color: TricksStyle.colorHighlight;
                                        opacity: 0.65;
                                        visible: parent.containsMouse;
                                        anchors.fill: parent;
                                        anchors.margins: TricksStyle.lineSize;
                                    }
                                    Loader {
                                        id: loader;
                                        clip: true;
                                        sourceComponent: base.delegateForDropdown;
                                        anchors {
                                            margins: base.padding;
                                            verticalCenter: (parent ? parent.verticalCenter : undefined);
                                        }
                                        ExtraAnchors.horizontalFill: parent;
                                        onInstanceChanged: {
                                            if (instance !== null) {
                                                instance.active    = Qt.binding (function () { return loader.isCurrent; });
                                                instance.model     = model;
                                                instance.modelData = model.modelData;
                                            }
                                        }

                                        readonly property ComboListDelegateForDropdown instance : item;

                                        readonly property var  value     : (instance ? instance.value : undefined);
                                        readonly property bool isCurrent : (value === base.value);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
