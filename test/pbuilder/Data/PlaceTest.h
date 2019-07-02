#ifndef PATHBUILDER_PLACETEST_H
#define PATHBUILDER_PLACETEST_H

#include "gtest/gtest.h"

#include "pbuilder/Data/Place.h"

using namespace pbuilder;

namespace {

    TEST(PlaceTest, NearestInterval) {
        auto el1 = std::make_shared<FixedTimetableElement>(TimePoint(20), TimePoint(10), 100);
        auto el2 = std::make_shared<FreeTimetableElement>(TimePoint(14), TimePoint(100), TimePoint(15), 50);

        PlaceWithMixedTimetable place1(Coordinates{10, 20},
                                  TimePoint(5));
        place1.addTimetableElement(el1, 0);
        place1.addTimetableElement(el2, 1);

        TimePoint tp1(7), tp2(30), tp3(10), tp4(16), tp5(90);

        EXPECT_EQ(place1.nearestTime(tp1, 0).starts, TimePoint(20));
        EXPECT_EQ(place1.nearestTime(tp2, 0).starts, TimePoint(INF));
        EXPECT_EQ(place1.nearestTime(tp3, 1).starts, TimePoint(14));
        EXPECT_EQ(place1.nearestTime(tp4, 1).starts, TimePoint(16));
        EXPECT_EQ(place1.nearestTime(tp5, 1).starts, TimePoint(INF));

    }

}  // namespace

#endif //PATHBUILDER_PLACETEST_H
