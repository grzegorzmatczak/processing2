#include "gtest_graph.hpp"

#include "graph.hpp"
#include "processing.h"

#include <QJsonObject>
#include <QDir>
#include <QTimer>

#include <opencv2/opencv.hpp>

#include <typeinfo>

#include "configreader.hpp"

using ::testing::AtLeast;

namespace gtest_graph {

	constexpr auto TEST_GRAPH_CONFIG{"/test_graph_config.json"};
	constexpr auto TEST_GRAPH{"/test_graph.json"};
	constexpr auto TEST_GRAPH2{"/test_graph2.json"};
	constexpr auto TEST_GRAPH_NAME{"testGraph"};

    TEST_F(GTest_graph, test_testGraph_1)
    {
		Graph<Processing, _data> graphProcessing;
		std::vector<Processing*> processingBlock;
		std::vector<std::vector<_data>> processingData;
		std::vector<cv::Mat> outputData;
		ConfigReader cf;
		QString path = QDir::currentPath();

		QJsonObject _graphConfig;
		QJsonObject _blockConfiguration;
		bool isConfigReadingSuccess{false};
		isConfigReadingSuccess = cf.readConfig(path+TEST_GRAPH, _graphConfig);
		if(!isConfigReadingSuccess)
		{
			EXPECT_EQ(isConfigReadingSuccess, true);
			qDebug()<<"Config Reading fail:"<<path+TEST_GRAPH;
			return;
		}
		
		isConfigReadingSuccess = cf.readConfig(path+TEST_GRAPH_CONFIG, _blockConfiguration);
		if(!isConfigReadingSuccess)
		{
			EXPECT_EQ(isConfigReadingSuccess, true);
			qDebug()<<"Config Reading fail:"<<path+TEST_GRAPH_CONFIG;
			return;
		}
		QJsonArray graphConfig = _graphConfig[TEST_GRAPH_NAME].toArray();
		QJsonArray blockConfiguration = _blockConfiguration[TEST_GRAPH_NAME].toArray();

		cv::Mat frame = cv::Mat(100, 100, CV_8UC1, cv::Scalar(1));
		graphProcessing.loadGraph(graphConfig, blockConfiguration, processingBlock);
		std::vector<cv::Mat> inputs{frame.clone(), frame.clone(), frame.clone()};

		for(int i = 0; i<graphConfig.size(); i++)
		{
			std::vector<_data> dataVec;
			const QJsonObject _obj = graphConfig[i].toObject();
			const QJsonArray _prevId = _obj[PREV].toArray();
			const QJsonArray _nextId = _obj[NEXT].toArray();
			if(graphProcessing.checkIfLoadInputs(_prevId, dataVec, inputs, i))
			{
				graphProcessing.loadInputs(_prevId, graphConfig, dataVec, processingData);
			}
			EXPECT_EQ(dataVec.size(), 1);
			try
			{
				processingBlock[i]->process(dataVec);
			}
			catch(cv::Exception& e)
			{
				const char* err_msg = e.what();
				qDebug()<<"exception caught in ProcessingModules: "<<err_msg;
			}
			processingData.push_back(std::move(dataVec));
			dataVec.clear();
			EXPECT_EQ(dataVec.size(), 0);
			graphProcessing.checkAndReturnData(_nextId, i, outputData, processingData);
		}
		EXPECT_EQ(processingData.size(), 3);
		EXPECT_EQ(processingData[0][0].processing.empty(), true);
		EXPECT_EQ(processingData[1][0].processing.empty(), true);
		EXPECT_EQ(processingData[2][0].processing.empty(), false);
		EXPECT_EQ(outputData.size(), 1);
		EXPECT_EQ(outputData.empty(), false);

    }

    TEST_F(GTest_graph, test_testGraph_2)
	{
		const QDateTime dateTime1 = QDateTime::currentDateTime();
		cv::TickMeter timer;
		timer.start();
		double timeProcessing{0.0};
		double timeTest{0.0};

		Graph<Processing, _data> graphProcessing;
		std::vector<Processing*> processingBlock;
		std::vector<std::vector<_data>> processingData;
		std::vector<cv::Mat> outputData;
		ConfigReader cf;
		const QString path = QDir::currentPath();
		const QJsonObject graph_config = cf.readObjectConfig(path+TEST_GRAPH2);
		const QJsonArray graphConfig = graph_config[TEST_GRAPH_NAME].toArray();
		cv::Mat frame = cv::Mat(3000, 3000, CV_8UC1, cv::Scalar(1));
		graphProcessing.loadGraph(graphConfig, processingBlock);

		std::vector<cv::Mat> inputs{frame.clone(), frame.clone(), frame.clone()};

		//for(const auto& process :processingBlock)
		for(int i = 0; i < graphConfig.size(); i++)
		{
			std::vector<_data> dataVec;
			const QJsonObject _obj = graphConfig[i].toObject();
			const QJsonArray _prevId = _obj[PREV].toArray();
			const QJsonArray _nextId = _obj[NEXT].toArray();
			if(graphProcessing.checkIfLoadInputs (_prevId, dataVec, inputs, i))
			{
				graphProcessing.loadInputs(_prevId, graphConfig, dataVec, processingData);
			}
			try
			{
				processingBlock[i]->process(dataVec);
			}
			catch(cv::Exception& e)
			{
				const char* err_msg = e.what();
				qDebug()<<"exception caught in ProcessingModules: "<<err_msg;
			}
			processingData.push_back(std::move(dataVec));
			graphProcessing.checkAndReturnData(_nextId, i, outputData, processingData);

			timeProcessing += processingBlock[i]->getElapsedTime();
			dataVec.clear();
		}

		timer.stop();
		timeTest = timer.getTimeMilli();
		
		const QDateTime dateTime2 = QDateTime::currentDateTime();

		const qint64 millisecondsDiff = dateTime1.msecsTo(dateTime2);
		qDebug()<<"timeProcessing by process: "<< timeProcessing;
		qDebug()<<"timeProcessing: "<< timeTest;
		qDebug()<<"timeProcessing: "<< millisecondsDiff;
	}
}  // namespace gtest_graph
