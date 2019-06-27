#ifndef PATHBUILDER_TIMEPOINTTEST_H
#define PATHBUILDER_TIMEPOINTTEST_H

#include "gtest/gtest.h"

#include "pbuilder/Data/TimePoint.h"

using namespace pbuilder;

namespace {

    TEST(TimePointTest, CorrectCreation) {
        TimePoint point(27, 1, 9);
        EXPECT_EQ(point.getStartingTime(), 1);
        EXPECT_EQ(point.getInterval(), 9);

    }

    TEST(TimePointTest, CorrectCalculation) {
        TimePoint point(27, 1, 9);
        EXPECT_EQ(point.getTimePoint(), 3);
        EXPECT_EQ(point.getTime(), 28);
    }

    TEST(TimePointTest, ComparingTest) {
        TimePoint p1(27), p2(27), p3(23), p4(30);
        EXPECT_LT(p3, p4);
        EXPECT_LE(p1, p2);
        EXPECT_GT(p4, p1);
        EXPECT_GE(p1, p3);
        EXPECT_EQ(p1, p2);
        EXPECT_NE(p1, p3);
    }

    TEST(IntervalTest, IntervalComparsion) {
        Interval i1, i2;
        i1.starts = TimePoint(15);
        i2.starts = TimePoint(15);
        i1.lasts = TimePoint(10);
        i2.lasts = TimePoint(10);
        i1.price = 100;
        i2.price = 120;

        EXPECT_LT(i1, i2);

    }

    TEST(TimePointTest, ParsingFromString) {
        auto tp1 = TimePoint::fromString("10:05", "9:00", 1);
        EXPECT_EQ(tp1.getTimePoint(), 65);
    }

    TEST(TimePointTest, IncorrectParsingFromString) {
        EXPECT_THROW(TimePoint::fromString("24:05", "9:67", 1), std::runtime_error);
    }

    TEST(TimePointTest, ToString) {
        auto tp1 = TimePoint::fromString("10:5", "9:00", 1);
        EXPECT_EQ(tp1.toString(), "10:05");
    }

}  // namespace

#endif //PATHBUILDER_TIMEPOINTTEST_H
