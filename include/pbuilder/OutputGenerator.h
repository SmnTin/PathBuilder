#ifndef PATHBUILDER_OUTPUTGENERATOR_H
#define PATHBUILDER_OUTPUTGENERATOR_H

#include "pbuilder/PathBuilder.h"
#include "pbuilder/PathChecker.h"

namespace pbuilder {

    class OutputGenerator {
    public:
        virtual std::string generate() = 0;
    };

    class OutputGeneratorFullMode : public OutputGenerator {
    public:
        std::string generate() override = 0;

        static ShPtr<OutputGeneratorFullMode> create(const PathBuilder::Result & output);
    };

    class OutputGeneratorRouteMode : public OutputGenerator {
    public:
        std::string generate() override = 0;

        static ShPtr<OutputGeneratorRouteMode> create(const PathChecker::Result & output);
    };
}

#endif //PATHBUILDER_OUTPUTGENERATOR_H
