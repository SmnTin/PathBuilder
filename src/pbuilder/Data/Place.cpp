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

    PlaceWithTimetable::PlaceWithTimetable() {}

    PlaceWithTimetable::PlaceWithTimetable(const Coordinates & coords_,
                                           std::vector<Interval> intervals_,
                                           const TimePoint & timeToGet_,
                                           Id id_) {
        id = id_;
        coords = coords_;
        timeToGet = timeToGet_;
        for(auto & item : intervals_)
            intervals.insert(item);
    }
    PlaceWithTimetable::PlaceWithTimetable(const Coordinates & coords_,
                                           std::set<Interval> intervals_,
                                           const TimePoint & timeToGet_,
                                           Id id_)
                : intervals(std::move(intervals_)) {
        id = id_;
        coords = coords_;
        timeToGet = timeToGet_;
    }

    //get nearest existing interval right after certain time point
    Interval PlaceWithTimetable::nearestTime(const TimePoint & timePoint) {
        Interval interval;
        interval.starts = timePoint;

        //binary search
        auto it = intervals.lower_bound(interval);
        if (it == intervals.end()) {
            //in case we found nothing appropriate
            Interval badInterval;
            badInterval.starts = TimePoint(INF);

            return badInterval;
        }
        return *it;

    }

    bool PlaceWithTimetable::operator<(const Place &b) const {
        return timeToGet < b.timeToGet;
    }

    PlaceWithFreeTime::PlaceWithFreeTime(const pbuilder::Coordinates &coords_,
                                         pbuilder::TimePoint visitingStart_,
                                         pbuilder::TimePoint visitingEnd_,
                                         pbuilder::TimePoint visitingDuration_,
                                         int price_,
                                         const pbuilder::TimePoint &timeToGet_,
                                         pbuilder::Place::Id id_) {
        coords = coords_;
        timeToGet = timeToGet_;
        price = price_;
        id = id_;
        visitingStart = visitingStart_;
        visitingDuration = visitingDuration_;
        visitingEnd = visitingEnd_;
    }

    PlaceWithFreeTime::PlaceWithFreeTime() = default;

    Interval PlaceWithFreeTime::nearestTime(const pbuilder::TimePoint &timePoint) {
        if(timePoint >= visitingStart && timePoint <= visitingEnd - visitingDuration) {
            Interval interval;

            interval.starts = timePoint;
            interval.lasts = visitingDuration;
            interval.price = price;
            return interval;
        } else {
            Interval badInterval;
            badInterval.starts = TimePoint(INF);

            return badInterval;
        }
    }

    bool PlaceWithFreeTime::operator<(const pbuilder::Place &b) const {
        return id < b.id;
    }

} //pbuilder
