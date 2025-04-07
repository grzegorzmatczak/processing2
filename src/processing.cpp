#include "processing.h"

#include "Adder/adder.h"
#include "Filter/filter.h"
#include "Subtractor/subtractor.h"

#include "logger.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QMetaEnum>
#include <QtCore>
#include <stdio.h>

Processing::Processing(QObject *parent)
	: QObject(parent)
{
  mLogger =
      std::make_unique<logger::Logger>(logger::LogType::PROCESSING, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
  mLogger->showThreadId();
}

Processing::~Processing()
{
	mLogger->printStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
}

Processing::Processing(const Processing& other)
{
	mLogger->printStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
}

Processing& Processing::operator=(const Processing& other)
{
	mLogger->printStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
	if(this==&other)
		return *this;
	return *this;
}
Processing::Processing(Processing&& other) noexcept
{
	mLogger->printStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
}
Processing& Processing::operator=(Processing&& other) noexcept
{
	mLogger->printStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
	return *this;
}

Processing* Processing::make(QString model)
{
	if (model == "Filter")
	{
		return new Filter(nullptr);
	}
	else if (model == "Estimator")
	{
		return new Subtractor(nullptr);
	}
	else if (model == "Adder")
	{
		return new Adder(nullptr);
	}
	return new Filter(nullptr);
}
