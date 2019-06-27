#ifndef PATHBUILDER_PATHCHECKER_H
#define PATHBUILDER_PATHCHECKER_H

class PathChecker : public PathWorker {
public:
    static ShPtr<PathBuilder> createImpl1();
    static ShPtr<PathBuilder> createImpl2();

    virtual void check() = 0;

private:
    PathChecker () {};
    ~PathChecker () {};
};


#endif //PATHBUILDER_PATHCHECKER_H
