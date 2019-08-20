#include "pbuilder/OutputGenerator.h"

#include "nlohmann/json.hpp"

namespace pbuilder {

    nlohmann::json generateBlockJson(const ShPtr<PathWorker::Block> &block) {
        nlohmann::json jday;
        jday["price"] = block->price;
        jday["places"] = nlohmann::json::array();
        jday["day_of_week"] = DAYS_OF_WEEK_STR[block->dayOfWeek];
        jday["day"] = block->day;

        for (auto &place : block->order) {
            nlohmann::json jplace;
            jplace["id"] = place->id;
            jplace["starts"] = place->interval.starts.toString();
            jplace["lasts"] = place->interval.lasts.toString();
            jplace["price"] = place->interval.price;

//            jplace["transports"] = nlohmann::json::object();

            for (size_t i = 0; i < place->transports.size(); ++i) {
                jplace["transports"][std::to_string(i)]["possible"] = place->transports[i].possible;
                jplace["transports"][std::to_string(i)]["takes"] = place->transports[i].takesMinutes;
                jplace["transports"][std::to_string(i)]["time_left"] = place->transports[i].timeLeftMinutes;
                jplace["transports"][std::to_string(i)]["cost"] = place->transports[i].cost;
            }

            jday["places"].push_back(jplace);
        }

        return jday;
    }

    class OutputGeneratorFullModeImpl1 : public OutputGeneratorFullMode {
    public:
        explicit OutputGeneratorFullModeImpl1(const PathBuilder::Result &output) {
            _output = output;
        }

        std::string generate() override {
            nlohmann::json json;

            json["full_price"] = _output.price;

            json["days"] = nlohmann::json::array();

            for (auto &block : _output.blocks) {
                nlohmann::json jday = generateBlockJson(block);

                json["days"].push_back(jday);
            }

            json["unvisited"] = nlohmann::json::array();
            for (auto &unvisitedPlace : _output.unvisited) {
                json["unvisited"].push_back(unvisitedPlace);
            }

            return json.dump(4);
        }

    private:
        PathBuilder::Result _output;
    };

    ShPtr<OutputGeneratorFullMode> OutputGeneratorFullMode::create(const PathBuilder::Result &output) {
        return std::make_shared<OutputGeneratorFullModeImpl1>(output);
    }

    class OutputGeneratorRouteModeImpl1 : public OutputGeneratorRouteMode {
    public:
        explicit OutputGeneratorRouteModeImpl1(const PathChecker::Result &output) {
            _output = output;
        }

        std::string generate() override {
            nlohmann::json json;

            json["possible"] = _output.possible;
            if (_output.possible)
                json["day"] = generateBlockJson(_output.block);

            return json.dump(4);
        }

    private:
        PathChecker::Result _output;
    };

    ShPtr<OutputGeneratorRouteMode> OutputGeneratorRouteMode::create(const PathChecker::Result &output) {
        return std::make_shared<OutputGeneratorRouteModeImpl1>(output);
    }
}