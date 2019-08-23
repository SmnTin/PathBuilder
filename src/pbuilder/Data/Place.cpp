#include "pbuilder/Data/Place.h"

namespace pbuilder {

    double distance(Coordinates p1, Coordinates p2) {
        const double pi = atan(-1);

        double R = 6371e3; // metres
        double f1 = p1.latitude * pi / 180;
        double f2 = p2.latitude * pi / 180;
        double df = (p2.latitude - p1.latitude) * pi / 180;
        double dl = (p2.longitude - p1.longitude) * pi / 180;

        double a = sin(df / 2) * sin(df / 2) +
                   cos(f1) * cos(f2) *
                   sin(dl / 2) * sin(dl / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));

        double d = R * c;

        return d;
    }

    Place::Place() = default;

    Place::Place(const Coordinates &coords_, const TimePoint &timeToGet_,
                 Place::Id id_) {
        id = id_;
        coords = coords_;
        timeToGet = timeToGet_;
    }

    PlaceWithMixedTimetable::PlaceWithMixedTimetable() {
        _timetable.assign(DAYS_IN_WEEK, std::vector<ShPtr<TimetableElement>>());
    }

    PlaceWithMixedTimetable::PlaceWithMixedTimetable(const Coordinates &coords_,
                                                     const TimePoint &timeToGet_,
                                                     Id id_) : Place(coords_, timeToGet_, id_) {
        _timetable.assign(DAYS_IN_WEEK, std::vector<ShPtr<TimetableElement>>());
    }

    //get nearest existing interval right after certain time point
    Interval PlaceWithMixedTimetable::nearestTime(const TimePoint &timePoint, size_t dayOfWeek) const {
        Interval result;
        result.starts = TimePoint(INF);

        for (auto &elPtr : _timetable[dayOfWeek])
            result = std::min(result, elPtr->nearestTime(timePoint));

        return result;
    }

    void PlaceWithMixedTimetable::addTimetableElement(ShPtr<TimetableElement> element, size_t dayOfWeek) {
        _timetable[dayOfWeek].push_back(element);
    }

    bool PlaceWithMixedTimetable::operator<(const Place &b) const {
        return id < b.id;
    }

    bool PlaceWithMixedTimetable::visitable(TimePoint dayStart, TimePoint dayEnd) const {
        for (size_t dayOfWeek = 0; dayOfWeek < DAYS_IN_WEEK; dayOfWeek++) {
            auto interval = nearestTime(dayStart + timeToGet, dayOfWeek);
            if (interval.starts + interval.lasts < dayEnd)
                return true;
        }
        return false;
    }

    FixedTimetableElement::FixedTimetableElement(TimePoint starts_, TimePoint lasts_, int price_) {
        starts = starts_;
        lasts = lasts_;
        price = price_;
    }

    Interval FixedTimetableElement::nearestTime(const TimePoint &timePoint) const {
        if (timePoint > starts) {
            Interval badInterval;
            badInterval.starts = TimePoint(INF);
            return badInterval;
        } else {
            Interval result;
            result.starts = starts;
            result.lasts = lasts;
            result.price = price;
            return result;
        }
    }

    FreeTimetableElement::FreeTimetableElement(TimePoint starts_, TimePoint ends_,
                                               TimePoint lasts_, int price_) {
        starts = starts_;
        lasts = lasts_;
        ends = ends_;
        price = price_;
    }

    Interval FreeTimetableElement::nearestTime(const TimePoint &timePoint) const {
        if (timePoint > ends - lasts) {
            Interval badInterval;
            badInterval.starts = TimePoint(INF);
            return badInterval;
        } else {
            Interval result;
            result.starts = std::max(timePoint, starts);
            result.lasts = lasts;
            result.price = price;
            return result;
        }
    }

    bool PlaceWithMixedTimetable::daysOfWeekComparator(size_t dayA, size_t dayB) const {
        return _timetable[dayA].size() < _timetable[dayB].size();
    }

    bool CustomPlace::critical(uint day) const {
        if (hasCondition(Condition::EVERY_DAY))
            return true;

        for (uint _day : _days)
            if (_day == day)
                return true;

        return false;
    }

    void CustomPlace::addCondition(CustomPlace::Condition condition) {
        _conditions[condition] = true;
    }

    bool CustomPlace::hasCondition(CustomPlace::Condition condition) const {
        return _conditions[condition];
    }

    void CustomPlace::addDay(uint day) {
        _days.push_back(day);
    }

    bool CustomPlace::hasOverDay(uint day) {
        for (uint _day : _days)
            if (_day >= day)
                return true;

        return false;
    }

    CustomPlace::CustomPlace() = default;

    CustomPlace::CustomPlace(const Coordinates &coords_, const TimePoint &timeToGet_,
                             Place::Id id_) : PlaceWithMixedTimetable(coords_, timeToGet_, id_) {}

    bool CustomPlace::custom() const {
        return true;
    }

    bool PlaceWithMixedTimetable::custom() const {
        return false;
    }

    bool PlaceWithMixedTimetable::critical(uint day) const {
        return false;
    }

} //pbuilder
