#include "graph.hpp"

#include <QJsonArray>
#include <QJsonObject>

#include "logger.hpp"

template<typename T, typename T_data>
Graph<T, T_data>::Graph()
{
	mLogger =
		std::make_unique<logger::Logger>(logger::LogType::CONFIG, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
	mLogger->printStartFunction(logger::LogLevel::MEDIUM, __FUNCTION__);
}
template<typename T, typename T_data>
Graph<T, T_data>::~Graph()
{}

template<typename T, typename T_data>
void Graph<T, T_data>::loadGraph(const QJsonArray& graph, const QJsonArray& config, std::vector<T*>& block)
{
	block.clear();
	for (int i = 0; i < graph.size(); i++)
	{
		QJsonObject _obj = graph[i].toObject();
		QString _type = _obj[TYPE].toString();
		QJsonArray _prevActive = _obj[PREV].toArray();
		T* _block = T::make(_type);
		_block->configure(config[i].toObject());
		block.push_back(_block);
	}
}

template<typename T, typename T_data>
void Graph<T, T_data>::loadGraph(const QJsonArray& graph, std::vector<T*>& block)
{
	block.clear();
	for(int i = 0; i<graph.size(); i++)
	{
		QJsonObject _obj = graph[i].toObject();
		QString _type = _obj[TYPE].toString();
		QJsonArray _prevActive = _obj[PREV].toArray();
		T* _block = T::make(_type);
		_block->configure(_obj[CONFIG].toObject());
		block.push_back(_block);
	}
}

template<typename T, typename T_data>
bool Graph<T, T_data>::checkIfLoadInputs(const QJsonArray& prevActive, std::vector<T_data>& dataVec, cv::Mat& input)
{
	bool _flagNotStart{true};
	for(int j = 0; j<prevActive.size(); j++)
	{
		if(prevActive[j].toObject()[ID].toInt()==-1)
		{
			_flagNotStart = false;
			//T_data data{input.clone(), "temp1"};
			dataVec.push_back({input.clone(), "temp1"});
			qDebug()<<"dataVec.push_back(data); 2";
		}
	}
	return _flagNotStart;
}

template<typename T, typename T_data>
bool Graph<T, T_data>::checkIfLoadInputs(const QJsonArray& prevActive, std::vector<T_data>& dataVec, std::vector<cv::Mat>& input, int i)
{
	bool _flagNotStart{true};
	for(int j = 0; j<prevActive.size(); j++)
	{
		if(prevActive[j].toObject()[ID].toInt() == -1)
		{
			if(i<input.size())
			{
				_flagNotStart = false;
				//T_data data;
				dataVec.push_back({input[i].clone(), "test1"});
			}
			else
			{
			}
		}
	}
	return _flagNotStart;
}


template<typename T, typename T_data>
void Graph<T, T_data>::loadInputs(const QJsonArray& prevActive, const QJsonArray& a_graph, std::vector<T_data>& dataVec,
	std::vector<std::vector<T_data>>& a_data)
{
	qDebug()<<"loadInputs:";
	for(int prevIter = 0; prevIter<prevActive.size(); prevIter++)
	{
		int _prevIterator = prevActive[prevIter].toObject()[ID].toInt();
		int signalToCopy = prevActive[prevIter].toObject()[COPY_SIGNAL].toInt();

		qDebug()<<"_prevIterator:" <<_prevIterator;
		qDebug()<<"signalToCopy:"<<signalToCopy;
		if(_prevIterator<0||signalToCopy<0)
		{
			qDebug()<<"prevActive:"<<prevActive;
			qDebug()<<"a_graph:"<<a_graph;
		}
		const QJsonArray _nextActivePrevArray = a_graph[_prevIterator].toObject()[NEXT].toArray();
		qDebug()<<"_nextActivePrevArray:"<<_nextActivePrevArray;
		if(_nextActivePrevArray.size()>1)
		{
			qDebug()<<"T_data data; copy 1";
			T_data data;
			qDebug()<<"data = a_data";
			data = a_data[_prevIterator][signalToCopy];

			data.processing = a_data[_prevIterator][signalToCopy].processing.clone();

			qDebug()<<"push_back:";
			dataVec.push_back(data);
			qDebug()<<"T_data data; copy 2";
			
		}
		else
		{
			qDebug()<<"std::move: 1";
			dataVec.push_back(std::move(a_data[_prevIterator][signalToCopy]));
			qDebug()<<"std::move: 2";
		}
	}
}

template<typename T, typename T_data>
bool Graph<T, T_data>::checkIfReturnData(const QJsonArray& nextActive)
{
	bool _flagReturnData{false};
	for(int j = 0; j<nextActive.size(); j++)
	{
		if(nextActive[j].toObject()[ID].toInt() == -1)
		{
			_flagReturnData = true;
		}
	}
	return _flagReturnData;
}

template<typename T, typename T_data>
void Graph<T, T_data>::returnData(int i, std::vector<cv::Mat>& outputData, std::vector<std::vector<T_data>>& outputDataVector,
	std::vector<std::vector<T_data>>& data)
{
	for(int ii = 0; ii<data[i].size(); ii++)
	{
		outputData.push_back(data[i][ii].processing.clone());
	}
	outputDataVector.push_back(data[i]);
}

template<typename T, typename T_data>
void Graph<T, T_data>::returnData(int i, std::vector<cv::Mat>& outputData, std::vector<std::vector<T_data>>& data)
{
	for(int ii = 0; ii<data[i].size(); ii++)
	{
		outputData.push_back(data[i][ii].processing.clone());
	}
}

template<typename T, typename T_data>
void Graph<T, T_data>::checkAndReturnData(const QJsonArray& nextActive, int i, std::vector<cv::Mat>& outputData,
	std::vector<std::vector<T_data>>& data)
{
	for(int j = 0; j<nextActive.size(); j++)
	{
		if(nextActive[j].toObject()[ID].toInt()==-1)
		{
			outputData.push_back(data[i][j].processing.clone());
		}
	}
}

template class Graph<Processing, _data>;