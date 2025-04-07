#include "gtest_graph_threads.hpp"

#include "processing_threads.h"
#include "graph_threads.hpp"
#include "processing.h"

#include <QJsonObject>
#include <QDir>
#include <QTimer>
#include <QThread>

#include <opencv2/opencv.hpp>

#include <typeinfo>

#include "configreader.hpp"

using ::testing::AtLeast;

namespace gtest_graph_threads
{

	constexpr auto TEST_GRAPH_CONFIG{"/test_graph_config.json"};
	constexpr auto TEST_GRAPH{"/test_graph.json"};
	constexpr auto TEST_GRAPH2{"/test_graph2.json"};
	constexpr auto TEST_GRAPH3{"/test_graph3.json"};
	constexpr auto TEST_GRAPH_NAME{"testGraph"};

    TEST_F(GTest_graph_threads, test_testGraph_threads_1)
    {}

    TEST_F(GTest_graph_threads, test_testGraph_2_threads)
	{
		const QDateTime dateTime1 = QDateTime::currentDateTime();
		cv::TickMeter timer;
		timer.start();
		double timeProcessing{0.0};
		double timeTest{0.0};
		qRegisterMetaType<std::vector<_data>>("std::vector<_data>");
		_data dt;
		//cv::Mat frame = cv::Mat(3000, 3000, CV_8UC1, cv::Scalar(1));
		dt.processing = cv::Mat(3000, 3000, CV_8UC1, cv::Scalar(1));
		dt.testStr = "test1";
		std::vector<_data> _ProcessingData;
		_ProcessingData.push_back(dt);
		GraphThreads graphProcessing;
		std::vector<cv::Mat> outputData;
		ConfigReader cf;
		const QString path = QDir::currentPath();
		const QJsonObject graph_config = cf.readObjectConfig(path+TEST_GRAPH3);
		const QJsonArray graphConfig = graph_config[TEST_GRAPH_NAME].toArray();
		
		std::vector<QThread*> mGraphTreads;
		graphProcessing.loadGraph(graphConfig);
		graphProcessing.runGraph(_ProcessingData);
		timer.stop();
		timeTest = timer.getTimeMilli();
		const QDateTime dateTime2 = QDateTime::currentDateTime();
		const qint64 millisecondsDiff = dateTime1.msecsTo(dateTime2);
		qDebug()<<"timeProcessing by process: "<< timeProcessing;
		qDebug()<<"timeProcessing: "<< timeTest;
		qDebug()<<"timeProcessing: "<< millisecondsDiff;

	}
}  // namespace gtest_graph_threads
