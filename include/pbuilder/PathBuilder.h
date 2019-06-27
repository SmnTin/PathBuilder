#ifndef PATHBUILDER_PATHBUILDER_H
#define PATHBUILDER_PATHBUILDER_H

#include "pbuilder/PathWorker.h"
#include "pbuilder/Structures/Mat.h"
#include "pbuilder/Data/Place.h"

class PathBuilder : public PathWorker {
public:
    static ShPtr<PathBuilder> createImpl1();
    static ShPtr<PathBuilder> createImpl2();

    virtual void build();

private:
    PathBuilder () {};
    ~PathBuilder () {};
};

#endif //PATHBUILDER_PATHBUILDER_H
