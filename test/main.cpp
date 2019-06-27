#include "gtest/gtest.h"
#include "pbuilder/Structures/MatTest.h"
#include "pbuilder/Data/PlaceTest.h"
#include "pbuilder/Data/TimePointTest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}