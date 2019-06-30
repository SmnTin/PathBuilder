#include "pbuilder/OutputGenerator.h"

#include "nlohmann/json.hpp"

namespace pbuilder {

    class OutputGeneratorFullModeImpl1 : public OutputGeneratorFullMode {
    public:
        explicit OutputGeneratorFullModeImpl1(const PathBuilder::Result &output) {
            _output = output;
        }

        std::string generate() override {
            nlohmann::json json;

            json["full_price"] = _output.price;

            json["days"] = nlohmann::json::array();

            for(auto & block : _output.blocks) {
                nlohmann::json jday;
                jday["price"] = block->price;
                jday["places"] = nlohmann::json::array();

                for(auto & place : block->order) {
                    nlohmann::json jplace;
                    jplace["id"] = place->id;
                    jplace["starts"] = place->interval.starts.toString();
                    jplace["lasts"] = place->interval.lasts.toString();
                    jplace["price"] = place->interval.price;

                    jday["places"].push_back(jplace);
                }

                json["days"].push_back(jday);
            }

            return json.dump();
        }

    private:
        PathBuilder::Result _output;
    };

    ShPtr<OutputGeneratorFullMode> OutputGeneratorFullMode::create(const PathBuilder::Result &output) {
        return std::make_shared<OutputGeneratorFullModeImpl1>(output);
    }

    //TODO: generate()
    class OutputGeneratorRouteModeImpl1 : public OutputGeneratorRouteMode {
    public:
        explicit OutputGeneratorRouteModeImpl1(const PathChecker::Result &output) {
            _output = output;
        }

        std::string generate() override {
            nlohmann::json json;

            return json.dump();
        }

    private:
        PathChecker::Result _output;
    };

    ShPtr<OutputGeneratorRouteMode> OutputGeneratorRouteMode::create(const PathChecker::Result &output) {
        return std::make_shared<OutputGeneratorRouteModeImpl1>(output);
    }
}