#ifndef PATHBUILDER_PATHWORKER_H
#define PATHBUILDER_PATHWORKER_H

#include "pbuilder/Definitions.h"
#include "pbuilder/Data/Place.h"
#include "pbuilder/Structures/Mat.h"

#include <vector>

namespace pbuilder {

    class PathWorker {
    public:

        struct Transport {
            bool possible = false;
            uint takesMinutes = 0, timeLeftMinutes = 0, cost = 0;
        };

        struct PlaceVisited {
            typedef Place::Id Id;
            typedef Place::OriginalId OriginalId;
            Id id = 0;
            OriginalId originalId = "";
            Interval interval;
            std::vector<Transport> transports;
            uint chosenTransport = 0;
        };

        struct Block {
            int price = 0;
            int mask = 0;
            uint dayOfWeek = 0, day = 0;
            std::vector<ShPtr<PlaceVisited>> order;
        };


        virtual void setPlaces(const std::vector<ShPtr<Place>> &places) {
            _places = places;
        };

        virtual void setFirstDayStart(const TimePoint &dayStart) {
            _firstDayStart = dayStart;
        }

        virtual void setDayStart(const TimePoint &dayStart) {
            _dayStart = dayStart;
        }

        virtual void setDayEnd(const TimePoint &dayEnd) {
            _dayEnd = dayEnd;
        };

        virtual void setMatrices(const std::vector<ShPtr<MatInt>> &matrices) {
            _matrices = matrices;
            if (matrices.empty())
                throw std::runtime_error("Matrix of chosen transports isn't provided.");

            numberOfTransports = (size_t) std::max(0, (int) _matrices.size() - 1);
        };

        virtual void setStartingPos(const Coordinates &coordinates) {
            _coordinates = coordinates;
        };

        virtual void setDayOfWeek(uint dayOfWeek) {
            _dayOfWeek = dayOfWeek;
        }

    protected:
        std::vector<ShPtr<Place>> _places;
        TimePoint _firstDayStart, _dayStart, _dayEnd;
        std::vector<ShPtr<MatInt>> _matrices;
        size_t numberOfTransports = 0;
        Coordinates _coordinates;
        uint _dayOfWeek = 0;
    };
} //pbuilder


#endif //PATHBUILDER_PATHWORKER_H
