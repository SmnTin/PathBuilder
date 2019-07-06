#include "pbuilder/PathChecker.h"

namespace pbuilder {

    class PathCheckerImpl1 : public PathChecker {
    public:
        Result check() override {
            _prepareInput();

            auto result = _check();

            //adding transport variations
            if(result.possible) {
                if(!_places.empty()) {
                    auto & place = _places[0];
                    auto & placeRes = result.block->order[0];

                    placeRes->transports.assign(place->timesToGet.size(), Transport());
                    placeRes->chosenTransport = place->chosen;

                    for(size_t i = 0; i < place->timesToGet.size(); ++i) {
                        place->chosen = i;
                        placeRes->transports[i].takesMinutes = place->timesToGet[i].getTime();

                        auto curResult = _check();

                        if(curResult.possible)
                            placeRes->transports[i].possible = true;
                    }

                    place->chosen = placeRes->chosenTransport;

                }
                for(size_t i = 0; i + 1 < _places.size(); ++i) {
                    auto & place = result.block->order[i];
                    auto & nextPlace = result.block->order[i+1];

                    nextPlace->transports.assign(numberOfTransports, Transport());
                    nextPlace->chosenTransport = _matrices[numberOfTransports]->at(place->id, nextPlace->id);
                    for(size_t j = 0; j < numberOfTransports; ++j) {
                        nextPlace->transports[j].takesMinutes = _matrices[j]->at(place->id, nextPlace->id);
                        _resultedMat->at(place->id, nextPlace->id) = _matrices[j]->at(place->id, nextPlace->id);

                        auto curResult = _check();

                        if(curResult.possible)
                            nextPlace->transports[j].possible = true;
                    }

                }
            }

            return result;
        }

    protected:

        ShPtr<MatInt> _resultedMat;

        virtual void _prepareInput() {
            size_t n = _matrices[0]->cols();
            _resultedMat = std::make_shared<MatInt>(n, n, INF);

            for(size_t i = 0; i < n; ++i) {
                for(size_t j = 0; j < n; ++j) {
                    //we take corresponding value based on chosen transport (foot-walking, public-transport or car)
                    _resultedMat->at(i, j) = _matrices[_matrices[numberOfTransports]->at(i, j)]->at(i, j);
                }
            }
        }

        virtual Result _check() {
            TimePoint currentTime = _dayStart;
            Result result;
            result.block = std::make_shared<Block>();
            result.block->dayOfWeek = _dayOfWeek;
            result.possible = true;

            for(size_t i = 0; i < _places.size(); ++i) {
                if(i == 0) {
                    currentTime = currentTime + _places[i]->timesToGet[_places[i]->chosen];
                } else {
                    currentTime = currentTime + TimePoint(_resultedMat->at(_places[i-1]->id, _places[i]->id));
                }

                auto interval = _places[i]->nearestTime(currentTime, _dayOfWeek);
                currentTime = interval.starts + interval.lasts;

                if(currentTime.getTimePoint() >= _dayEnd.getTimePoint()) {
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