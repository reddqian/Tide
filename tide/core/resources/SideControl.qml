import QtQuick 2.0
import "qrc:/qml/core/style.js" as Style

Item {
    id: sideControl

    width: childrenRect.width

    anchors.verticalCenter: parent.verticalCenter
    anchors.left: parent.left

    // Can't be made a top-level item; this results in unexplained incorrect
    // centering of buttons inside the Canvas.
    SideButton {
        id: buttonShape
        width: buttons.width + 2 * Style.buttonsPaddingLarge
        color: Style.surfaceControlsColor
        dropShadow: true
    }
    Column {
        id: buttons
        anchors.centerIn: buttonShape
        anchors.horizontalCenterOffset: -0.5 * buttonShape.dropShadowWidth
        LaunchButton {
            id: launchButton
            visible: !displaygroup.hasFocusedWindows
                     && !displaygroup.hasFullscreenWindows
            size: Style.buttonsSizeLarge
        }
        ContentActionButton {
            id: contentActionButton
        }
        ClockButton {
            visible: !launchButton.visible && !contentActionButton.visible
            size: Style.buttonsSizeLarge
        }
        HorizontalButtonSeparator {
            width: Style.buttonsSizeLarge
        }
        ExitButton {
            id: exitButton
            visible: displaygroup.hasFocusedWindows
                     || displaygroup.hasFullscreenWindows
            size: Style.buttonsSizeLarge
        }
        FocusButton {
            id: focusButton
            visible: !exitButton.visible && !displaygroup.empty
                     && !displaygroup.hasVisiblePanels
            size: Style.buttonsSizeLarge
            onClicked: sidecontroller.toggleFocusAll()
        }
        LockButton {
            visible: !exitButton.visible && !focusButton.visible
            size: Style.buttonsSizeLarge
        }
    }
}
