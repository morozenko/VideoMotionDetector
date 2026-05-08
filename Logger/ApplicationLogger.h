/**
 * @file    ApplicationLogger.h
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   Just usual logger. Writes to file, but nice.
 *          writeToFile has lock and do all job.
 *          Other methods use it to store message to file
 *          Call getInstance and use it.
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

#ifndef APPLICATIONLOGGER_H
#define APPLICATIONLOGGER_H

#include <QFile>
#include <mutex>

class ApplicationLogger
{
public:
    static ApplicationLogger& getInstance();

    ~ApplicationLogger();

    ApplicationLogger(const ApplicationLogger&) = delete;
    ApplicationLogger& operator=(const ApplicationLogger&) = delete;

    static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);
    void writeToFile(QtMsgType type, const QMessageLogContext& context, const QString& msg);

    void logError(const std::string& errorMsg);
    void logWarning(const std::string& warningMsg);
    void logDebug(const std::string& debugMsg);

private:
    ApplicationLogger();
    void initOnce();
    void initLogger();

private:
    std::once_flag m_initFlag;
    std::mutex m_logMutex;

    QFile m_logFile;
    QTextStream m_outStream;
};

#endif // APPLICATIONLOGGER_H
