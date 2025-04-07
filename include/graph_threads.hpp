#pragma once

#include <mutex>

#include "processing.h"
#include "processing_global.h"
#include "graph_global.h"

#include <QJsonArray>

class QThread;
class ProcessingThreads;

class GraphThreads
{
public:
	GraphThreads();
	~GraphThreads();
	void loadGraph(const QJsonArray& graph);
	void runGraph(std::vector<_data>& _data);

private:
	std::unique_ptr<logger::LoggerThread> mLogger;
	std::vector<std::vector<QJsonObject>> mThreadedGraphId;
	std::vector<QThread*> mGraphTreads;
	std::shared_ptr<std::vector<std::mutex>> mGraphMutexes;
	std::vector<ProcessingThreads*> mProcessingBlock;
	std::shared_ptr<std::vector<std::vector<_data>>> mData;
};
