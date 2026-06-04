/**
 * @file    main.cpp
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   Here app is created, initialized
 *          Main window has property, to avoid self caturing
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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQuickStyle>
#include <QQmlContext>

#include <windows.h>
#include <io.h>

#include "ApplicationController.h"
#include "GStreamerWorker/GStreamerWorker.h"
#include "Logger/ApplicationLogger.h"

void redirectAllOutputToFile(const char* fileName)
{
    // Create/Open the file
    QFile* file = new QFile(fileName);
    if (file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {

        // get windows handle
        HANDLE hFile = (HANDLE)_get_osfhandle(file->handle());

        // redirect system output (to capture linked DLL logs)
        SetStdHandle(STD_OUTPUT_HANDLE, hFile);
        SetStdHandle(STD_ERROR_HANDLE, hFile);

        // redirect C streams (printf/fprintf)
        _dup2(file->handle(), _fileno(stdout));
        _dup2(file->handle(), _fileno(stderr));

        // switch off buffering
        setvbuf(stdout, NULL, _IONBF, 0);
        setvbuf(stderr, NULL, _IONBF, 0);
    }
}

int main(int argc, char *argv[])
{
    // uncomment to have detailed gstreamer logs
    // qputenv("GST_DEBUG", "3");

    ApplicationLogger& logger = ApplicationLogger::getInstance();
#ifndef DEBUG
    qInstallMessageHandler(logger.messageHandler);
    // redirectAllOutputToFile(ApplicationLogger::getLogFileName());
#endif

    QQuickStyle::setStyle("Material");
    QGuiApplication app(argc, argv);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // create appcontroller
    ApplicationController& appController = ApplicationController::getInstance();
    appController.setEngine(&engine);
    appController.openStartView();

    return QCoreApplication::exec();
}
