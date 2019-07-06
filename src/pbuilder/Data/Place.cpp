#include "pbuilder/Data/Place.h"

namespace pbuilder {

    double distance(Coordinates p1, Coordinates p2) {
        const double pi = atan(-1);

        double R = 6371e3; // metres
        double f1 = p1.latitude * pi / 180;
        double f2 = p2.latitude * pi / 180;
        double df = (p2.latitude - p1.latitude) * pi / 180;
        double dl = (p2.longitude - p1.longitude) * pi / 180;

        double a = sin(df/2) * sin(df/2) +
                cos(f1) * cos(f2) *
                sin(dl/2) * sin(dl/2);
        double c = 2 * atan2(sqrt(a), sqrt(1-a));

        double d = R * c;

        return d;
    }

    PlaceWithMixedTimetable::PlaceWithMixedTimetable() = default;

    PlaceWithMixedTimetable::PlaceWithMixedTimetable(const Coordinates & coords_,
                                                     const std::vector<TimePoint> & timesToGet_,
                                                     int chosen_,
                                                     Id id_) {
        id = id_;
        coords = coords_;
        timesToGet = timesToGet_;
        chosen = chosen_;

        _timetable.assign(DAYS_IN_WEEK, std::vector<ShPtr<TimetableElement>>());
    }

    //get nearest existing interval right after certain time point
    Interval PlaceWithMixedTimetable::nearestTime(const TimePoint & timePoint, int dayOfWeek) const {
        Interval result;
        result.starts = TimePoint(INF);

        for(auto & elPtr : _timetable[dayOfWeek])
            result = std::min(result, elPtr->nearestTime(timePoint));

        return result;
    }

    void PlaceWithMixedTimetable::addTimetableElement(ShPtr<pbuilder::TimetableElement> element, int dayOfWeek) {
        _timetable[dayOfWeek].push_back(element);
    }

    bool PlaceWithMixedTimetable::operator<(const Place &b) const {
        return id < b.id;
    }

    bool PlaceWithMixedTimetable::visitable(TimePoint dayStart, TimePoint dayEnd) const {
        for(int dayOfWeek = 0; dayOfWeek < DAYS_IN_WEEK; dayOfWeek++) {
            auto interval = nearestTime(dayStart + timesToGet[chosen], dayOfWeek);
            if(interval.starts + interval.lasts < dayEnd)
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
        if(timePoint > starts) {
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
        if(timePoint > ends - lasts) {
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

    bool PlaceWithMixedTimetable::daysOfWeekComparator(int dayA, int dayB) const {
        return _timetable[dayA].size() < _timetable[dayB].size();
    }

} //pbuilder
