/**
 * @file    BusyIndicator.qml
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   Busy indicator is shown when windows loads for long time
 *
 * @version 1.0
 * @date    2026-06-16
 *
 * @copyright Andrii Moroz (c) 2026
 * All rights reserved
 *
 * WARNING: This code is confidential and proprietary.
 * Unauthorized copying, distribution, or use of this file,
 * in any medium, is strictly prohibited without author permission.
 */

import QtQuick
import QtQuick.Window

Window {
    id: busyWindow
    width: 150
    height: 150
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    color: "transparent"
    x: (Screen.width - width) / 2
    y: (Screen.height - height) / 2

    // Біла кругла плашка-фон
    Rectangle {
        anchors.fill: parent
        color: "white"
        radius: 16
        border.color: "#E0E0E0"
        border.width: 1

        // Контейнер для нашого індикатора
        Item {
            id: spinnerContainer
            anchors.centerIn: parent
            width: 70
            height: 70

            // Малюємо кільце за допомогою стандартних властивостей Rectangle
            // Це гарантує, що воно завантажиться миттєво в пам'ять GPU
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                radius: width / 2
                border.color: "#3498db" // Синій колір індикатора
                border.width: 6

                // Робимо з кільця "дугу" (розріз), щоб було видно обертання
                // Для цього накладаємо маску прозорості або використовуємо простий трюк з градієнтом
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#3498db" }
                    GradientStop { position: 0.7; color: "#3498db" }
                    GradientStop { position: 1.0; color: "transparent" }
                }
            }

            // Цей аніматор працює ОБОРОТНО на Render Thread (GPU).
            // Він ігнорує зависання головного C++ потоку програми!
            RotationAnimator {
                target: spinnerContainer
                running: busyWindow.visible
                from: 0
                to: 360
                loops: Animation.Infinite
                duration: 1000 // Швидкість обертання (1 секунда на коло)
            }
        }
    }
}