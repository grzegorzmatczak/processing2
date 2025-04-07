#include "processing_threads.h"

#include "Adder/adder.h"
#include "Filter/filter.h"
#include "Subtractor/subtractor.h"

#include "logger_thread.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <QThread>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QMetaEnum>
#include <QtCore>

#include <stdio.h>

ProcessingThreads::ProcessingThreads(std::shared_ptr<std::vector<std::mutex>> mutexes, std::shared_ptr<std::vector<std::vector<_data>>> data, int tid)
	: mMutexes(mutexes)
	, mData(data)
	, mTid(tid)
{
  mLogger = std::make_unique<logger::LoggerThread>(logger::LogType::PROCESSING, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
  mLogger->showThreadIdThreaded();
}

ProcessingThreads::~ProcessingThreads()
{
	mLogger->printThreadedStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
	mBlockProcessing.clear();
}

ProcessingThreads::ProcessingThreads(const ProcessingThreads& other)
{
	mLogger->printThreadedStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
}

ProcessingThreads& ProcessingThreads::operator=(const ProcessingThreads& other)
{
	mLogger->printThreadedStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
	if(this==&other)
		return *this;
	return *this;
}
ProcessingThreads::ProcessingThreads(ProcessingThreads&& other) noexcept
{
	mLogger->printThreadedStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
}
ProcessingThreads& ProcessingThreads::operator=(ProcessingThreads&& other) noexcept
{
	mLogger->printThreadedStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
	return *this;
}

double ProcessingThreads::getElapsedTime()
{
	return 0.0;
}

void ProcessingThreads::configure(const std::vector<QJsonObject>& config)
{
	mLogger->printThreadedStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
	mBlockProcessing.clear();
	for(const auto& block:config)
	{
		QString _type = block[TYPE].toString();
		mLogger->printThreaded(QString("_type:%1").arg(_type), logger::LogLevel::MEDIUM, __FUNCTION__);
		Processing* processing = Processing::make(_type);
		const QJsonObject& configPerBlock = block[CONFIG].toObject();
		processing->configure(configPerBlock);
		mBlockProcessing.push_back(processing);
	}
}

void ProcessingThreads::processThread()
{
	mLogger->printThreaded(QString("try lock data:%1 mBlockProcessing.size:%2").arg(mTid).arg(mBlockProcessing.size()), logger::LogLevel::MEDIUM, __FUNCTION__);
	mMutexes->at(mTid).lock();
	std::vector<_data> _data = mData->at(mTid);
	mMutexes->at(mTid).unlock();
	for(auto& block:mBlockProcessing)
	{
		block->process(_data);
	}
	mLogger->printThreaded(QString("processing:%1 cols:%2").arg(mTid).arg(_data[0].processing.cols), logger::LogLevel::MEDIUM, __FUNCTION__);
	mMutexes->at(mTid+1).lock();
	mData->at(mTid+1) = _data;
	mMutexes->at(mTid+1).unlock();

	emit returnData();
}

void ProcessingThreads::process(std::vector<_data>& _data)
{
	mLogger->printThreadedStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
	m_timer.start();
	//m_adder->process(_data);
	m_timer.stop();
}
