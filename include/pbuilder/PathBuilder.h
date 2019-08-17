#ifndef PATHBUILDER_PATHBUILDER_H
#define PATHBUILDER_PATHBUILDER_H

#include "pbuilder/PathWorker.h"
#include "pbuilder/Structures/Mat.h"
#include "pbuilder/Data/Place.h"

namespace pbuilder {

    class PathBuilder : public PathWorker {
    public:
        struct Result {
            std::vector<ShPtr<Block>> blocks;
            std::vector<Place::Id> unvisited;
            int price = 0;
        };

        static ShPtr<PathBuilder> createImpl1();
//        static ShPtr<PathBuilder> createImpl2();

        virtual void setMaxBlockSize(size_t maxBlockSize) = 0;

        virtual Result build() = 0;

    protected:
        PathBuilder() = default;

        ~PathBuilder() = default;
    };
} //pbuilder

#endif //PATHBUILDER_PATHBUILDER_H
