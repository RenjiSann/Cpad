#include <gtest/gtest.h>

#include "error_tests.hh"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}