/**
 * @file    Main.qml
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   Here is main application window with all controls
 *
 * @version 1.0
 * @date    2026-05-05
 *
 * @copyright Andrii Moroz (c) 2026
 * All rights reserved
 *
 * WARNING: This code is confidential and proprietary.
 * Unauthorized copying, distribution, or use of this file,
 * in any medium, is strictly prohibited without author permission.
 */

import QtQuick
import QtMultimedia
import QtQuick.Controls
import QtQuick.Window 2.2
import org.freedesktop.gstreamer.Qt6GLVideoItem 1.0

Window {
    id: currentWindowId

    x: appViewModel.X0
    y: appViewModel.Y0
    width: appViewModel.frameWidth
    height: appViewModel.frameHeight
    maximumWidth: 1920
    maximumHeight: 1080

    onWidthChanged: resizeTimer.restart()
    onHeightChanged: resizeTimer.restart()
    onXChanged: resizeTimer.restart()
    onYChanged: resizeTimer.restart()

    visible: true
    flags: Qt.FramelessWindowHint

    // Edge thickness for mouse detection
    property int resizeMargin: 8

    title: qsTr("VideoMotionDetector")

    // timer for handling resize/dragging
    Timer {
        id: resizeTimer
        interval: 200 // delay in msec
        repeat: false
        onTriggered: {
            // don't allow coordinates become negative, when window is dragged
            let safeX = Math.max(0, currentWindowId.x);
            let safeY = Math.max(0, currentWindowId.y);

            // rounded values
            let safeWidth = Math.floor(currentWindowId.width / 16) * 16;
            let safeHeight = Math.floor(currentWindowId.height / 2) * 2;

            console.log("Updating GStreamer width: ", safeWidth, " height: ", safeHeight);

            // update viewModel
            appViewModel.frameWidth = safeWidth;
            appViewModel.frameHeight = safeHeight;
            appViewModel.X0 = safeX
            appViewModel.Y0 = safeY
        }
    }

    // slider for changing delay
    Column {
        id: columnId
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 20
        spacing: 20
        opacity: delaySliderId.pressed ? 1.0 : 0.5
        z: 1000

        Text {
            text: "Delay: " + appViewModel.sliderValue
            font.pixelSize: 20
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // timer to prevent frequent updates
        Timer {
            id: updateDelayTimer
            interval: 200 // not often than 200 msec
            repeat: false
            onTriggered: {
                appViewModel.sliderValue = delaySliderId.value
                console.log("Value updated to:", delaySliderId.value)
            }
        }

        Slider {
            id: delaySliderId
            from: 20
            to: 200
            stepSize: 10
            value: appViewModel.sliderValue
            onValueChanged: {
                // if cjanged by dragging, not from code
                if (pressed) {
                    updateDelayTimer.restart()
                }
            }

            // Slider line
            background: Rectangle {
                x: delaySliderId.leftPadding
                y: delaySliderId.topPadding + delaySliderId.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 4
                width: delaySliderId.availableWidth
                height: implicitHeight
                radius: 2
                color: "#60525252" // Grey
            }

            // slider handle
            handle: Rectangle {
                x: delaySliderId.leftPadding + delaySliderId.visualPosition * (delaySliderId.availableWidth - width)
                y: delaySliderId.topPadding + delaySliderId.availableHeight / 2 - height / 2
                implicitWidth: 20
                implicitHeight: 20
                radius: 10 // Circle
                color: "white"
                border.color: "#bdbebf"
            }
        }
    }

    GstGLQt6VideoItem {
            id: videoOutput
            objectName: "videoOutputItem" // Used by C++ to find the item
            anchors.fill: parent
        }

    MouseArea {
        anchors.fill: parent
        onPressed: (mouse) => {
            // Allows moving the window by dragging anywhere
            if (mouse.button === Qt.LeftButton) {
                currentWindowId.startSystemMove();
            }
        }
    }

    MouseArea {
        id: resizeAreaId
        anchors.fill: parent
        hoverEnabled: true

        // Internal function to determine which edge we are near
        function getEdges(x, y) {
            let edges = 0;
            if (x < resizeMargin) edges |= Qt.LeftEdge;
            if (x > width - resizeMargin) edges |= Qt.RightEdge;
            if (y < resizeMargin) edges |= Qt.TopEdge;
            if (y > height - resizeMargin) edges |= Qt.BottomEdge;
            return edges;
        }

        // Update cursor shape based on edge
        onPositionChanged: (mouse) => {
            let edges = getEdges(mouse.x, mouse.y);
            if (edges === (Qt.LeftEdge | Qt.TopEdge) || edges === (Qt.RightEdge | Qt.BottomEdge))
                cursorShape = Qt.SizeFDiagCursor;
            else if (edges === (Qt.RightEdge | Qt.TopEdge) || edges === (Qt.LeftEdge | Qt.BottomEdge))
                cursorShape = Qt.SizeBDiagCursor;
            else if (edges & (Qt.LeftEdge | Qt.RightEdge))
                cursorShape = Qt.SizeHorCursor;
            else if (edges & (Qt.TopEdge | Qt.BottomEdge))
                cursorShape = Qt.SizeVerCursor;
            else
                cursorShape = Qt.ArrowCursor;
        }

        onPressed: (mouse) => {
            let edges = getEdges(mouse.x, mouse.y);
            if (edges !== 0) {
                currentWindowId.startSystemResize(edges);
            } else if (mouse.button === Qt.LeftButton) {
                currentWindowId.startSystemMove(); // Drag to move if not resizing
            }
        }
    }

    // close button
    Rectangle {
        id: closeButtonId
        width: 30
        height: 30
        color: closeMouseArea.containsMouse ? "#BFe74c3c" : "#60525252"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5
        radius: 4

        Text {
            text: "✕"
            color: "white"
            anchors.centerIn: parent
            font.pixelSize: 16
        }

        MouseArea {
            id: closeMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: currentWindowId.close()
        }
    }
}
