#ifndef PATHBUILDER_PATHCHECKER_IMPL1TEST_H
#define PATHBUILDER_PATHCHECKER_IMPL1TEST_H

#include "gtest/gtest.h"

#include "pbuilder/PathChecker.h"

using namespace pbuilder;

namespace {

    TEST(PathCheckerImpl1, GeneralTest) {
        auto _pathChecker = PathChecker::create();

        Interval i1{TimePoint(5),TimePoint(1), 100};
        Interval i2{TimePoint(10),TimePoint(1), 150};
        Interval i3{TimePoint(15),TimePoint(1), 200};
        Interval i4{TimePoint(20),TimePoint(1), 250};

        PlaceWithTimetable place1(Coordinates{10, 0},
                                  std::vector<Interval>({i1}),
                                  TimePoint(0), 0);
        PlaceWithTimetable place2(Coordinates{20, 0},
                                  std::vector<Interval>({i2}),
                                  TimePoint(0),3);
        PlaceWithTimetable place3(Coordinates{30, 0},
                                  std::vector<Interval>({i3}),
                                  TimePoint(0), 2);
        PlaceWithTimetable place4(Coordinates{40, 0},
                                  std::vector<Interval>({i4}),
                                  TimePoint(0), 1);

        MatInt mat1 = MatInt::createFromVector(4, 4,
                                               {0, 1, INF, 1,
                                                1, 0, 1, 1,
                                                INF, 1, 0, 1,
                                                1, 1, 1, 0}
        );
        MatInt mat2 (4, 4, 0);
        MatInt mat3 (4, 4, 0);
        MatInt mat4 (4, 4, 0);
        MatInt mat5 (4, 4, 0);

        _pathChecker->setDayStart(TimePoint(0));
        _pathChecker->setDayEnd(TimePoint(INF));

        _pathChecker->setPlaces({
            std::make_shared<PlaceWithTimetable>(place1),
            std::make_shared<PlaceWithTimetable>(place2),
            std::make_shared<PlaceWithTimetable>(place3),
            std::make_shared<PlaceWithTimetable>(place4)
        });
        _pathChecker->setMatrices({
            std::make_shared<MatInt>(mat1),
            std::make_shared<MatInt>(mat2),
            std::make_shared<MatInt>(mat3),
            std::make_shared<MatInt>(mat4),
            std::make_shared<MatInt>(mat5)
        });

        _pathChecker->setStartingPos(Coordinates{0,0});

        auto result = _pathChecker->check();

        ASSERT_EQ(result.block->order.size(), 4);
        ASSERT_TRUE(result.possible);

        ASSERT_EQ(result.block->order[0]->id, 0);
        ASSERT_EQ(result.block->order[1]->id, 3);
        ASSERT_EQ(result.block->order[2]->id, 2);
        ASSERT_EQ(result.block->order[3]->id, 1);

        ASSERT_EQ(result.block->order[0]->interval, i1);
        ASSERT_EQ(result.block->order[1]->interval, i2);
        ASSERT_EQ(result.block->order[2]->interval, i3);
        ASSERT_EQ(result.block->order[3]->interval, i4);
    }

}
#endif //PATHBUILDER_PATHCHECKER_IMPL1TEST_H
