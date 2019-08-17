#ifndef PATHBUILDER_TIMEPOINT_H
#define PATHBUILDER_TIMEPOINT_H

#include "pbuilder/Definitions.h"

#include <vector>
#include <exception>
#include <cmath>
#include <stdio.h>
#include <string>
#include <stdexcept>

namespace pbuilder {

    //TimePoint is scaled time variable to limit DP brute-force labour
    class TimePoint {
    public:
        explicit TimePoint(int time_ = 0, int startingTime = 0, int interval = 1);

        static TimePoint fromTimePoint(int timePoint, int startingTime = 0, int interval = 1);

        static TimePoint fromString(
                std::string timeStr,
                std::string startingTimeStr = "00:00",
                int interval = 1);

        std::string toString();

        int getTime() const;

        int getStartingTime() const;

        int getInterval() const;

        int getTimePoint() const;

        TimePoint operator+(const TimePoint &b) const;

        TimePoint operator-(const TimePoint &b) const;

        bool operator<(const TimePoint &b) const;

        bool operator<=(const TimePoint &b) const;

        bool operator>(const TimePoint &b) const;

        bool operator>=(const TimePoint &b) const;

        bool operator==(const TimePoint &b) const;

        bool operator!=(const TimePoint &b) const;

    private:
        int _t, _t0, _d;

        static int _parseTimeString(std::string timeStr);

        static std::string _toTimeString(int t);

        void _checkEquality(const TimePoint &b) const;
    };

    class Interval {
    public:
        TimePoint starts;
        TimePoint lasts;
        int price = 0;

        bool operator<(const Interval &b) const;

        bool operator<=(const Interval &b) const;

        bool operator>(const Interval &b) const;

        bool operator>=(const Interval &b) const;

        bool operator==(const Interval &b) const;

        bool operator!=(const Interval &b) const;
    };
} //pbuilder

#endif //PATHBUILDER_TIMEPOINT_H
