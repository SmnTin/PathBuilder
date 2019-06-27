#ifndef PATHBUILDER_PLACETEST_H
#define PATHBUILDER_PLACETEST_H

#include "gtest/gtest.h"

#include "pbuilder/Data/Place.h"

using namespace pbuilder;

namespace {

    TEST(PlaceTest, NearestInterval) {
        Interval i1{TimePoint(10),TimePoint(5), 100};
        Interval i2{TimePoint(5),TimePoint(7), 150};
        Interval i3{TimePoint(12),TimePoint(4), 200};

        Place place1(Coordinates{10, 20},
                    std::vector<Interval>({i1, i2, i3}),
                    TimePoint(5));

        TimePoint tp1(7), tp2(5), tp3(14);

        EXPECT_EQ(place1.nearestTime(tp1).starts, i1.starts);
        EXPECT_EQ(place1.nearestTime(tp2).starts, i2.starts);
        EXPECT_EQ(place1.nearestTime(tp3).starts.getTime(), INF);

        Place place2;
        EXPECT_EQ(place2.nearestTime(tp1).starts, tp1);

    }

}  // namespace

#endif //PATHBUILDER_PLACETEST_H
