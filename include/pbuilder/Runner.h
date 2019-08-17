#ifndef PATHBUILDER_RUNNER_H
#define PATHBUILDER_RUNNER_H

#include "pbuilder/Definitions.h"

#include <string>

namespace pbuilder {

    class Runner {
    public:
        static ShPtr <Runner> create();

        virtual std::string run(std::string input) = 0;
    };
} //pbuilder

#endif //PATHBUILDER_RUNNER_H
