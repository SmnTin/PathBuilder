#include "pbuilder/Parser.h"

namespace pbuilder {
    class ParserImpl1 : public Parser {
    public:
        Result parse(const std::string &data) override {
            auto json = nlohmann::json::parse(data);

            Result result;
            result.matrices = _parseMatrices(json);
            result.places = _parsePlaces(json);

            if(json["mode"] == "route")
                result.mode = Result::Mode::ROUTE;
            else
                result.mode = Result::Mode::FULL;

            result.startingPos.latitude = json["starting_pos"]["lat"];
            result.startingPos.longitude = json["starting_pos"]["long"];
            result.dayStart = TimePoint::fromString(json["day_start"], "00:00", 1);
            result.dayEnd = TimePoint::fromString(json["day_end"], "00:00", 1);

            return result;
        }

    private:
        std::vector<ShPtr<MatInt>> _parseMatrices(nlohmann::json & json) {
            std::vector<ShPtr<MatInt>> res;
            auto & arr = json["matrices"];
            for(nlohmann::json::iterator it = arr.begin(); it != arr.end(); ++it) {
                std::vector<int> matvec;
                for(auto &el : (*it).items())
                    matvec.push_back(el.value());

                size_t sqrt;
                for(sqrt = 1; sqrt*sqrt < matvec.size(); ++sqrt);

                res.push_back(std::make_shared<MatInt>(MatInt::createFromVector(sqrt, sqrt, matvec)));
            }

            return res;
        }

        std::vector<ShPtr<Place>> _parsePlaces(nlohmann::json & json) {
            std::vector<ShPtr<Place>> res;
            auto & arr = json["places"];
            int ind = 0;
            for(nlohmann::json::iterator it = arr.begin(); it != arr.end(); ++it) {
                nlohmann::json & objJson = *it;

                ShPtr<Place> place;

                Coordinates coords;
                coords.latitude = objJson["coords"]["lat"];
                coords.longitude = objJson["coords"]["long"];

                int id = ind;
                if(json["mode"] == "route")
                    id = objJson["id"];

                auto timeToGet = TimePoint::fromString(objJson["time_to_get"]);

                if(objJson.count("interval")) {
                    auto starts = TimePoint::fromString(objJson["interval"]["starts"]);
                    auto ends = TimePoint::fromString(objJson["interval"]["ends"]);
                    auto lasts = TimePoint::fromString(objJson["interval"]["lasts"]);
                    int price = objJson["interval"]["price"];

                    place = std::make_shared<PlaceWithFreeTime>(coords, starts, ends, lasts, price, timeToGet, id);
                } else {
                    std::vector<Interval> intervals;
                    for(nlohmann::json::iterator it2 = objJson["time_points"].begin(); it2 != objJson["time_points"].end(); ++it2) {
                        auto starts = TimePoint::fromString((*it2)["starts"]);
                        auto lasts = TimePoint::fromString((*it2)["lasts"]);
                        int price = (*it2)["price"];

                        Interval i;
                        i.starts = starts;
                        i.lasts = lasts;
                        i.price = price;

                        intervals.push_back(i);
                    }

                    place = std::make_shared<PlaceWithTimetable>(coords, intervals, timeToGet, id);
                }
                res.push_back(place);
                ++ind;
            }
            return res;
        }

    };

    ShPtr<Parser> Parser::create() {
        return std::make_shared<ParserImpl1>();
    }
} //pbuilder