#ifndef PATHBUILDER_RUNNERIMPL1_H
#define PATHBUILDER_RUNNERIMPL1_H

#include <pbuilder/Parser.h>
#include "gtest/gtest.h"

#include "pbuilder/Runner.h"

using namespace pbuilder;

namespace {

    TEST(Runner, GeneralTest1) {
        std::string input = R"(
{
    "mode": "full",
    "starting_pos": {
      "lat": 111,
      "long": 222
    },
    "first_day_start": "8:00",
    "day_start": "8:00",
    "day_end": "23:00",
    "day_of_week": "monday",
    "places": [
      {
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
            }
          ]
        },
        "time_to_get": "1:00"
      },
      {
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
            }
          ]
        },
        "time_to_get": "1:00"
      }
    ],
    "matrices": [
      [10,10,10,10],
      [20,20,20,20],
      [30,30,30,30],
      [1,1,1,1]
    ]
  }
        )";

        auto runner = Runner::create();

        auto result = runner->run(input);

//        std::cout << result << std::endl;
    }

    TEST(Runner, GeneralTest2) {
        std::string input = R"(
{
  "mode": "full",
  "starting_pos": {
    "lat": 50.084243,
    "long": 8.719368
  },
  "first_day_start": "8:00",
  "day_start": "8:00",
  "day_end": "23:00",
  "places": [
    {
      "coords": {
        "lat": 50.084243,
        "long": 8.719368
      },
      "interval": {
        "starts": "9:00",
        "ends": "18:00",
        "lasts": "0:45",
        "price": 100
      },
      "time_to_get": "00:00"
    },
    {
      "coords": {
        "lat": 50.140496,
        "long": 8.754902
      },
      "interval": {
        "starts": "12:00",
        "ends": "22:00",
        "lasts": "1:15",
        "price": 100
      },
      "time_to_get": "00:00"
    },
    {
      "coords": {
        "lat": 50.154796,
        "long": 8.702202
      },
      "interval": {
        "starts": "11:00",
        "ends": "21:00",
        "lasts": "1:30",
        "price": 100
      },
      "time_to_get": "00:20"
    },
    {
      "coords": {
        "lat": 50.151606,
        "long": 8.534317
      },
      "interval": {
        "starts": "11:00",
        "ends": "16:00",
        "lasts": "0:30",
        "price": 100
      },
      "time_to_get": "00:23"
    },
    {
      "coords": {
        "lat": 50.218545,
        "long": 8.574829
      },
      "interval": {
        "starts": "11:00",
        "ends": "16:00",
        "lasts": "2:00",
        "price": 100
      },
      "time_to_get": "00:27"
    },
    {
      "coords": {
        "lat": 50.112433,
        "long": 8.613453
      },
      "time_points": [
        {
          "starts": "11:30",
          "lasts": "2:10",
          "price": 150
        },
        {
          "starts": "12:30",
          "lasts": "2:10",
          "price": 150
        },
        {
          "starts": "13:15",
          "lasts": "2:10",
          "price": 150
        },
        {
          "starts": "14:10",
          "lasts": "2:10",
          "price": 150
        },
        {
          "starts": "15:20",
          "lasts": "2:10",
          "price": 150
        }
      ],
      "time_to_get": "00:15"
    },
    {
      "coords": {
        "lat": 50.140496,
        "long": 8.754902
      },
      "time_points": [
        {
          "starts": "10:30",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "11:30",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "13:00",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "14:25",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "15:20",
          "lasts": "0:35",
          "price": 150
        }
      ],
      "time_to_get": "00:19"
    },
    {
      "coords": {
        "lat": 50.147426,
        "long": 8.632164
      },
      "time_points": [
        {
          "starts": "9:30",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "11:15",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "12:00",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "13:25",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "18:20",
          "lasts": "0:45",
          "price": 150
        }
      ],
      "time_to_get": "00:25"
    },
    {
      "coords": {
        "lat": 50.130483,
        "long": 8.648472
      },
      "time_points": [
        {
          "starts": "9:30",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "11:30",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "12:30",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "13:30",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "14:30",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "15:30",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "16:30",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "19:30",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "20:30",
          "lasts": "0:45",
          "price": 150
        },
        {
          "starts": "21:30",
          "lasts": "0:45",
          "price": 150
        }
      ],
      "time_to_get": "00:22"
    },
    {
      "coords": {
        "lat": 50.083692,
        "long": 8.646755
      },
      "time_points": [
        {
          "starts": "9:00",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "11:00",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "12:00",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "13:00",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "14:30",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "15:00",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "16:00",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "19:00",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "20:00",
          "lasts": "0:35",
          "price": 150
        },
        {
          "starts": "21:30",
          "lasts": "0:35",
          "price": 150
        }
      ],
      "time_to_get": "00:14"
    }
  ],
  "matrices": [
      [0,0,20,23,27,15,19,25,22,14,
      0,0,20,23,27,15,19,25,22,14,
      19,19,0,25,19,21,16,22,16,24,
      28,28,27,0,24,10,32,19,17,17,
      27,27,21,22,0,18,26,23,4,23,
      22,22,21,11,18,0,26,12,9,11,
      18,18,14,31,26,24,0,28,23,23,
      27,27,21,15,21,11,27,0,9,18,
      23,23,16,17,22,10,24,9,0,16,
      16,16,24,17,22,9,25,19,15,0],

      [0,0,114,208,252,117,103,128,99,75,
      0,0,114,208,252,117,103,128,99,75,
      114,114,0,176,165,118,62,72,73,123,
      208,208,176,0,124,108,231,108,128,166,
      252,252,165,124,0,186,226,127,159,225,
      117,117,118,108,186,0,146,67,49,70,
      103,103,62,231,226,146,0,126,109,137,
      128,128,72,108,127,67,126,0,33,103,
      99,99,73,128,159,49,109,33,0,77,
      75,75,123,166,225,70,137,103,77,0],

      [0,0,20,23,27,15,19,25,22,14,
      0,0,20,23,27,15,19,25,22,14,
      19,19,0,25,19,21,16,22,16,24,
      28,28,27,0,24,10,32,19,17,17,
      27,27,21,22,0,18,26,23,4,23,
      22,22,21,11,18,0,26,12,9,11,
      18,18,14,31,26,24,0,28,23,23,
      27,27,21,15,21,11,27,0,9,18,
      23,23,16,17,22,10,24,9,0,16,
      16,16,24,17,22,9,25,19,15,0],

      [1,1,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0]
    ]
}
  )";

        auto runner = Runner::create();

//        auto result = runner->run(input);
//
//        std::cout << result << std::endl;
    }

}
#endif //PATHBUILDER_RUNNERIMPL1_H
