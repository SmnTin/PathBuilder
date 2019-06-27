#ifndef PATHBUILDER_PATHWORKER_H
#define PATHBUILDER_PATHWORKER_H

class PathWorker {
public:
    virtual void setPlaces(const std::vector<ShPtr<Place>> & places)
            : _places(places):
    virtual void setLimit(const TimePoint & limit)
            : _limit(limit) {};
    virtual void setMatrices(const std::vector<ShPtr<Mat>> & matrices)
            : _matrices(matrices) {};
    virtual void setStartingPos(const Coordinates & coordinates)
            : _coordinates(coordinates) {};

private:
    std::vector<ShPtr<Place>> _places;
    TimePoint _limit;
    std::vector<ShPtr<Mat>> _matrices;
    Coordinates _coordinates;
};

#endif //PATHBUILDER_PATHWORKER_H
