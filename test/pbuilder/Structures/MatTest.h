#ifndef PATHBUILDER_MATTEST_H
#define PATHBUILDER_MATTEST_H

#include "gtest/gtest.h"

#include "pbuilder/Structures/Mat.h"

using namespace pbuilder;

namespace {

    //mat initialized with empty constructor must have zeroed size
    TEST(MatTest, EmptyMatHasCorrectSize) {
        MatInt mat;
        EXPECT_EQ(mat.rows(), 0);
        EXPECT_EQ(mat.cols(), 0);
    }

    TEST(MatTest, MatHasCorrectSize) {
        MatInt mat(5, 4);
        EXPECT_EQ(mat.rows(), 5);
        EXPECT_EQ(mat.cols(), 4);
    }

    TEST(MatTest, MatHasCorrectSizeAfterAssign) {
        MatInt mat1, mat2(5, 4), mat3, mat4;
        mat3 = mat1;
        mat4 = mat2;
        EXPECT_EQ(mat3.rows(), 0);
        EXPECT_EQ(mat3.cols(), 0);
        EXPECT_EQ(mat4.rows(), 5);
        EXPECT_EQ(mat4.cols(), 4);
    }

    //trying to access mat out of its bounds and retrieve corresponding exception
    TEST(MatTest, AccessingMatOutOfBounds) {
        MatInt mat(5, 4);
        EXPECT_THROW(mat.at(5, 1), MatOutOfBoundsException);
    }

    TEST(MatTest, SavingAngRestoringValue) {
        MatInt mat(5, 4);
        int a = 100;
        mat.at(1,1) = a;
        EXPECT_EQ(mat.at(1,1), a);
    }

    TEST(MatTest, FillingWithDefaultValue) {
        int a = 5;
        MatInt mat(5, 4, a);
        for(int i = 0; i < 5; ++i)
            for(int j = 0; j < 4; ++j)
                EXPECT_EQ(mat.at(i, j), a);
    }

    TEST(MatTest, FillingWithValue) {
        int a = 5;
        MatInt mat(5, 4);
        mat.fill(a);

        for(int i = 0; i < 5; ++i)
            for(int j = 0; j < 4; ++j)
                EXPECT_EQ(mat.at(i, j), a);
    }


}  // namespace

#endif //PATHBUILDER_MATTEST_H
