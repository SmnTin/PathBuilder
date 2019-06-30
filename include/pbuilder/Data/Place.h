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
        double latitude = 0;
        double longitude = 0;
    };

    double distance(Coordinates p1, Coordinates p2);

    class Place {
    public:
        typedef int Id;

        Coordinates coords;
        TimePoint timeToGet;
        Id id;

        //get nearest existing interval right after certain time point
        virtual Interval nearestTime(const TimePoint & timePoint) = 0;

        virtual bool operator< (const Place & b) const = 0;

    };


    class PlaceWithTimetable : public Place {
    public:
        PlaceWithTimetable();

        PlaceWithTimetable(const Coordinates & coords_,
                           std::vector<Interval> intervals_,
                           const TimePoint & timeToGet_,
                           Id id_ = 0);

        PlaceWithTimetable(const Coordinates & coords_,
                           std::set<Interval> intervals_,
                           const TimePoint & timeToGet_,
                           Id id_ = 0);

        std::set<Interval> intervals;

        Interval nearestTime(const TimePoint & timePoint) override;

        bool operator< (const Place & b) const override;

    };

    class PlaceWithFreeTime : public Place {
    public:
        PlaceWithFreeTime();

        PlaceWithFreeTime(const Coordinates & coords_,
                          TimePoint visitingStart_,
                          TimePoint visitingEnd_,
                          TimePoint visitingDuration_,
                          int price_,
                          const TimePoint & timeToGet_,
                          Id id_= 0);

        TimePoint visitingStart;
        TimePoint visitingEnd;
        TimePoint visitingDuration;
        int price;

        Interval nearestTime(const TimePoint & timePoint) override;

        bool operator< (const Place & b) const override;

    };

} //pbuilder

#endif //PATHBUILDER_PLACE_H
