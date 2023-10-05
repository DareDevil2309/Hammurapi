#pragma once

struct YearStat {
    YearStat() :
        population(0),
        acres(0),
        wheat(0),
        acrePrice(0),
        arrived(0),
        starved(0),
        starvedPercent(0.0f),
        hasPlague(false),
        harvested(0),
        hatvestedByAcre(0),
        rats(0)
    {}

    friend std::istream& operator>> (std::istream& input, YearStat& year) {
        input
            >> year.population
            >> year.acres
            >> year.wheat
            >> year.acrePrice
            >> year.arrived
            >> year.starved
            >> year.starvedPercent
            >> year.hasPlague
            >> year.harvested
            >> year.hatvestedByAcre
            >> year.rats;

        return input;
    }

    friend std::ostream& operator<< (std::ostream& output, const YearStat& year) {
        output
            << year.population << ' '
            << year.acres << ' '
            << year.wheat << ' '
            << year.acrePrice << ' '
            << year.arrived << ' ' 
            << year.starved << ' '
            << year.starvedPercent << ' '
            << year.hasPlague << ' '
            << year.harvested << ' '
            << year.hatvestedByAcre << ' '
            << year.rats;

        return output;
    }

    int population;
    int acres;
    int wheat;
    int acrePrice;
    int arrived;
    int starved;
    float starvedPercent;
    bool hasPlague;
    int hatvestedByAcre;
    int harvested;
    int rats;
};