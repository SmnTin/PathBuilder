#include <pbuilder/OutputGenerator.h>
#include "pbuilder/Runner.h"

#include "pbuilder/PathBuilder.h"
#include "pbuilder/PathChecker.h"
#include "pbuilder/Parser.h"
#include "pbuilder/OutputGenerator.h"

namespace pbuilder {

    class RunnerImpl1 : public Runner {
    public:
        RunnerImpl1() {
            _parser = Parser::create();
        }

        std::string run(std::string input) override {
            auto parsedInput = _parser->parse(input);

            ShPtr<OutputGenerator> gen;

            if (parsedInput.mode == Parser::Result::Mode::FULL) {
                gen = _runFullMode(parsedInput);
            } else {
                gen = _runRouteMode(parsedInput);
            }

            return gen->generate();
        }

    private:
        ShPtr<Parser> _parser;

        ShPtr<OutputGenerator> _runFullMode(Parser::Result &parsedInput) {
            auto pathBuilder = PathBuilder::createImpl1();
            auto pathCompleter = PathChecker::create();

            _setCommonInput(pathBuilder, parsedInput);
            _setCommonInput(pathCompleter, parsedInput);

            auto output = pathBuilder->build();
            for (auto &block : output.blocks) {
                std::vector<ShPtr<Place>> filteredPlaces;
                for (auto &placeVisited : block->order) {
                    filteredPlaces.push_back(parsedInput.places[placeVisited->id]);
                }
                pathCompleter->setPlaces(filteredPlaces);
                pathCompleter->setDayOfWeek(block->dayOfWeek);
                pathCompleter->setDay(block->day);

                block = pathCompleter->check().block;
            }
            return OutputGeneratorFullMode::create(output);
        }

        ShPtr<OutputGenerator> _runRouteMode(Parser::Result &parsedInput) {
            auto pathChecker = PathChecker::create();

            _setCommonInput(pathChecker, parsedInput);
            pathChecker->setDay(parsedInput.day);

            auto output = pathChecker->check();
            return OutputGeneratorRouteMode::create(output);
        }

        void _setCommonInput(const ShPtr<PathWorker> &pathWorker, Parser::Result &parsedInput) {
            pathWorker->setStartingPos(parsedInput.startingPos);
            pathWorker->setDayStart(parsedInput.dayStart);
            pathWorker->setDayEnd(parsedInput.dayEnd);
            pathWorker->setMatrices(parsedInput.matrices);
            pathWorker->setPlaces(parsedInput.places);
            pathWorker->setDayOfWeek(parsedInput.dayOfWeek);
        }
    };

    ShPtr<Runner> Runner::create() {
        return std::make_shared<RunnerImpl1>();
    }
} //pbuilder