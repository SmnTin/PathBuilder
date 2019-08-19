#include "pbuilder/Parser.h"

namespace pbuilder {
    class ParserImpl1 : public Parser {
    public:
        Result parse(const std::string &data) override {
            auto json = nlohmann::json::parse(data);

            Result result;
            result.places = _parsePlaces(json["places"], json["mode"]);
            result.matrices = _parseMatrices(json["matrices"]);

            if (result.matrices.empty())
                throw std::runtime_error("No matrices were given");

            for (auto &mat : result.matrices)
                if (mat->rows() != mat->cols() || mat->rows() != result.places.size())
                    throw std::runtime_error("Incorrect matrices were given");

            result.dayOfWeek = _parseDayOfWeek(json["day_of_week"]);

            if (json["mode"] == "route")
                result.mode = Result::Mode::ROUTE;
            else
                result.mode = Result::Mode::FULL;

            result.startingPos.latitude = json["starting_pos"]["lat"];
            result.startingPos.longitude = json["starting_pos"]["long"];
            result.firstDayStart = TimePoint::fromString(json["first_day_start"], "00:00", 1);
            result.dayStart = TimePoint::fromString(json["day_start"], "00:00", 1);
            result.dayEnd = TimePoint::fromString(json["day_end"], "00:00", 1);

            return result;
        }

    private:
        uint _parseDayOfWeek(const std::string &str) {
            for (uint dayOfWeek = 0; dayOfWeek < DAYS_IN_WEEK; ++dayOfWeek) {
                if (str == DAYS_OF_WEEK_STR[dayOfWeek]) {
                    return dayOfWeek;
                }
            }

            throw std::runtime_error("Incorrect day of week occurred during parsing");
        }

        std::vector<ShPtr<MatInt>> _parseMatrices(nlohmann::json &arr) {
            std::vector<ShPtr<MatInt>> res;
            for (auto &objJson : arr) {
                std::vector<int> matvec;
                for (auto &el : objJson.items())
                    matvec.push_back(el.value());

                size_t sqrt;
                for (sqrt = 1; sqrt * sqrt < matvec.size(); ++sqrt);

                res.push_back(std::make_shared<MatInt>(MatInt::createFromVector(sqrt, sqrt, matvec)));
            }

            return res;
        }

        std::vector<ShPtr<Place>> _parsePlaces(nlohmann::json &arr, const std::string &mode) {
            std::vector<ShPtr<Place>> res;
            int ind = 0;
            for (auto &objJson : arr) {
                Coordinates coords;
                coords.latitude = objJson["coords"]["lat"];
                coords.longitude = objJson["coords"]["long"];

                int id = ind;
                if (mode == "route")
                    id = objJson["id"];

                auto timeToGet = TimePoint::fromString(objJson["time_to_get"]);

                ShPtr<PlaceWithMixedTimetable> place = std::make_shared<PlaceWithMixedTimetable>(coords, timeToGet, id);

                for (size_t dayOfWeek = 0; dayOfWeek < DAYS_IN_WEEK; dayOfWeek++) {
                    if (objJson["timetable"].count(DAYS_OF_WEEK_STR[dayOfWeek])) {
                        auto &dayJson = objJson["timetable"][DAYS_OF_WEEK_STR[dayOfWeek]];

                        for (auto &timetableEl : dayJson) {
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