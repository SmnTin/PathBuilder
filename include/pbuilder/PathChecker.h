#ifndef PATHBUILDER_PATHCHECKER_H
#define PATHBUILDER_PATHCHECKER_H

#include "pbuilder/PathWorker.h"
#include "pbuilder/Structures/Mat.h"
#include "pbuilder/Data/Place.h"

namespace pbuilder {

    class PathChecker : public PathWorker {
    public:
        static ShPtr<PathBuilder> create();

        virtual void check() = 0;

    protected:
        PathChecker() {};
        ~PathChecker() {};
    };
}


#endif //PATHBUILDER_PATHCHECKER_H
