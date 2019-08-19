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

        virtual bool visitable(TimePoint dayStart, TimePoint dayEnd) const = 0;

        //get nearest existing interval right after certain time point
        virtual Interval nearestTime(const TimePoint &timePoint, size_t dayOfWeek) const = 0;

        virtual bool daysOfWeekComparator(size_t dayA, size_t dayB) const = 0;

        virtual bool operator<(const Place &b) const = 0;

    };

    class TimetableElement {
    public:
        virtual Interval nearestTime(const TimePoint &timePoint) const = 0;
    };

    class FixedTimetableElement : public TimetableElement {
    public:
        FixedTimetableElement(TimePoint starts_,
                              TimePoint lasts_,
                              int price_);

        TimePoint starts, lasts;
        int price;

        Interval nearestTime(const TimePoint &timePoint) const override;
    };

    class FreeTimetableElement : public TimetableElement {
    public:
        FreeTimetableElement(TimePoint starts_,
                             TimePoint ends_,
                             TimePoint lasts_,
                             int price_);

        TimePoint starts, ends, lasts;
        int price;

        Interval nearestTime(const TimePoint &timePoint) const override;
    };

    class PlaceWithMixedTimetable : public Place {
    public:
        PlaceWithMixedTimetable();

        PlaceWithMixedTimetable(const Coordinates &coords_,
                                const TimePoint &timeToGet_,
                                Id id_ = 0);

        void addTimetableElement(ShPtr<TimetableElement> element, size_t dayOfWeek);

        bool visitable(TimePoint dayStart, TimePoint dayEnd) const override;

        Interval nearestTime(const TimePoint &timePoint, size_t dayOfWeek) const override;

        bool daysOfWeekComparator(size_t dayA, size_t dayB) const override;

        bool operator<(const Place &b) const override;

    private:
        std::vector<std::vector<ShPtr<TimetableElement>>> _timetable;
    };

} //pbuilder

#endif //PATHBUILDER_PLACE_H
