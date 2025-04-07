#pragma once

#include "processing.h"
#include "processing_global.h"
#include "graph_global.h"
#include <QJsonArray>

template<typename T, typename T_data>
class Graph
{
public:
	Graph();
	~Graph();
	void loadGraph(const QJsonArray& graph, const QJsonArray& config, std::vector<T*>& block);
	void loadGraph(const QJsonArray& graph, std::vector<T*>& block);
	bool checkIfLoadInputs(const QJsonArray& prevActive, std::vector<T_data>& dataVec, cv::Mat& input);
	bool checkIfLoadInputs(const QJsonArray& prevActive, std::vector<T_data>& dataVec, std::vector<cv::Mat>& input, int i);
	void loadInputs(const QJsonArray& prevActive, const QJsonArray& a_graph, std::vector<T_data>& dataVec,
		std::vector<std::vector<T_data>>& a_data);
	bool checkIfReturnData(const QJsonArray& nextActive);
	void returnData(int i, std::vector<cv::Mat>& outputData, std::vector<std::vector<T_data>>& outputDataVector,
		std::vector<std::vector<T_data>>& data);

	void returnData(int i, std::vector<cv::Mat>& outputData, std::vector<std::vector<T_data>>& data);
	void checkAndReturnData(const QJsonArray& nextActive, int i, std::vector<cv::Mat>& outputData,
		std::vector<std::vector<T_data>>& data);

private:
	std::unique_ptr<logger::Logger> mLogger;

};
