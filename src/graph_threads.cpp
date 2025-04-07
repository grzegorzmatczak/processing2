#include "graph_threads.hpp"

#include <QJsonArray>
#include <QJsonObject>
#include <QThread>

#include "logger_thread.hpp"
#include "processing_threads.h"


GraphThreads::GraphThreads()
{
	mLogger =
		std::make_unique<logger::LoggerThread>(logger::LogType::CONFIG, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
	mLogger->printThreadedStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
}
GraphThreads::~GraphThreads()
{
	mLogger->printThreadedStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
}

void GraphThreads::loadGraph(const QJsonArray& graph)
{
	mLogger->printThreadedStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
	// Check if graph ok:
	std::set<int> setTids;
	for(const auto& node:mThreadedGraphId)
	{
		int tid = node[0][TID].toInt();
		setTids.emplace(tid);
		qDebug()<<"tid:"<<tid;
	}
	if(mThreadedGraphId.size()!=setTids.size())
	{
		mLogger->printThreadedError(QString("graph have wrong tid's"), logger::LogLevel::MEDIUM, __FUNCTION__);
		return;
	}

	mProcessingBlock.clear();
	int maxThreadId{0};
	for(const auto& node:graph)
	{
		const QJsonObject _obj = node.toObject();
		int id = _obj[TID].toInt();
		if(id>maxThreadId)
			maxThreadId = id;

		int currentSizeOfGraph = mThreadedGraphId.size() - 1;
		if(id>(currentSizeOfGraph))
		{
			std::vector<QJsonObject> objs;
			objs.push_back(_obj);
			mThreadedGraphId.push_back(objs);
		}
		else
		{
			mThreadedGraphId[id].push_back(_obj);
		}
	}
	mGraphMutexes.reset(new std::vector<std::mutex>(mThreadedGraphId.size()+1));
	mData.reset(new std::vector<std::vector<_data>>(mThreadedGraphId.size()+1));
	

	for(const auto& node:mThreadedGraphId)
	{
		int tid = node[0][TID].toInt();
		qDebug()<<"tid:"<<tid;
		ProcessingThreads* _processingThreads = new ProcessingThreads(mGraphMutexes, mData, tid);
		qDebug()<<"\nnode:"<<node;
		_processingThreads->configure(node);
		mProcessingBlock.push_back(_processingThreads);
		QThread* _thread = new QThread();
		_processingThreads->moveToThread(_thread);
		_thread->start();
		mGraphTreads.push_back(_thread);

		const QJsonArray _prevTId = node[0][TPREV].toArray();
		const QJsonArray _nextTId = node[0][TNEXT].toArray();
		int id = node[0][TID].toInt();
		for(const auto& pTid:_prevTId)
		{
			int _prevIterator = pTid.toObject()[TID].toInt();
			int _signalToCopy = pTid.toObject()[COPY_SIGNAL].toInt();
			if(mProcessingBlock.size()>=_prevIterator)
				QObject::connect(mProcessingBlock[_prevIterator], &ProcessingThreads::returnData
					, mProcessingBlock[id], &ProcessingThreads::processThread, Qt::ConnectionType::QueuedConnection);
		}
	}
}

void GraphThreads::runGraph(std::vector<_data>& _data)
{
	for(const auto& vec:_data)
	{
		mLogger->printThreaded(QString("_data:%1").arg(vec.testStr), logger::LogLevel::MEDIUM, __FUNCTION__);

	}
	for(const auto& node:mThreadedGraphId)
	{
		const QJsonArray _prevTId = node[0][TPREV].toArray();
		int tid = node[0][TID].toInt();
		for(const auto& pTid:_prevTId)
		{
			int _prevIterator = pTid.toObject()[TID].toInt();
			int _signalToCopy = pTid.toObject()[COPY_SIGNAL].toInt();
			if(_prevIterator<0||_signalToCopy<0)
			{
				mLogger->printThreaded(QString("kopiowanie daty :%1 ").arg(tid), logger::LogLevel::MEDIUM, __FUNCTION__);
				mData.get()->at(tid) = _data;
				//mProcessingBlock[tid]->processThread();
			}
		}
	}
	for(const auto& node : *mData.get())
	{
		mLogger->printThreaded(QString("mData:size:%1").arg(node.size()), logger::LogLevel::MEDIUM, __FUNCTION__);
		for(const auto& vec:node)
		{
			mLogger->printThreaded(QString("vec:%1").arg(vec.testStr), logger::LogLevel::MEDIUM, __FUNCTION__);

		}
	}

	for(const auto& node:mThreadedGraphId)
	{
		const QJsonArray _prevTId = node[0][TPREV].toArray();
		int tid = node[0][TID].toInt();
		for(const auto& pTid:_prevTId)
		{
			int _prevIterator = pTid.toObject()[TID].toInt();
			int _signalToCopy = pTid.toObject()[COPY_SIGNAL].toInt();
			if(_prevIterator<0||_signalToCopy<0)
			{
				mLogger->printThreaded(QString("uruchomienie :%1 ").arg(tid), logger::LogLevel::MEDIUM, __FUNCTION__);
				//mData[tid] = _data;
				mProcessingBlock[tid]->processThread();
				mProcessingBlock[tid]->processThread();
				mProcessingBlock[tid]->processThread();
				mProcessingBlock[tid]->processThread();
				mProcessingBlock[tid]->processThread();
			}
		}
	}
} 
