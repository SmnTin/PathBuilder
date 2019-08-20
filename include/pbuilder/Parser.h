#ifndef PATHBUILDER_PARCER_H
#define PATHBUILDER_PARCER_H

#include "nlohmann/json.hpp"

#include "pbuilder/PathBuilder.h"

namespace pbuilder {
    class Parser {
    public:
        struct Result {
            enum Mode {
                FULL = 0,
                ROUTE = 1
            } mode = Mode::FULL;
            uint dayOfWeek = 0, day = 0;
            Coordinates startingPos;
            TimePoint firstDayStart, dayStart, dayEnd;
            std::vector<ShPtr<MatInt>> matrices;
            std::vector<ShPtr<Place>> places;
        };

        static ShPtr<Parser> create();

        virtual Result parse(const std::string &request) = 0;
    };
}

#endif //PATHBUILDER_PARCER_H
