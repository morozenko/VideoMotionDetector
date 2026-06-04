/**
 * @file    ApplicationController.h
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   ApplicationController responsible for flow when windows are closed opened.
 *          When app starts fisrt window is opened allowing to choose how to proceed.
 *          If external video usb adapter not connected only one mode is avaiable -
 *          Filter mode.
 *          When external usb device available - app can work in dual mode.
 *          In dual window mode video is captured from usb device.
 *          First window shows filtered video and second plays original stream.
 *
 *
 * @version 1.0
 * @date    2026-06-04
 *
 * @copyright Andrii Moroz (c) 2026
 * All rights reserved
 *
 * WARNING: This code is confidential and proprietary.
 * Unauthorized copying, distribution, or use of this file,
 * in any medium, is strictly prohibited without author permission.
 */

#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

class QQmlApplicationEngine;
class StartWindowViewModel;
class FilterWindowViewModel;

class ApplicationController : public QObject
{
    Q_OBJECT

public:
    ApplicationController(const ApplicationController& controller) = delete;
    ApplicationController& operator= (ApplicationController& other) = delete;

    static ApplicationController& getInstance();

    void setEngine(QQmlApplicationEngine* engine);
    void openStartView();

public slots:
    void openFilterWindow();

private:
    explicit ApplicationController(QObject* parent = nullptr);
    ~ApplicationController() override = default;
    void closeStartView();
    void updateGstreamerContext(QQuickWindow* filterWindow, QQuickItem* videoItem);

private:
    QQmlApplicationEngine* m_applicationEngine;
    QQuickWindow* m_currentWindow;

    std::unique_ptr<StartWindowViewModel> m_startWindowViewModel;
    std::unique_ptr<FilterWindowViewModel> m_filterWindowViewModel;
};

#endif // APPLICATIONCONTROLLER_H
