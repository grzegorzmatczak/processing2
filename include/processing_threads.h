#pragma once

#include <memory>

#include <QObject>
#include "processing.h"
#include "processing_global.h"

#include <opencv2/core/core.hpp>
#include "logger_thread.hpp"
struct _dataThread
{
    _dataThread()
    {
        mLogger =
            std::make_unique<logger::LoggerThread>(logger::LogType::CONFIG, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
        mLogger->printThreaded(QString("_dataThread()"), logger::LogLevel::MEDIUM, __FUNCTION__);
    }
    _dataThread(cv::Mat& frame, const QString& str)
        : processing(frame)
        , testStr(str)

    {
        mLogger =
            std::make_unique<logger::LoggerThread>(logger::LogType::CONFIG, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
        mLogger->printThreaded(QString("_dataThread(cv::Mat& frame, const QString& str)"), logger::LogLevel::MEDIUM, __FUNCTION__);
    }
    ~_dataThread()
    {}
    _dataThread(const _dataThread& other)
        : processing(other.processing)
        , testStr(other.testStr)
    {
        mLogger =
            std::make_unique<logger::LoggerThread>(logger::LogType::CONFIG, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
        mLogger->printThreaded(QString("_dataThread(const _dataThread& other)"), logger::LogLevel::MEDIUM, __FUNCTION__);
    }
    _dataThread& operator=(const _dataThread& other)
    {
        mLogger =
            std::make_unique<logger::LoggerThread>(logger::LogType::CONFIG, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
        mLogger->printThreaded(QString("_dataThread& operator=(const _dataThread& other)"), logger::LogLevel::MEDIUM, __FUNCTION__);
        if(this==&other)
            return *this;
        return *this;
    }
    _dataThread(_dataThread&& other) noexcept
        : mLogger(std::move(other.mLogger))
        , processing(std::move(other.processing))
    {
        mLogger->printThreaded(QString("_dataThread(_dataThread&& other) noexcept"), logger::LogLevel::MEDIUM, __FUNCTION__);
    }
    _dataThread& operator=(_dataThread&& other) noexcept
    {
        mLogger = std::move(other.mLogger);
        
        mLogger->printThreaded(QString("_dataThread& operator=(_dataThread&& other) noexcept"), logger::LogLevel::MEDIUM, __FUNCTION__);
        return *this;
    }
    cv::Mat processing;
    QString testStr;
    std::unique_ptr<logger::LoggerThread> mLogger;
};

class Filter;
class Subtractor;
class Adder;

class ProcessingThreads : public QObject
{
    Q_OBJECT

public:
    
    explicit ProcessingThreads(std::shared_ptr<std::vector<std::mutex>> mutexes, std::shared_ptr<std::vector<std::vector<_data>>> data, int tid);
    ~ProcessingThreads();
    ProcessingThreads(const ProcessingThreads& other); // II. copy constructor
    ProcessingThreads& operator=(const ProcessingThreads& other); // III. copy assignment
    ProcessingThreads(ProcessingThreads&& other) noexcept; // IV. move constructor
    ProcessingThreads& operator=(ProcessingThreads&& other) noexcept; // V. move assignment

    //static ProcessingThreads* make(QString model);

    void configure(const std::vector<QJsonObject>& config);
    void process(std::vector<_data>& _data);
    double getElapsedTime();
    int getProcessingSize()
    {
        return mBlockProcessing.size();
    }
signals:
    void returnData();
public slots:
    void processThread();

private:
    cv::TickMeter m_timer;
    std::unique_ptr<logger::LoggerThread> mLogger;
    std::vector<Processing*> mBlockProcessing;
    std::shared_ptr<std::vector<std::mutex>> mMutexes;
    std::shared_ptr<std::vector<std::vector<_data>>> mData;
    int mTid{0};
};
