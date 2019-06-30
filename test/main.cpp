#include "gtest/gtest.h"
#include "pbuilder/Structures/MatTest.h"
#include "pbuilder/Data/PlaceTest.h"
#include "pbuilder/Data/TimePointTest.h"
#include "pbuilder/PathBuilder/Impl1Test.h"
#include "pbuilder/PathChecker/Impl1Test.h"
#include "pbuilder/Parser/Impl1Test.h"
#include "pbuilder/Runner/Impl1Test.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}