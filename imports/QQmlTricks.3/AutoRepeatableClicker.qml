import QtQuick 2.6;
import QQmlTricks 3.0;

Item {
    id: base;
    onEnabledChanged: {
        if (!enabled) {
            timerAutoRepeatDelay.stop ();
            timerAutoRepeatInterval.stop ();
        }
    }

    property int   repeatDelay    : 650;
    property int   repeatInterval : 30;
    property int   sensitiveHalo  : 0;
    property bool  autoRepeat     : false;
    property alias pressed        : clicker.pressed;
    property alias hoverEnabled   : clicker.hoverEnabled;
    property alias containsMouse  : clicker.containsMouse;

    signal clicked (bool isRepeated);

    MouseArea {
        id: clicker;
        anchors.fill: parent;
        anchors.margins: -sensitiveHalo;
        onPressed: {
            if (autoRepeat) {
                timerAutoRepeatDelay.start ();
            }
        }
        onReleased: {
            if (autoRepeat) {
                if (timerAutoRepeatDelay.running) {
                    base.clicked (false);
                }
                timerAutoRepeatDelay.stop ();
                timerAutoRepeatInterval.stop ();
            }
            else {
                base.clicked (false);
            }
        }

        Timer {
            id: timerAutoRepeatDelay;
            repeat: false;
            running: false;
            interval: repeatDelay;
            onTriggered: { timerAutoRepeatInterval.start (); }
        }
        Timer {
            id: timerAutoRepeatInterval;
            repeat: true;
            running: false;
            interval: repeatInterval;
            triggeredOnStart: true;
            onTriggered: { base.clicked (true); }
        }
    }
}
