#include "gtest/gtest.h"
#include "gmock/gmock.h" 

#include <QDebug>

using ::testing::AtLeast;

namespace gtest_graph_threads
{
    class GTest_graph_threads : public ::testing::Test
    {
        protected:
            GTest_graph_threads(){}
            ~GTest_graph_threads() override {}
            void SetUp() override{}
            void TearDown() override {}
    };
}  // namespace gtest_graph_threads
