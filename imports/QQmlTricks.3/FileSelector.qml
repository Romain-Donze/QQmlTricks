import QtQuick 2.6;
import QQmlTricks 3.0;

FocusScope {
    id: base;

    property string folder     : FileSystem.homePath;
    property string rootFolder : FileSystem.rootPath;

    property bool showFiles  : true;
    property bool showHidden : false;

    property var nameFilters : [];

    property var iconProvider  : (function (entry) { return mimeHelper.getSvgIconPathForUrl (entry.url); });
    property var labelProvider : (function (entry) { return entry.name + (entry.isDir ? "/" : ""); });

    property int selectionType : (selectFile);

    readonly property int selectFile     : 1;
    readonly property int selectDir      : 2;
    readonly property int selectAllowNew : 4;

    readonly property var entries : FileSystem.list (folder, nameFilters, showHidden, showFiles);

    readonly property string currentPath : (inputName.value !== "" ? (folder + "/" + inputName.value) : "");

    signal fileDoubleClicked ();
    signal fileNameReturned ();

    function select (name) {
        var tmp = (name || "").trim ();
        inputName.text = tmp;
    }

    function goToFolder (path) {
        var tmp = (path || "").trim ();
        if (tmp !== "" && FileSystem.exists (tmp)) {
            folder = tmp;
        }
    }

    MimeIconsHelper { id: mimeHelper; }
    ColumnContainer {
        spacing: TricksStyle.spacingNormal;
        anchors.fill: parent;

        RowContainer {
            spacing: TricksStyle.spacingNormal;
            ExtraAnchors.horizontalFill: parent;

            ComboList {
                id: combo;
                model: FileSystem.drivesList;
                visible: (FileSystem.rootPath !== "/");
                delegateForControl: ComboListDelegateForControl {
                    text: modelData;
                    value: modelData;

                    TextLabel {
                        text: parent.text;
                    }
                }
                delegateForDropdown: ComboListDelegateForDropdown {
                    text: modelData;
                    value: modelData;

                    TextLabel {
                        text: parent.text;
                        emphasis: parent.active;
                    }
                }
                anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
                onEdited: {
                    if (value !== undefined && value !== "") {
                        rootFolder = value;
                        goToFolder (rootFolder);
                    }
                }

                Binding on value { value: (FileSystem.rootPath !== "/" ? folder.substring (0, 3) : "/"); }
            }
            ColumnContainer {
                anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
                Container.horizontalStretch: 1;

                TextLabel {
                    id: path;
                    text: {
                        var ret  = "";
                        if (btnParent.hasParent) {
                            ret = btnParent.parentDir;
                            if (FileSystem.hasParent (btnParent.parentDir)) {
                                ret += "/";
                            }
                        }
                        return ret;
                    }
                    elide: Text.ElideMiddle;
                    visible: (text !== "");
                    font.pixelSize: TricksStyle.fontSizeSmall;
                    ExtraAnchors.horizontalFill: parent;
                }
                TextLabel {
                    id: name;
                    text: (btnParent.hasParent ? FileSystem.baseName (folder) + "/" : folder);
                    color: TricksStyle.colorLink;
                    elide: Text.ElideMiddle;
                    font.pixelSize: TricksStyle.fontSizeNormal;
                    ExtraAnchors.horizontalFill: parent;
                }
            }
            TextButton {
                id: btnParent;
                text: qsTr ("Parent");
                enabled: hasParent;
                icon: SvgIconLoader {
                    icon: "actions/chevron-up";
                    size: TricksStyle.iconSize (1);
                    color: TricksStyle.colorForeground;
                }
                anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
                onClicked: { goToFolder (parentDir); }

                readonly property bool   hasParent : FileSystem.hasParent (folder);
                readonly property string parentDir : FileSystem.parentDir (folder);
            }
        }
        ScrollContainer {
            placeholder: (list.count === 0 ? qsTr ("Empty.") : "");
            ExtraAnchors.horizontalFill: parent;
            Container.verticalStretch: 1;

            ListView {
                id: list;
                model: entries;
                delegate: MouseArea {
                    id: delegate;
                    height: (Math.max (label.height, img.height) + label.anchors.margins * 2);
                    ExtraAnchors.horizontalFill: parent;
                    onClicked: {
                        if (entry.isDir) {
                            select ((selectionType & selectDir) ? entry.name : "");
                        }
                        else if (entry.isFile) {
                            select ((selectionType & selectFile) ? entry.name : "");
                        }
                        else { }
                    }
                    onDoubleClicked: {
                        if (entry.isDir) {
                            select ("");
                            goToFolder (entry.path);
                        }
                        else {
                            select ((selectionType & selectFile) ? entry.name : "");
                            fileDoubleClicked ();
                        }
                    }

                    readonly property FileSystemModelEntry entry : modelData;

                    readonly property bool isCurrent : (entry.path === currentPath);

                    Rectangle {
                        color: TricksStyle.colorHighlight;
                        opacity: 0.35;
                        visible: delegate.isCurrent;
                        anchors.fill: parent;
                    }
                    Line {
                        opacity: 0.65;
                        ExtraAnchors.bottomDock: parent;
                    }
                    SvgIconLoader {
                        id: img;
                        size: TricksStyle.realPixels (24);
                        icon: iconProvider (delegate.entry);
                        anchors {
                            left: parent.left;
                            margins: TricksStyle.spacingNormal;
                            verticalCenter: (parent ? parent.verticalCenter : undefined);
                        }
                    }
                    TextLabel {
                        id: label;
                        text: labelProvider (delegate.entry);
                        elide: Text.ElideRight;
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere;
                        maximumLineCount: 3;
                        anchors {
                            left: img.right;
                            right: parent.right;
                            margins: TricksStyle.spacingNormal;
                            verticalCenter: (parent ? parent.verticalCenter : undefined);
                        }
                    }
                }
            }
        }
        RowContainer {
            spacing: TricksStyle.spacingNormal;
            ExtraAnchors.horizontalFill: parent;

            TextLabel {
                text: qsTr ("Name :");
                anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
            }
            TextBox {
                id: inputName;
                focus: true;
                enabled: (selectionType & selectAllowNew);
                anchors.verticalCenter: (parent ? parent.verticalCenter : undefined);
                Container.horizontalStretch: 1;
                onAccepted: { base.fileNameReturned (); }

                readonly property string value : (text.trim ());
            }
        }
    }
}
