/**
 * @file    ApplicationController.cpp
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   ApplicationController responsible for flow when windows are closed opened.
 *          When app starts fisrt window is opened allowing to choose how to proceed.
 *          If external video usb adapter not connected only one mode is avaiable -
 *          Filter mode.
 *          When external usb device available - app can work in dual mode.
 *          In dual window mode video is captured from usb device.
 *          First window shows filtered video and second plays original stream.
 *
 *          setNonCapturable - method disable self capturing to avoid infinite loop
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

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <windows.h>

#include "ApplicationController.h"
#include "ViewModels/StartWindowViewModel.h"
#include "ViewModels/FilterWindowViewModel.h"
#include "VideoManager/VideoDeviceManager.h"

void setNonCapturable(QQuickWindow *window)
{
    if (!window)
    {
        return;
    }

    HWND hwnd = (HWND)window->winId();

    // Set WS_EX_APPWINDOW flag, to display window on taskbar
    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    exStyle |= WS_EX_APPWINDOW;
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);

    // WDA_EXCLUDEFROMCAPTURE = 0x00000011
    SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);
}

ApplicationController::ApplicationController(QObject* parent)
    : m_applicationEngine(nullptr),
      m_currentWindow(nullptr),
      m_filterWindowViewModel(nullptr)
{
}

ApplicationController& ApplicationController::getInstance()
{
    static ApplicationController instance;
    return instance;
}

void ApplicationController::setEngine(QQmlApplicationEngine* engine)
{
    if ((nullptr != engine) && (engine != m_applicationEngine))
    {
        m_applicationEngine = engine;
    }
}

void ApplicationController::openStartView()
{
    VideoDeviceManager videoDeviceManager;
    if (nullptr == videoDeviceManager.getExternalVideoAdapter())
    {
        qDebug() << "External adapter not found!";
    }

    m_startWindowViewModel = std::make_unique<StartWindowViewModel>(videoDeviceManager);

    connect(m_startWindowViewModel.get(), &StartWindowViewModel::goToFilterWindow, this, &ApplicationController::openFilterWindow);

    // connection to capture pointer to created window
    connect(m_applicationEngine, &QQmlApplicationEngine::objectCreated, this, [this](QObject *obj, const QUrl &objUrl) {
        if (!obj && !objUrl.isEmpty())
        {
            qCritical() << "Failed to load QML object:" << objUrl;
            return;
        }
        // save start window pointer
        if (!m_currentWindow)
        {
            m_currentWindow = qobject_cast<QQuickWindow*>(obj);
        }
    }, Qt::SingleShotConnection);

    m_applicationEngine->rootContext()->setContextProperty("viewModel", m_startWindowViewModel.get());

    m_applicationEngine->loadFromModule("VideoMotionDetector", "Main");
}

void ApplicationController::closeStartView()
{
    if (!m_applicationEngine)
    {
        qWarning() << "Engine not initialized in ApplicationController!";
        return;
    }

    // close start window
    if (m_currentWindow)
    {
        m_currentWindow->close();
        m_currentWindow->deleteLater();
        m_currentWindow = nullptr;
    }

    // clear engine cache (to avoid collision if components have same name)
    m_applicationEngine->trimComponentCache();
}

void ApplicationController::openFilterWindow()
{
    closeStartView();

    // gst_init() is called in GStreamerWorker constructor
    GStreamerWorker& gstWorker = GStreamerWorker::getInstance();
    gstWorker.CreateGstPipeline();

    m_filterWindowViewModel = std::make_unique<FilterWindowViewModel>(gstWorker);
    m_applicationEngine->rootContext()->setContextProperty("appViewModel", m_filterWindowViewModel.get());

    // connection to capture pointer to created window
    connect(m_applicationEngine, &QQmlApplicationEngine::objectCreated, this, [this](QObject *obj, const QUrl &objUrl) {
        if (!obj && !objUrl.isEmpty())
        {
            qCritical() << "Failed to load QML object:" << objUrl;
            return;
        }

        QQuickItem* videoItem;
        QQuickWindow* rootObject = qobject_cast<QQuickWindow*>(obj);

        videoItem = rootObject->findChild<QQuickItem*> ("videoOutputItem");
        g_assert (videoItem);

        // set window not capturable
        setNonCapturable(rootObject);
        updateGstreamerContext(rootObject, videoItem);

    }, Qt::SingleShotConnection);

    m_applicationEngine->loadFromModule("VideoMotionDetector", "FilterScreenWindow");
}

void ApplicationController::updateGstreamerContext(QQuickWindow* filterWindow, QQuickItem* videoItem)
{
    // Update gstreamer context, after QML is initialized
    GstElement* sink = GStreamerWorker::getInstance().getSink();
    auto connection = QObject::connect(filterWindow, &QQuickWindow::beforeRendering, filterWindow, [sink, videoItem, filterWindow]() {
        // update captured frame size
        GStreamerWorker::getInstance().updateVideoFrameSize(filterWindow->x(),
                                                            filterWindow->y(),
                                                            filterWindow->width(),
                                                            filterWindow->height());

        if (sink) {
            // Set window directly into sink
            // It force qml6glsink to take context, created by QT
            g_object_set(sink, "widget", videoItem, nullptr);
            GStreamerWorker::getInstance().startPlaying();
        }
    }, Qt::SingleShotConnection);
}