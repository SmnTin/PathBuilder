#include "pbuilder/PathBuilder.h"

#include <algorithm>
#include <queue>
#include <iostream>

namespace pbuilder {

    class PathBuilderImpl1 : public PathBuilder {
    public:
        PathBuilderImpl1() = default;

        ~PathBuilderImpl1() = default;

        Result build() override {
            _prepareInput();

            return _calcResult();
        }

        void setMaxBlockSize(size_t maxBlockSize) override {
            _maxBlockSize = maxBlockSize;
        }

    protected:
        size_t _maxBlockSize = PATH_BUILDER_MAX_BLOCK_SIZE;

        int _count(int mask) {
            return __builtin_popcount(mask);
        }

        bool _bit(int mask, int i) {
            return (bool) ((mask >> i) & 1);
        }

        ShPtr<MatInt> _resultedMat;

        std::vector<Place::Id> _unvisited;

        void _prepareInput() {
            _preparePlaces();
            _prepareMatrices();
        }

        void _preparePlaces() {
            std::vector<ShPtr<Place>> newPlaces;

            for (auto &place : _places) {
                if (!place->visitable(_dayStart, _dayEnd)) {
                    _unvisited.push_back(place->id);
                } else {
                    newPlaces.push_back(place);
                }
            }

            _places = newPlaces;
        }

        void _prepareMatrices() {
            if (_matrices.empty())
                throw std::runtime_error("No matrices were given");

            size_t n = _matrices[0]->cols();
            _resultedMat = std::make_shared<MatInt>(n, n, INF);

            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    //we take corresponding value based on chosen transport (foot-walking, public-transport or car)
                    _resultedMat->at(i, j) = _matrices[_matrices[numberOfTransports]->at(i, j)]->at(i, j);
                }
            }

            for (auto &placeFrom : _places)
                for (auto &placeTo : _places)
                    if (placeFrom != placeTo) {
                        if (placeFrom->custom() && !placeTo->custom()) {
                            auto placeFromCasted = std::dynamic_pointer_cast<CustomPlace>(placeFrom);

                            if (placeFromCasted->hasCondition(CustomPlace::Condition::DAY_START))
                                _resultedMat->at(placeTo->id, placeFrom->id) = INF;
                        } else if (placeFrom->custom() && placeTo->custom()) {
                            auto placeFromCasted = std::dynamic_pointer_cast<CustomPlace>(placeFrom);
                            auto placeToCasted = std::dynamic_pointer_cast<CustomPlace>(placeTo);

                            if (placeFromCasted->hasCondition(CustomPlace::Condition::DAY_START) &&
                                !placeToCasted->hasCondition(CustomPlace::Condition::DAY_START))
                                _resultedMat->at(placeTo->id, placeFrom->id) = INF;

                            if (!placeFromCasted->hasCondition(CustomPlace::Condition::DAY_END) &&
                                placeToCasted->hasCondition(CustomPlace::Condition::DAY_END))
                                _resultedMat->at(placeTo->id, placeFrom->id) = INF;
                        } else if (!placeFrom->custom() && placeTo->custom()) {
                            auto placeToCasted = std::dynamic_pointer_cast<CustomPlace>(placeTo);

                            if (placeToCasted->hasCondition(CustomPlace::Condition::DAY_END))
                                _resultedMat->at(placeTo->id, placeFrom->id) = INF;
                        }
                    }
        }

        //the "core" function. It calculates an optimal route through all given vertices;
        ShPtr<Block> _hamilton(const std::vector<Place::Id> &pts, uint day, uint dayOfWeek, int savedMask = 0) {

            size_t n = pts.size();
            int M = (1 << n);

            std::vector<std::vector<int>>
                    dp(M, std::vector<int>(n, INF)), //dynamic programming function that we are optimizing
                    p(M, std::vector<int>(n,
                                          -1)); //the point that is chosen on the certain iteration as the point we came from

            std::vector<std::vector<Interval>> p_interval(M, std::vector<Interval>(n));

            //values needed to restore the vertices order
            std::vector<int> min_val(n + 1, INF), min_mask(n + 1, 0);
            std::vector<size_t> min_i(n + 1, 0);
            int max_val = 0;

            //calculating DP
            //mask represents subset of pts where 1 means i-th element is in subset whilst 0 means it's not
            for (int mask = 1; mask < M; ++mask) {
                int cnt = _count(mask);

                for (size_t i = 0; i < n; ++i) {
                    auto place = _places[pts[i]];

                    if (!_bit(mask, i)) {
                        continue;
                    } else if (_count(mask) == 1) {
                        // base case
                        TimePoint dayStart = (day == 0 ? _firstDayStart : _dayStart);
                        p_interval[mask][i] = place->nearestTime(dayStart + place->timeToGet, dayOfWeek);
                        dp[mask][i] = (p_interval[mask][i].starts +
                                       p_interval[mask][i].lasts).getTimePoint();
                    } else {
                        //on that step we are trying to build a transition from smaller subset of pts to the current through the edge (j, i)
                        for (size_t j = 0; j < n; ++j) {
                            if (i != j && _bit(mask, j)) {
                                auto interval = place->nearestTime(
                                        TimePoint(dp[mask ^ (1 << i)][j] +
                                                  _resultedMat->at(_places[pts[j]]->id, _places[pts[i]]->id)),
                                        dayOfWeek);
//                                std::cout << mask << " " << i << " " << j << " " << interval.starts.getTimePoint() << " | ";
//                                std::cout << (mask^(1 << i)) << " " << dp[mask^(1 << i)][j] << " " << _resultedMat->at(pts[j], pts[i]) << "\n";

                                int res = (interval.starts +
                                           interval.lasts).getTimePoint();

                                if (res > _dayEnd.getTimePoint())
                                    res = INF;

                                if (TimePoint(res) <= _dayEnd && res < dp[mask][i] &&
                                    (cnt < _count(savedMask) || (mask & savedMask) == savedMask)) {
                                    dp[mask][i] = res;
                                    p[mask][i] = j;
                                    p_interval[mask][i] = interval;
                                }
                            }
                        }
                    }

                    if (min_val[cnt] > dp[mask][i]) {
                        min_val[cnt] = dp[mask][i];
                        min_mask[cnt] = mask;
                        min_i[cnt] = i;

                        max_val = std::max(max_val, cnt);
                    }
                }
            }

            //retrieving result
            auto block = std::make_shared<Block>();
            block->dayOfWeek = dayOfWeek;
            block->day = day;

            if (max_val > 0) {
                int cur_mask = min_mask[max_val];
                int cur_i = min_i[max_val];

                block->mask = cur_mask;

                while (cur_i > -1) {
                    auto place = std::make_shared<PlaceVisited>();

                    place->interval = p_interval[cur_mask][cur_i];
                    place->id = _places[pts[cur_i]]->id;
                    place->originalId = _places[pts[cur_i]]->originalId;

                    block->price += place->interval.price;

                    block->order.push_back(place);

                    int mask1 = (cur_mask ^ (1 << cur_i));
                    int i1 = p[cur_mask][cur_i];

                    cur_mask = mask1;
                    cur_i = i1;
                }

                //the result is found in reversed order
                std::reverse(block->order.begin(), block->order.end());
            }

            return block;
        }

        ShPtr<Block>
        _hamiltonReorder(const ShPtr<Block> &origBlock, uint day, uint dayOfWeek) {
            std::vector<Place::Id> pts;
            for (auto pl : origBlock->order)
                pts.push_back(pl->id);

            size_t n = pts.size();
            int M = (1 << n);

            std::vector<std::vector<std::pair<double, int>>>
                    dp(M, std::vector<std::pair<double, int>>(n, {INF,
                                                                  INF})); //dynamic programming function that we are optimizing
            std::vector<std::vector<int>>
                    p(M, std::vector<int>(n,
                                          -1)); //the point that is chosen on the certain iteration as the point we came from

            std::vector<std::vector<double>> p_dist(M, std::vector<double>(n));
            std::vector<std::vector<Interval>> p_interval(M, std::vector<Interval>(n));

            //values needed to restore the vertices order
            std::vector<std::pair<double, int>> min_val(n + 1, {INF, INF});
            std::vector<int> min_mask(n + 1, 0);
            std::vector<size_t> min_i(n + 1, 0);
            int max_val = 0;

            //calculating DP
            //mask represents subset of pts where 1 means i-th element is in subset whilst 0 means it's not
            for (int mask = 1; mask < M; ++mask) {
                int cnt = _count(mask);

                for (size_t i = 0; i < n; ++i) {
                    auto place = _places[pts[i]];

                    if (!_bit(mask, i)) {
                        continue;
                    } else if (_count(mask) == 1) {
                        // base case
                        TimePoint dayStart = (day == 0 ? _firstDayStart : _dayStart);
                        p_interval[mask][i] = place->nearestTime(dayStart + place->timeToGet, dayOfWeek);
                        p_dist[mask][i] = 0;
                        dp[mask][i] = {p_dist[mask][i], (p_interval[mask][i].starts +
                                                         p_interval[mask][i].lasts).getTimePoint()};
                    } else {
                        //on that step we are trying to build a transition from smaller subset of pts to the current through the edge (j, i)
                        for (size_t j = 0; j < n; ++j) {
                            if (i != j && _bit(mask, j)) {
                                auto interval = place->nearestTime(
                                        TimePoint(dp[mask ^ (1 << i)][j].second +
                                                  _resultedMat->at(_places[pts[j]]->id, _places[pts[i]]->id)),
                                        dayOfWeek);
//                                std::cout << mask << " " << i << " " << j << " " << interval.starts.getTimePoint() << " | ";
//                                std::cout << (mask^(1 << i)) << " " << dp[mask^(1 << i)][j] << " " << _resultedMat->at(pts[j], pts[i]) << "\n";

                                double res_d = dp[mask ^ (1 << i)][j].first +
                                               distance(_places[pts[j]]->coords, _places[pts[i]]->coords);
                                int res_i = (interval.starts +
                                             interval.lasts).getTimePoint();

                                if (res_i > _dayEnd.getTimePoint())
                                    res_i = INF;

                                std::pair<double, int> res = {res_d, res_i};

                                if (TimePoint(res_i) <= _dayEnd && res < dp[mask][i]) {
                                    dp[mask][i] = res;
                                    p[mask][i] = j;
                                    p_interval[mask][i] = interval;
                                }
                            }
                        }
                    }

                    if (min_val[cnt] > dp[mask][i]) {
                        min_val[cnt] = dp[mask][i];
                        min_mask[cnt] = mask;
                        min_i[cnt] = i;

                        max_val = std::max(max_val, cnt);
                    }
                }
            }

            //retrieving result
            auto block = std::make_shared<Block>();
            block->dayOfWeek = dayOfWeek;
            block->day = day;

            if (max_val > 0) {
                int cur_mask = min_mask[max_val];
                int cur_i = min_i[max_val];

                block->mask = cur_mask;

                while (cur_i > -1) {
                    auto place = std::make_shared<PlaceVisited>();

                    place->interval = p_interval[cur_mask][cur_i];
                    place->id = _places[pts[cur_i]]->id;
                    place->originalId = _places[pts[cur_i]]->originalId;

                    block->price += place->interval.price;

                    block->order.push_back(place);

                    int mask1 = (cur_mask ^ (1 << cur_i));
                    int i1 = p[cur_mask][cur_i];

                    cur_mask = mask1;
                    cur_i = i1;
                }

                //the result is found in reversed order
                std::reverse(block->order.begin(), block->order.end());
            }

            if (block->order.size() == origBlock->order.size())
                return block;
            else
                return origBlock;
        }

        bool _satisfiedAllCriticalPlaces(uint day) {
            for (auto &_place : _places) {
                auto place = std::dynamic_pointer_cast<CustomPlace>(_place);

                if (place && place->hasOverDay(day)) {
                    return false;
                }
            }

            return true;
        }

        Result _calcResult() {
            //TODO: impl2
            return _calcResultImpl1();
        }

        Result _calcResultImpl1() {
            Result result;

            uint dayOfWeek = _dayOfWeek, day = 0;
            std::queue<int>
                    toVisit = _orderedPlaces(day, dayOfWeek),
                    visited;

            while (!toVisit.empty() || !_satisfiedAllCriticalPlaces(day)) {
                toVisit = _orderedPlaces(toVisit, day, dayOfWeek);

                std::vector<int> places;
                int mask = 0;
                auto block = std::make_shared<Block>();

                while (!toVisit.empty()
                       && _count(mask) < _maxBlockSize
                        ) {
                    //take places from not visited queue to fit the block size
                    while (places.size() < _maxBlockSize && !toVisit.empty()) {
                        places.push_back(toVisit.front());
                        toVisit.pop();
                    }

                    //apply DP algorithm to find optimal route
                    auto curBlock = _hamilton(places, day, dayOfWeek, mask);
                    mask = curBlock->mask;
                    block = curBlock;

                    std::vector<int> newPlaces;

                    //push to "visited" queue unused places to use them on the next iterations
                    for (size_t i = 0; i < places.size(); ++i) {
                        if (_bit(mask, i)) {
                            newPlaces.push_back(places[i]);
                        } else {
                            visited.push(places[i]);
                        }
                    }

                    mask = (1 << newPlaces.size()) - 1;
                    places = newPlaces;
                }

                block = _hamiltonReorder(block, day, dayOfWeek);

                result.blocks.push_back(block);
                result.price += block->price;
                places.clear();

                while (!visited.empty()) {
                    toVisit.push(visited.front());
                    visited.pop();
                }

                //move to the next day of a week
                dayOfWeek++;
                dayOfWeek %= DAYS_IN_WEEK;
                day++;
            }

            result.unvisited = _unvisited;
            return result;
        }

        std::queue<int> _orderedPlaces(std::vector<int> pts, uint day, uint dayOfWeek) {
            std::vector<std::tuple<bool, int, double, int>> order;
            std::queue<int> result;

            std::vector<bool> placesUsed(_places.size(), false);
            for (size_t i = 0; i < pts.size(); ++i) {
                placesUsed[pts[i]] = true;
            }
            for (size_t i = 0; i < _places.size(); ++i)
                if (!placesUsed[i] && _places[i]->critical(day))
                    pts.push_back(i);

            size_t criticalPlacesCnt = 0;
            for (size_t i = 0; i < pts.size(); ++i) {
                criticalPlacesCnt += _places[pts[i]]->critical(day);

                if (!_places[pts[i]]->critical(day) && _places[pts[i]]->custom())
                    continue;

                order.emplace_back(!_places[pts[i]]->critical(day),
                                   _places[pts[i]]->daysOfWeekComparator(dayOfWeek, (dayOfWeek + 1) % DAYS_IN_WEEK),
                                   distance(_places[pts[i]]->coords, _coordinates), pts[i]);
            }
            if (criticalPlacesCnt > PATH_BUILDER_MAX_BLOCK_SIZE)
                throw std::runtime_error("Too many custom places.");

            std::sort(order.begin(), order.end());

            for (size_t i = 0; i < order.size(); ++i)
                result.push(std::get<3>(order[i]));

            return result;
        }

        std::queue<int> _orderedPlaces(std::queue<int> ptsq, uint day, uint dayOfWeek) {
            std::vector<int> pts;
            while (!ptsq.empty()) {
                pts.push_back(ptsq.front());
                ptsq.pop();
            }

            return _orderedPlaces(pts, day, dayOfWeek);
        }

        std::queue<int> _orderedPlaces(uint day, uint dayOfWeek) {
            std::vector<int> pts;
            for (size_t i = 0; i < _places.size(); ++i)
                pts.push_back(i);

            return _orderedPlaces(pts, day, dayOfWeek);
        }

    };

    ShPtr<PathBuilder> PathBuilder::createImpl1() {
        return std::make_shared<PathBuilderImpl1>();
    }
}