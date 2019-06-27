#include "pbuilder/Data/Place.h"

namespace pbuilder {

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
                : coords(coords_), timeToGet(timeToGet_), intervals(intervals_) {}

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

} //pbuilder
