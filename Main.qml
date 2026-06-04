/**
 * @file    Main.qml
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   Here is main application window with all controls
 *
 * @version 1.0
 * @date    2026-06-03
 *
 * @copyright Andrii Moroz (c) 2026
 * All rights reserved
 *
 * WARNING: This code is confidential and proprietary.
 * Unauthorized copying, distribution, or use of this file,
 * in any medium, is strictly prohibited without author permission.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

Window {
    id: mainWindow
    width: 400
    height: 300
    visible: true
    title: "Video Fly"

    // Nice grey background color for the main window
    color: "#E0E0E0"

    // Grid layout ensures all child buttons match the column width
    Grid {
        id: buttonContainer
        anchors.centerIn: parent
        columns: 1
        spacing: 20

        // Explicitly force the grid layout width to size up to the largest button inside it
        width: Math.max(filterButton.implicitWidth, dualScreenButton.implicitWidth)

        Button {
            id: filterButton
            text: "Open filter view"

            // Force this button to take full width of the container
            width: parent.width

            hoverEnabled: true

            background: Rectangle {
                // Dark grey when normal, slightly lighter when hovered/pressed, transparent if disabled
                color: filterButton.enabled ?
                       (filterButton.down ? "#616161" :
                       (filterButton.hovered ? "#424242" : "#212121")) : "#BDBDBD"
                radius: 4
            }

            contentItem: Text {
                text: filterButton.text
                font.pixelSize: 14
                color: filterButton.enabled ? "white" : "#757575" // White text when active
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                viewModel.onFilterButtonClicked()
            }
        }

        Button {
            id: dualScreenButton
            text: "Open dual screen view"

            // Force this button to take full width of the container
            width: parent.width

            enabled: (viewModel.screensAmount >= 2) && (viewModel.usbVideoEnbled === true)

            background: Rectangle {
                // Dark grey when normal, slightly lighter when hovered/pressed, muted grey if disabled
                color: dualScreenButton.enabled ? (dualScreenButton.down ? "#424242" : "#212121") : "#9E9E9E"
                radius: 4
                // Visual boundary indicator for disabled state
                border.color: dualScreenButton.enabled ? "transparent" : "#757575"
                border.width: 1
            }

            contentItem: Text {
                text: dualScreenButton.text
                font.pixelSize: 14
                color: dualScreenButton.enabled ? "white" : "#E0E0E0" // White text when active, light grey when disabled
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}