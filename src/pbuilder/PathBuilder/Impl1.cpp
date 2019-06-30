#include "pbuilder/PathBuilder.h"

#include <algorithm>
#include <queue>
#include <iostream>

namespace pbuilder {

    class PathBuilderImpl1 : public PathBuilder {
    public:
        PathBuilderImpl1 () = default;
        ~PathBuilderImpl1 () = default;

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
            return (bool)((mask >> i)&1);
        }

        ShPtr<MatInt> _resultedMat;

        virtual void _prepareInput() {
            size_t n = _places.size();
            _resultedMat = std::make_shared<MatInt>(n, n, INF);

            for(size_t i = 0; i < n; ++i) {
                for(size_t j = 0; j < n; ++j) {
                    //we take corresponding value based on chosen transport (foot-walking, public-transport or car)
                    if(_matrices[3]->at(i, j) == 0)
                        _resultedMat->at(i, j) = _matrices[0]->at(i, j);
                    else if(_matrices[4]->at(i, j) == 1)
                        _resultedMat->at(i, j) = _matrices[1]->at(i, j);
                    else
                        _resultedMat->at(i, j) = _matrices[2]->at(i, j);
                }
            }
        }

        //the "core" function. It calculates an optimal route through all given vertices;
        virtual ShPtr<Block> _hamilton(const std::vector<Place::Id> & pts, int savedMask = 0) {

            size_t n = pts.size();
            int M = (1 << n);

            std::vector<std::vector<int>>
                    dp(M, std::vector<int>(n, INF)), //dynamic programming function that we are optimizing
                    p(M, std::vector<int>(n, -1)); //the point that is chosen on the certain iteration as the point we came from

            std::vector<std::vector<Interval>> p_interval(M, std::vector<Interval>(n));

            //values needed to restore the vertices order
            std::vector<int> min_val(n+1, INF), min_mask(n+1, 0), min_i(n+1, 0);
            int max_val = 0;

            //calculating DP
            //mask represents subset of pts where 1 means i-th element is in subset whilst 0 means it's not
            for(int mask = 1; mask < M; ++mask) {
                int cnt = _count(mask);

                for (size_t i = 0; i < n; ++i) {
                    auto place = _places[pts[i]];

                    if(!_bit(mask, i)) {
                        continue;
                    } else if(_count(mask) == 1) {
                        // base case
                        p_interval[mask][i] = place->nearestTime(_dayStart + place->timeToGet);
                        dp[mask][i] = (p_interval[mask][i].starts +
                                       p_interval[mask][i].lasts).getTimePoint();
                    } else {
                        //on that step we are trying to build a transition from smaller subset of pts to the current through the edge (j, i)
                        for (size_t j = 0; j < n; ++j) {
                            if(i != j && _bit(mask, j)) {
                                auto interval = place->nearestTime(TimePoint(dp[mask^(1 << i)][j] +
                                                                             _resultedMat->at(_places[pts[j]]->id, _places[pts[i]]->id)));
//                                std::cout << mask << " " << i << " " << j << " " << interval.starts.getTimePoint() << " | ";
//                                std::cout << (mask^(1 << i)) << " " << dp[mask^(1 << i)][j] << " " << _resultedMat->at(pts[j], pts[i]) << "\n";

                                int res = (interval.starts +
                                           interval.lasts).getTimePoint();

                                if(TimePoint(res) <= _dayEnd && res < dp[mask][i] && (cnt < _count(savedMask) || (mask&savedMask)==savedMask)) {
                                    dp[mask][i] = res;
                                    p[mask][i] = j;
                                    p_interval[mask][i] = interval;
                                }
                            }
                        }
                    }

                    if(min_val[cnt] > dp[mask][i]) {
                        min_val[cnt] = dp[mask][i];
                        min_mask[cnt] = mask;
                        min_i[cnt] = i;

                        max_val = std::max(max_val, cnt);
                    }
                }
            }

            //retrieving result
            auto block = std::make_shared<Block>();
            if(max_val > 0) {
                int cur_mask = min_mask[max_val];
                int cur_i = min_i[max_val];

                block->mask = cur_mask;

                while(cur_i > -1) {
                    auto place = std::make_shared<PlaceVisited>();

                    place->interval = p_interval[cur_mask][cur_i];
                    place->id = _places[pts[cur_i]]->id;

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

        virtual Result _calcResult() {
            //TODO: impl2
            return _calcResultImpl1();
        }

        virtual Result _calcResultImpl1() {
            Result result;

            std::queue<int>
                    toVisit = _distanceOrderedPlaces(),
                    visited;

            while(!toVisit.empty()) {
                std::vector<int> places;
                int mask = 0;
                int updatedMask = 0;
                auto block = std::make_shared<Block>();

                while(!toVisit.empty()
                    && _count(mask) < _maxBlockSize
                ) {

                    //push to "visited" queue unused places to use them on the next iterations
                    for(size_t i = 0; i < places.size(); ++i) {
                        if (!_bit(mask, i)) {
                            visited.push(places[i]);
                            updatedMask = (updatedMask ^ (1 << i));
                        }
                    }

                    //replace unused places
                    for(size_t i = 0; i < places.size() && !toVisit.empty(); ++i) {
                        if (!_bit(mask, i)) {
                            places[i] = toVisit.front();
                            toVisit.pop();
                            updatedMask = (updatedMask ^ (1 << i));
                        }
                    }

                    //take places from not visited queue to fit the block size
                    while(places.size() < _maxBlockSize && !toVisit.empty()) {
                        places.push_back(toVisit.front());
                        int i = places.size()-1;
                        toVisit.pop();
                        updatedMask = (updatedMask ^ (1 << i));
                    }

                    //apply DP algorithm to find optimal route
                    auto curBlock = _hamilton(places, mask);
                    mask = curBlock->mask;
                    block = curBlock;
                }

                //push to "visited" queue unused places to use them on the next iterations
                for(size_t i = 0; i < places.size(); ++i) {
                    if (!_bit(mask, i)) {
                        visited.push(places[i]);
                        updatedMask = (updatedMask ^ (1 << i));
                    }
                }

                result.blocks.push_back(block);
                result.price += block->price;
                places.clear();

                while(!visited.empty()) {
                    toVisit.push(visited.front());
                    visited.pop();
                }
            }

            return result;
        }

        std::queue<int> _distanceOrderedPlaces() {
            std::vector<std::pair<double, int>> order;
            std::queue<int> result;


            for(size_t i = 0; i < _places.size(); ++i) {
                order.emplace_back(distance(_places[i]->coords, _coordinates), i);
            }

            std::sort(order.begin(), order.end());

            for(size_t i = 0; i < _places.size(); ++i)
                result.push(order[i].second);

            return result;
        }

    };

    ShPtr<PathBuilder> PathBuilder::createImpl1() {
        return std::make_shared<PathBuilderImpl1>();
    }
}