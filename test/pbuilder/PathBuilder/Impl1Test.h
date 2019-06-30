#ifndef PATHBUILDER_PATHBUILDER_IMPL1TEST_H
#define PATHBUILDER_PATHBUILDER_IMPL1TEST_H

#include "gtest/gtest.h"

#include "pbuilder/PathBuilder.h"

using namespace pbuilder;

namespace {

    TEST(PathBuilderImpl1, TooLongToReachEachOtherTest) {
        auto _pathBuilder = PathBuilder::createImpl1();

        Interval i1{TimePoint(5),TimePoint(1), 100};
        Interval i2{TimePoint(10),TimePoint(1), 150};
        Interval i3{TimePoint(15),TimePoint(1), 200};
        Interval i4{TimePoint(20),TimePoint(1), 250};

        PlaceWithTimetable place1(Coordinates{10, 0},
                                  std::vector<Interval>({i1}),
                                  TimePoint(0),
                                  0);
        PlaceWithTimetable place2(Coordinates{20, 0},
                                  std::vector<Interval>({i2}),
                                  TimePoint(0),
                                  1);
        PlaceWithTimetable place3(Coordinates{30, 0},
                                  std::vector<Interval>({i3}),
                                  TimePoint(0),
                                  2);
        PlaceWithTimetable place4(Coordinates{40, 0},
                                  std::vector<Interval>({i4}),
                                  TimePoint(0),
                                  3);

        MatInt mat1 = MatInt::createFromVector(4, 4,
                {0, INF, 1, 1,
                 INF, 0, 1, 1,
                 1, 1, 0, 1,
                 1, 1, 1, 0}
        );
        MatInt mat2 (4, 4, 0);
        MatInt mat3 (4, 4, 0);
        MatInt mat4 (4, 4, 0);
        MatInt mat5 (4, 4, 0);

        _pathBuilder->setMaxBlockSize(2);
        _pathBuilder->setDayStart(TimePoint(0));
        _pathBuilder->setDayEnd(TimePoint(INF));

        _pathBuilder->setPlaces({
            std::make_shared<PlaceWithTimetable>(place1),
            std::make_shared<PlaceWithTimetable>(place2),
            std::make_shared<PlaceWithTimetable>(place3),
            std::make_shared<PlaceWithTimetable>(place4)
        });
        _pathBuilder->setMatrices({
            std::make_shared<MatInt>(mat1),
            std::make_shared<MatInt>(mat2),
            std::make_shared<MatInt>(mat3),
            std::make_shared<MatInt>(mat4),
            std::make_shared<MatInt>(mat5)
        });

        _pathBuilder->setStartingPos(Coordinates{0,0});

        auto result = _pathBuilder->build();
        ASSERT_EQ(result.blocks.size(), 2);
        EXPECT_EQ(result.blocks[0]->order.size(), 2);
        EXPECT_EQ(result.blocks[1]->order.size(), 2);
    }

    TEST(PathBuilderImpl1, AddingMoreWeightsRestrictions) {
        auto _pathBuilder = PathBuilder::createImpl1();

        Interval i1{TimePoint(5),TimePoint(1), 100};
        Interval i2{TimePoint(10),TimePoint(1), 150};
        Interval i3{TimePoint(15),TimePoint(1), 200};
        Interval i4{TimePoint(20),TimePoint(1), 250};

        PlaceWithTimetable place1(Coordinates{10, 0},
                                  std::vector<Interval>({i1}),
                                  TimePoint(0),
                                  0);
        PlaceWithTimetable place2(Coordinates{20, 0},
                                  std::vector<Interval>({i2}),
                                  TimePoint(0),
                                  1);
        PlaceWithTimetable place3(Coordinates{30, 0},
                                  std::vector<Interval>({i3}),
                                  TimePoint(0),
                                  2);
        PlaceWithTimetable place4(Coordinates{40, 0},
                                  std::vector<Interval>({i4}),
                                  TimePoint(0),
                                  3);

        MatInt mat1 = MatInt::createFromVector(4, 4,
                                               {0, INF, 1, 1,
                                                INF, 0, 1, INF,
                                                1, 1, 0, 1,
                                                1, INF, 1, 0}
        );
        MatInt mat2 (4, 4, 0);
        MatInt mat3 (4, 4, 0);
        MatInt mat4 (4, 4, 0);
        MatInt mat5 (4, 4, 0);

        _pathBuilder->setMaxBlockSize(2);
        _pathBuilder->setDayStart(TimePoint(0));
        _pathBuilder->setDayEnd(TimePoint(INF));

        _pathBuilder->setPlaces({
            std::make_shared<PlaceWithTimetable>(place1),
            std::make_shared<PlaceWithTimetable>(place2),
            std::make_shared<PlaceWithTimetable>(place3),
            std::make_shared<PlaceWithTimetable>(place4)
        });
        _pathBuilder->setMatrices({
            std::make_shared<MatInt>(mat1),
            std::make_shared<MatInt>(mat2),
            std::make_shared<MatInt>(mat3),
            std::make_shared<MatInt>(mat4),
            std::make_shared<MatInt>(mat5)
        });

        _pathBuilder->setStartingPos(Coordinates{0,0});

        auto result = _pathBuilder->build();
        ASSERT_EQ(result.blocks.size(), 3);
        EXPECT_EQ(result.blocks[0]->order.size(), 2);
        EXPECT_EQ(result.blocks[1]->order.size(), 1);
        EXPECT_EQ(result.blocks[2]->order.size(), 1);
    }

    TEST(PathBuilderImpl1, CheckingBlocksDividingAlgorithm) {
        auto _pathBuilder = PathBuilder::createImpl1();

        Interval i1{TimePoint(5),TimePoint(1), 100};
        Interval i2{TimePoint(10),TimePoint(1), 150};
        Interval i3{TimePoint(15),TimePoint(1), 200};

        PlaceWithTimetable place1(Coordinates{10, 0},
                                  std::vector<Interval>({i1}),
                                  TimePoint(0),
                                  0);
        PlaceWithTimetable place2(Coordinates{20, 0},
                                  std::vector<Interval>({i2}),
                                  TimePoint(0),
                                  1);
        PlaceWithTimetable place3(Coordinates{30, 0},
                                  std::vector<Interval>({i3}),
                                  TimePoint(0),
                                  2);

        MatInt mat1 = MatInt::createFromVector(3, 3,
                                               {0, INF, 1,
                                                INF, 0, 1,
                                                1, 1, 0}
        );
        MatInt mat2 (3, 3, 0);
        MatInt mat3 (3, 3, 0);
        MatInt mat4 (3, 3, 0);
        MatInt mat5 (3, 3, 0);

        _pathBuilder->setMaxBlockSize(2);
        _pathBuilder->setDayStart(TimePoint(0));
        _pathBuilder->setDayEnd(TimePoint(INF));

        _pathBuilder->setPlaces({
            std::make_shared<PlaceWithTimetable>(place1),
            std::make_shared<PlaceWithTimetable>(place2),
            std::make_shared<PlaceWithTimetable>(place3)
        });
        _pathBuilder->setMatrices({
            std::make_shared<MatInt>(mat1),
            std::make_shared<MatInt>(mat2),
            std::make_shared<MatInt>(mat3),
            std::make_shared<MatInt>(mat4),
            std::make_shared<MatInt>(mat5)
        });

        _pathBuilder->setStartingPos(Coordinates{0,0});

        auto result = _pathBuilder->build();
        ASSERT_EQ(result.blocks.size(), 2);
        ASSERT_EQ(result.blocks[0]->order.size(), 2);
        ASSERT_EQ(result.blocks[1]->order.size(), 1);
    }

    TEST(PathBuilderImpl1, IntervalsOverlappingTest) {
        auto _pathBuilder = PathBuilder::createImpl1();

        Interval i1{TimePoint(5),TimePoint(5), 100};
        Interval i2{TimePoint(10),TimePoint(1), 150};

        PlaceWithTimetable place1(Coordinates{10, 0},
                                  std::vector<Interval>({i1}),
                                  TimePoint(0),
                                  0);
        PlaceWithTimetable place2(Coordinates{20, 0},
                                  std::vector<Interval>({i2}),
                                  TimePoint(0),
                                  1);

        MatInt mat1 = MatInt::createFromVector(3, 3,
                                               {0, 1,
                                                1, 0}
        );
        MatInt mat2 (2, 2, 0);
        MatInt mat3 (2, 2, 0);
        MatInt mat4 (2, 2, 0);
        MatInt mat5 (2, 2, 0);

        _pathBuilder->setMaxBlockSize(2);
        _pathBuilder->setDayStart(TimePoint(0));
        _pathBuilder->setDayEnd(TimePoint(INF));

        _pathBuilder->setPlaces({
            std::make_shared<PlaceWithTimetable>(place1),
            std::make_shared<PlaceWithTimetable>(place2)
        });
        _pathBuilder->setMatrices({
            std::make_shared<MatInt>(mat1),
            std::make_shared<MatInt>(mat2),
            std::make_shared<MatInt>(mat3),
            std::make_shared<MatInt>(mat4),
            std::make_shared<MatInt>(mat5)
        });

        _pathBuilder->setStartingPos(Coordinates{0,0});

        auto result = _pathBuilder->build();
        ASSERT_EQ(result.blocks.size(), 2);
        ASSERT_EQ(result.blocks[0]->order.size(), 1);
        ASSERT_EQ(result.blocks[1]->order.size(), 1);

        ASSERT_TRUE((result.blocks[0]->order[0]->id == 0 && result.blocks[1]->order[0]->id == 1) ||
                    (result.blocks[0]->order[0]->id == 1 && result.blocks[1]->order[0]->id == 0));

        ASSERT_TRUE((result.blocks[0]->order[0]->interval == i1 && result.blocks[1]->order[0]->interval == i2) ||
                    (result.blocks[0]->order[0]->interval == i2 && result.blocks[1]->order[0]->interval == i1));

    }


}  // namespace}

#endif //PATHBUILDER_PATHBUILDER_IMPL1TEST_H
