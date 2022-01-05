import QtQuick 2.6;
import QQmlTricks 3.0;

Text {
    color: (enabled ? TricksStyle.colorForeground : TricksStyle.colorBorder);
    linkColor: (enabled ? TricksStyle.colorLink : TricksStyle.colorBorder);
    textFormat: Text.PlainText;
    renderType: (TricksStyle.useNativeText ? Text.NativeRendering : Text.QtRendering);
    verticalAlignment: Text.AlignVCenter;
    font {
        family: TricksStyle.fontName;
        weight: (emphasis ? Font.Bold : (TricksStyle.useSlimFonts ? Font.Light : Font.Normal));
        pixelSize: TricksStyle.fontSizeNormal;
    }

    property bool emphasis : false;
}
