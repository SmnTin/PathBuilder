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

    Place::Place() : _anyTime(true) {}

    Place::Place(const Coordinates & coords_,
                std::vector<Interval> intervals_,
                const TimePoint & timeToGet_)
                : coords(coords_), timeToGet(timeToGet_) {
        for(auto & item : intervals_)
            intervals.insert(item);
    }
    Place::Place(const Coordinates & coords_,
                std::set<Interval> intervals_,
                const TimePoint & timeToGet_)
                : coords(coords_), timeToGet(timeToGet_), intervals(std::move(intervals_)) {}

    //get nearest existing interval right after certain time point
    Interval Place::nearestTime(const TimePoint & timePoint) {
        Interval interval;
        interval.starts = timePoint;
        if(_anyTime) {
            interval.starts = timePoint;
            interval.lasts = TimePoint(30);
            interval.price = 0;
            return interval;
        } else {
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

    }

    bool Place::operator<(const Place &b) const {
        return timeToGet < b.timeToGet;
    }

} //pbuilder
