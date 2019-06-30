#ifndef PATHBUILDER_RUNNERIMPL1_H
#define PATHBUILDER_RUNNERIMPL1_H

#include <pbuilder/Parser.h>
#include "gtest/gtest.h"

#include "pbuilder/Runner.h"

using namespace pbuilder;

namespace {

    TEST(Runner, GeneralTest1) {
        std::string input = R"({
            "mode": "full",
            "starting_pos": {
                "lat": 111,
                "long": 222
            },
            "day_start": "9:00",
            "day_end": "23:00",
            "places": [{
                "coords": {
                    "lat": 111,
                    "long": 222
                },
                "interval": {
                    "starts": "9:00",
                    "ends": "18:00",
                    "lasts": "1:00",
                    "price": 150
                },
                "time_to_get": "1:00"
            },
            {
                "coords": {
                    "lat": 111,
                    "long": 300
                },
                "time_points": [{
                    "starts": "10:00",
                    "lasts": "1:00",
                    "price": 150
                }],
                "time_to_get": "1:00"
            }],
            "matrices": [
                [0,0,0,0],
                [0,0,0,0],
                [0,0,0,0],
                [0,0,0,0]
            ]
        })";

        auto runner = Runner::create();

        auto result = runner->run(input);

        std::cout << result << std::endl;
    }

}
#endif //PATHBUILDER_RUNNERIMPL1_H
