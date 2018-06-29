import QtQuick 2.0
import Tide 1.0
import "qrc:/qml/core/style.js" as Style

Item {
    property alias sideControl: sideControl
    property alias streamNotificationArea: streamNotificationArea
    property alias contextMenu: contextMenu

    SideControl {
        id: sideControl
        z: Style.sideControlZorder
        visible: options.showControlArea
    }

    StreamNotificationArea {
        id: streamNotificationArea
        z: Style.streamNotificationZorder
        anchors.top: parent.verticalCenter
        anchors.left: parent.left
    }

    ContextMenu {
        id: contextMenu
        z: Style.sideControlZorder

        x: contextmenu.position.x
        y: contextmenu.position.y
        visible: contextmenu.visible
    }
}
