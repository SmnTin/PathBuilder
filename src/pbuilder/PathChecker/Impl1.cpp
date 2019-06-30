#include "pbuilder/PathChecker.h"

namespace pbuilder {

    class PathCheckerImpl1 : public PathChecker {
    public:
        Result check() override {
            _prepareInput();

            return _calcResult();
        }

    protected:

        ShPtr<MatInt> _resultedMat;

        virtual void _prepareInput() {
            size_t n = _places.size();
            _resultedMat = std::make_shared<MatInt>(n, n, INF);

            for(size_t i = 0; i < n; ++i) {
                for(size_t j = 0; j < n; ++j) {
                    //we take corresponding value based on chosen transport (foot-walking or car)
                    if(_matrices[4]->at(i, j) == 0)
                        _resultedMat->at(i, j) = _matrices[0]->at(i, j);
                    else
                        _resultedMat->at(i, j) = _matrices[2]->at(i, j);
                }
            }
        }

        virtual Result _calcResult() {
            TimePoint currentTime = _dayStart;
            Result result;
            result.block = std::make_shared<Block>();
            result.possible = true;

            for(size_t i = 0; i < _places.size(); ++i) {
                if(i == 0) {
                    currentTime = currentTime + _places[i]->timeToGet;
                } else {
                    currentTime = currentTime + TimePoint(_resultedMat->at(_places[i-1]->id, _places[i]->id));
                }

                auto interval = _places[i]->nearestTime(currentTime);
                currentTime = interval.starts + interval.lasts;

                if(currentTime.getTimePoint() >= INF) {
                    result.possible = false;
                    break;
                }

                ShPtr<PlaceVisited> place = std::make_shared<PlaceVisited>();
                place->interval = interval;
                place->id = _places[i]->id;

                result.block->order.push_back(place);
                result.block->price += interval.price;
            }

            return result;
        }
    };

    ShPtr<PathChecker> PathChecker::create() {
        return std::make_shared<PathCheckerImpl1>();
    }
} //pbuilder