#include "gtest/gtest.h"
#include "gmock/gmock.h" 

#include <QDebug>

using ::testing::AtLeast;

namespace gtest_graph
{
    class GTest_graph : public ::testing::Test
    {
        protected:
            GTest_graph(){}
            ~GTest_graph() override {}
            void SetUp() override{}
            void TearDown() override {}
    };
}  // namespace gtest_graph
