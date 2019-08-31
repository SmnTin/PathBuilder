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
        typedef std::string OriginalId;

        Place();

        Place(const Coordinates &coords_,
              const TimePoint &timeToGet_,
              Id id_ = 0);

        Coordinates coords;
        TimePoint timeToGet;
        Id id;
        OriginalId originalId;


        virtual bool visitable(TimePoint dayStart, TimePoint dayEnd) const = 0;

        virtual bool critical(uint day) const = 0;

        virtual bool custom() const = 0;

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

        bool custom() const override;

        bool critical(uint day) const override;

        void addTimetableElement(ShPtr<TimetableElement> element, size_t dayOfWeek);

        bool visitable(TimePoint dayStart, TimePoint dayEnd) const override;

        Interval nearestTime(const TimePoint &timePoint, size_t dayOfWeek) const override;

        bool daysOfWeekComparator(size_t dayA, size_t dayB) const override;

        bool operator<(const Place &b) const override;

    private:
        std::vector<std::vector<ShPtr<TimetableElement>>> _timetable;
    };

    class CustomPlace : public PlaceWithMixedTimetable {
    public:
        CustomPlace();

        CustomPlace(const Coordinates &coords_,
                    const TimePoint &timeToGet_,
                    Id id_ = 0);

        bool custom() const override;

        bool critical(uint day) const override;

        enum Condition {
            EVERY_DAY,
            DAY_START,
            DAY_END,
            CONDITIONS_COUNT = DAY_END + 1
        };

        virtual void addCondition(Condition condition);

        virtual bool hasCondition(Condition condition) const;

        virtual void addDay(uint day);

        virtual bool hasOverDay(uint day);

    protected:
        bool _conditions[CONDITIONS_COUNT] = {false};
        std::vector<uint> _days;
    };

} //pbuilder

#endif //PATHBUILDER_PLACE_H
