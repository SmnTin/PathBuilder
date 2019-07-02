#include "pbuilder/Parser.h"

namespace pbuilder {
    class ParserImpl1 : public Parser {
    public:
        Result parse(const std::string &data) override {
            auto json = nlohmann::json::parse(data);

            Result result;
            result.matrices = _parseMatrices(json);
            result.places = _parsePlaces(json);

            result.dayOfWeek = 0;
            for(int dayOfWeek = 0; dayOfWeek < DAYS_IN_WEEK; ++dayOfWeek) {
                std::string dayOfWeekStr = json["day_of_week"];
                if(dayOfWeekStr == DAYS_OF_WEEK_STR[dayOfWeek]) {
                    result.dayOfWeek = dayOfWeek;
                    break;
                }
            }

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

                Coordinates coords;
                coords.latitude = objJson["coords"]["lat"];
                coords.longitude = objJson["coords"]["long"];

                int id = ind;
                if(json["mode"] == "route")
                    id = objJson["id"];

                auto timeToGet = TimePoint::fromString(objJson["time_to_get"]);

                ShPtr<PlaceWithMixedTimetable> place = std::make_shared<PlaceWithMixedTimetable>(coords, timeToGet, id);

                for(size_t dayOfWeek = 0; dayOfWeek < DAYS_IN_WEEK; dayOfWeek++) {
                    if (objJson["timetable"].count(DAYS_OF_WEEK_STR[dayOfWeek])) {
                        auto &dayJson = objJson["timetable"][DAYS_OF_WEEK_STR[dayOfWeek]];

                        for (nlohmann::json::iterator it2 = dayJson.begin(); it2 != dayJson.end(); ++it2) {
                            auto &timetableEl = *it2;
                            if (timetableEl["type"] == "time_point") {
                                auto starts = TimePoint::fromString(timetableEl["starts"]);
                                auto lasts = TimePoint::fromString(timetableEl["lasts"]);
                                int price = timetableEl["price"];

                                place->addTimetableElement(
                                        std::make_shared<FixedTimetableElement>(starts, lasts, price),
                                        dayOfWeek);
                            } else if (timetableEl["type"] == "interval") {
                                auto starts = TimePoint::fromString(timetableEl["starts"]);
                                auto ends = TimePoint::fromString(timetableEl["ends"]);
                                auto lasts = TimePoint::fromString(timetableEl["lasts"]);
                                int price = timetableEl["price"];

                                place->addTimetableElement(
                                        std::make_shared<FreeTimetableElement>(starts, ends, lasts, price),
                                        dayOfWeek);
                            }

                        }
                    }
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