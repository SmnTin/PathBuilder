#ifndef PATHBUILDER_PATHWORKER_H
#define PATHBUILDER_PATHWORKER_H

#include "pbuilder/Definitions.h"
#include "pbuilder/Data/Place.h"
#include "pbuilder/Structures/Mat.h"

#include <vector>

namespace pbuilder {

    class PathWorker {
    public:

        struct PlaceVisited {
            typedef Place::Id Id;
            Id id = 0;
            Interval interval;
        };

        struct Block {
            int price = 0;
            int mask = 0;
            std::vector<ShPtr<PlaceVisited>> order;
        };


        virtual void setPlaces(const std::vector <ShPtr<Place>> &places) {
            _places = places;
        };

        virtual void setDayStart(const TimePoint &dayStart) {
            _dayStart = dayStart;
        }

        virtual void setDayEnd(const TimePoint &dayEnd) {
            _dayEnd = dayEnd;
        };

        virtual void setMatrices(const std::vector <ShPtr<MatInt>> &matrices) {
            _matrices = matrices;
        };

        virtual void setStartingPos(const Coordinates &coordinates) {
            _coordinates = coordinates;
        };

    protected:
        std::vector <ShPtr<Place>> _places;
        TimePoint _dayStart, _dayEnd;
        std::vector <ShPtr<MatInt>> _matrices;
        Coordinates _coordinates;
    };
} //pbuilder


#endif //PATHBUILDER_PATHWORKER_H
