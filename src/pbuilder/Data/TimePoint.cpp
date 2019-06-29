#include "pbuilder/Data/TimePoint.h"

namespace pbuilder {

    TimePoint::TimePoint(int time_, int startingTime, int interval) {
        _t0 = startingTime;
        _d = interval;
        _t = (int) lround((time_ - startingTime) / (double) interval);
    }

    TimePoint TimePoint::fromTimePoint(int timePoint, int startingTime, int interval) {
        TimePoint res;
        res._t = timePoint;
        res._t0 = startingTime;
        res._d = interval;
        return res;
    }

    TimePoint TimePoint::fromString(
            std::string timeStr,
            std::string startingTimeStr,
            int interval) {

        return TimePoint(_parseTimeString(std::move(timeStr)),
                         _parseTimeString(std::move(startingTimeStr)),
                         interval);
    }

    std::string TimePoint::toString() {
        return _toTimeString(getTime());
    }

    int TimePoint::getTime() const {
        return _t0 + _t * _d;
    }

    int TimePoint::getStartingTime() const {
        return _t0;
    }

    int TimePoint::getInterval() const {
        return _d;
    }

    int TimePoint::getTimePoint() const {
        return _t;
    }

    TimePoint TimePoint::operator+(const TimePoint &b) const {
        _checkEquality(b);
        return TimePoint(_t + b._t, _t0, _d);
    }

    TimePoint TimePoint::operator-(const TimePoint &b) const {
        _checkEquality(b);
        return TimePoint(_t - b._t, _t0, _d);
    }

    bool TimePoint::operator<(const TimePoint &b) const {
        return getTime() < b.getTime();
    }

    bool TimePoint::operator<=(const TimePoint &b) const {
        return getTime() <= b.getTime();
    }

    bool TimePoint::operator>(const TimePoint &b) const {
        return getTime() > b.getTime();
    }

    bool TimePoint::operator>=(const TimePoint &b) const {
        return getTime() >= b.getTime();
    }

    bool TimePoint::operator==(const TimePoint &b) const {
        return getTime() == b.getTime();
    }

    bool TimePoint::operator!=(const TimePoint &b) const {
        return getTime() != b.getTime();
    }

    int TimePoint::_parseTimeString(std::string timeStr) {
        int h, m;
        int scanned = sscanf(timeStr.c_str(), "%d:%d", &h, &m);
        if (scanned != 2 || h < 0 || h >= 24 || m < 0 || m >= 60)
            throw std::runtime_error("Invalid value.");

        return h * 60 + m;
    }

    std::string TimePoint::_toTimeString(int t) {
        int h = t / 60;
        int m = t % 60;

        if (h < 0 || h >= 24 || m < 0 || m >= 60)
            throw std::runtime_error("Invalid value.");

        char str[6];
        sprintf(str, "%0.2d:%0.2d", h, m);

        return std::string(str);
    }


    void TimePoint::_checkEquality(const TimePoint &b) const {
        if (_d != b._d)
            throw std::runtime_error("TimePoints' intervals aren't equal.");
        if (_t0 != b._t0)
            throw std::runtime_error("TimePoints' starting times aren't equal.");
    }

    bool Interval::operator<(const Interval &b) const {
        if (starts != b.starts)
            return starts < b.starts;
        else if (lasts != b.lasts)
            return lasts < b.lasts;
        return price < b.price;
    }

    bool Interval::operator<=(const Interval &b) const {
        if (starts != b.starts)
            return starts < b.starts;
        else if (lasts != b.lasts)
            return lasts < b.lasts;
        return price <= b.price;
    }

    bool Interval::operator>(const Interval &b) const {
        if (starts != b.starts)
            return starts > b.starts;
        else if (lasts != b.lasts)
            return lasts > b.lasts;
        return price > b.price;
    }

    bool Interval::operator>=(const Interval &b) const {
        if (starts != b.starts)
            return starts > b.starts;
        else if (lasts != b.lasts)
            return lasts > b.lasts;
        return price >= b.price;
    }

    bool Interval::operator== (const Interval & b) const {
        return !((*this) < b) && !((*this) > b);
    }
    bool Interval::operator!= (const Interval & b) const {
        return ((*this) < b) || ((*this) > b);
    }
} //pbuilder