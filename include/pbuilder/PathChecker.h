#ifndef PATHBUILDER_PATHCHECKER_H
#define PATHBUILDER_PATHCHECKER_H

#include "pbuilder/PathWorker.h"
#include "pbuilder/Structures/Mat.h"
#include "pbuilder/Data/Place.h"

namespace pbuilder {

    class PathChecker : public PathWorker {
    public:
        struct Result {
            bool possible = false;
            ShPtr <Block> block;
        };

        static ShPtr <PathChecker> create();

        virtual void setDay(uint day) = 0;

        virtual Result check() = 0;

    protected:
        PathChecker() {};

        ~PathChecker() {};
    };
}

#endif //PATHBUILDER_PATHCHECKER_H
