/**
 * @file    ApplicationLogger.cpp
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   Just usual logger. Writes to file, but nice
 * @version 1.0
 * @date    2026-05-08
 *
 * @copyright Andrii Moroz (c) 2026
 * All rights reserved
 *
 * WARNING: This code is confidential and proprietary.
 * Unauthorized copying, distribution, or use of this file,
 * in any medium, is strictly prohibited without author permission.
 */

#include <iostream>

#include "ApplicationLogger.h"

const char* ApplicationLogger::LOG_FILE_NAME = "VideoMotionDetector.log";

ApplicationLogger::ApplicationLogger()
    : m_logFile(LOG_FILE_NAME)  // open it beyond constructor, You should know why
{
}

ApplicationLogger::~ApplicationLogger()
{
    m_outStream.flush(); // I believe next call will do it
    m_logFile.close();
}

ApplicationLogger& ApplicationLogger::getInstance()
{
    static ApplicationLogger instance;
    instance.initOnce();
    return instance;
}

const char* ApplicationLogger::getLogFileName()
{
    return LOG_FILE_NAME;
}

void ApplicationLogger::initOnce()
{
    std::call_once(m_initFlag, &ApplicationLogger::initLogger, this);
}

void ApplicationLogger::initLogger()
{
    if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        std::cerr << "ERROR: Can't open log file" << std::endl;
        return;
    }

    m_outStream.setDevice(&m_logFile);
}

void ApplicationLogger::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    getInstance().writeToFile(type, context, msg);
}

void ApplicationLogger::writeToFile(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    std::scoped_lock<std::mutex> lock(m_logMutex);

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString level;

    switch (type)
    {
        case QtDebugMsg:    level = "DEBUG";    break;
        case QtInfoMsg:     level = "INFO";     break;
        case QtWarningMsg:  level = "WARNING";  break;
        case QtCriticalMsg: level = "CRITICAL"; break;
        case QtFatalMsg:    level = "FATAL";    break;
    }

    // Write the message with source file and line info (optional)
    m_outStream << QString("[%1] [%2] %3 (%4:%5)").arg(timestamp, level, msg, context.file).arg(context.line) << Qt::endl;
}

void ApplicationLogger::logDebug(const std::string& debugMsg)
{
    writeToFile(QtDebugMsg, QMessageLogContext(), debugMsg.c_str());
}

void ApplicationLogger::logWarning(const std::string& warningMsg)
{
    writeToFile(QtWarningMsg, QMessageLogContext(), warningMsg.c_str());
}

void ApplicationLogger::logError(const std::string& errorgMsg)
{
    writeToFile(QtCriticalMsg, QMessageLogContext(), errorgMsg.c_str());
}

