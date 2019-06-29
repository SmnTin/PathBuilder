#ifndef PATHBUILDER_PLACE_H
#define PATHBUILDER_PLACE_H

#include "pbuilder/Definitions.h"
#include "pbuilder/Data/TimePoint.h"

#include <vector>
#include <set>
#include <exception>
#include <cmath>
#include <stdio.h>
#include <string>
#include <stdexcept>

namespace pbuilder {

    struct Coordinates {
        int latitude = 0;
        int longitude = 0;
    };

    double distance(Coordinates p1, Coordinates p2);

    class Place {
    public:
        Place();

        typedef int Id;

        Place(const Coordinates & coords_,
                std::vector<Interval> intervals_,
                const TimePoint & timeToGet_);

        Place(const Coordinates & coords_,
                std::set<Interval> intervals_,
                const TimePoint & timeToGet_);

        Coordinates coords;
        std::set<Interval> intervals;
        TimePoint timeToGet;

        //get nearest existing interval right after certain time point
        Interval nearestTime(const TimePoint & timePoint);

        bool operator< (const Place & b) const;

    private:
        bool _anyTime = false;

    };

} //pbuilder

#endif //PATHBUILDER_PLACE_H
