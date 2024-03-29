#ifndef PATHBUILDER_PARSER_IMPL1TEST_H
#define PATHBUILDER_PARSER_IMPL1TEST_H

#include <pbuilder/Parser.h>
#include "gtest/gtest.h"

#include "pbuilder/Parser.h"

using namespace pbuilder;

namespace {

    TEST(Parser, GeneralTest1) {
        std::string input = R"({
            "mode": "full",
            "starting_pos": {
                "lat": 111,
                "long": 222
            },
            "day_start": "9:00",
            "first_day_start": "10:00",
            "day_end": "23:00",
            "day_of_week": "monday",
            "places": [{
                "coords": {
                    "lat": 111,
                    "long": 222
                },
                "timetable": {
                    "monday": [
                      {
                        "type": "interval",
                        "starts": "9:00",
                        "ends": "18:00",
                        "lasts": "1:00",
                        "price": 150
                      },
                      {
                        "type": "time_point",
                        "starts": "9:00",
                        "lasts": "1:00",
                        "price": 150
                      }
                    ]
                  },
                "time_to_get": "1:00"
            }],
            "matrices": [
                [0],
                [0],
                [0],
                [0]
            ]
        })";

        auto parser = Parser::create();

        auto result = parser->parse(input);
        EXPECT_EQ(result.mode, Parser::Result::Mode::FULL);

        ASSERT_EQ(result.matrices.size(), 4);
        ASSERT_EQ(result.matrices[0]->rows(), 1);
        ASSERT_EQ(result.matrices[0]->cols(), 1);
        ASSERT_EQ(result.matrices[1]->rows(), 1);
        ASSERT_EQ(result.matrices[1]->cols(), 1);
        ASSERT_EQ(result.matrices[2]->rows(), 1);
        ASSERT_EQ(result.matrices[2]->cols(), 1);
        ASSERT_EQ(result.matrices[3]->rows(), 1);
        ASSERT_EQ(result.matrices[3]->cols(), 1);

        ASSERT_EQ(result.dayStart.getTimePoint(), TimePoint(9 * 60).getTimePoint());
        ASSERT_EQ(result.firstDayStart.getTimePoint(), TimePoint(10 * 60).getTimePoint());
        ASSERT_EQ(result.dayEnd.getTimePoint(), TimePoint(23 * 60).getTimePoint());

        ASSERT_EQ(result.places.size(), 1);
        ASSERT_EQ(result.dayOfWeek, 0);
    }

}
#endif //PATHBUILDER_PARSER_IMPL1TEST_H
