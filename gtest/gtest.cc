#include "gtest/gtest.h"
#include "gmock/gmock.h" 
#include <QCoreApplication>
#include <QTimer>


using ::testing::AtLeast;
/*
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
*/
int main(int argc, char* argv[])
{
    QCoreApplication app{argc, argv};

    QTimer::singleShot(0, [&]()
        {
            ::testing::InitGoogleTest(&argc, argv);
            auto testResult = RUN_ALL_TESTS();
            app.exit(testResult);
        });

    return app.exec();
}