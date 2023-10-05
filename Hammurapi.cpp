#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "YearStat.h"

const char* FILE_NAME = "save";
const int YEARS_TOTAL = 10;
const int MIN_ACRE_PRICE = 17;
const int MAX_ACRE_PRICE = 26;
const int FEED_COUNT = 20;
const int WORK_COUNT = 10;
const float ACRE_SEED_COEF = 0.5f;
const int MIN_ACRE_HARVEST = 1;
const int MAX_ACRE_HARVEST = 6;
const float MIN_RATS_COEF = 0.0f;
const float MAX_RATS_COEF = 0.07f;
const float STARVED_LOSE_COEF = 0.45f;
const float PLAGUE_COEF = 0.15f;

int randomInt(int min, int max)
{
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> uniform_dist(min, max);
    int mean = uniform_dist(e);
    return mean;
}

float randomFloat(float min, float max)
{
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_real_distribution<float> uniform_dist(min, max);
    float mean = uniform_dist(e);
    return mean;
}

bool inputYesNo(const char* question)
{
    bool result = false;
    char answer = 'n';

    while (true) {
        std::cout << question;
        std::cin >> answer;

        if (answer == 'y') {
            result = true;
            break;
        }
        else if (answer == 'n') {
            result = false;
            break;
        }
        else {
            std::cout << "�� ���������� ����. ������� \"y\" ��� \"n\"\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
    }

    std::cout << std::endl;
    return result;
}

int inputInteger(const char* question)
{
    int result = 0;
    bool correct = false;

    while (true) {
        result = 0;
        std::cout << question;
        std::cin >> result;

        if (std::cin.fail()) {
            std::cout << "������������ ����. ������� ����� �����.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        break;
    }

    return result;
}

int inputNatural(const char* question)
{
    int result = 0;

    while (true) {
        std::cout << question;
        std::cin >> result;

        if (std::cin.fail()) {
            std::cout << "������������ ����. ������� ����������� �����.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (result < 0) {
            std::cout << "������� ��������������� ��������.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        break;
    }

    return result;
}

void trade(int& acres, int& wheat, const int price) {
    while (true) {
        int trade = inputInteger("������� ����� ����� ����������� ������/�������? ");
        int tradedAcres = trade * price;

        if (acres < trade) {
            std::cout << "�, ����������, ������! � ��� ����� " << acres << " ����� �����!" << std::endl;
            continue;
        }

        if (wheat < tradedAcres) {
            std::cout << "�, ����������, ������! � ��� ����� " << wheat << " ������� �������!" << std::endl;
            continue;
        }

        acres += trade;
        wheat -= tradedAcres;

        break;
    }
}

void feed(int& population, int& wheat, int& starved, float& starvedPercent, bool &starvedLose)
{
    while (true) {
        int feed = inputNatural("������� ������� ������� ����������� ������? ");

        if (wheat < feed) {
            std::cout << "�, ����������, ������! � ��� ����� " << wheat << " ������� �������!" << std::endl;
            continue;
        }
        
        int feeded = feed / FEED_COUNT;

        if (population < feeded) {
            std::cout << "�, ����������, �� �������� �����! � ��� ����� " << population << " ���������!" << std::endl;
            continue;
        }

        wheat -= feed;
        starved = population - feeded;
        starvedPercent = starved / static_cast<float>(population);
        
        if (starvedPercent > STARVED_LOSE_COEF) {
            std::cout << "������ " << static_cast<int>(STARVED_LOSE_COEF * 100) << "% ��������� ��������� ������� �� ������. �� ���������!" << std::endl;
            starvedLose = true;
            break;
        }

        population -= starved;
        break;
    }
}

void seed(const int population, const int acres, int& wheat, int& harvestedByAcre, int& harvested)
{
    while (true) {
        int seed = inputNatural("������� ����� ����� ����������� �������? ");

        if (population * WORK_COUNT < seed) {
            std::cout << "�, ����������, ������! ��� �� ������� �������! � ��� ����� " << population << " ���������!" << std::endl;
            continue;
        }

        if (wheat < seed * ACRE_SEED_COEF) {
            std::cout << "�, ����������, ������! ��� �� ������� �������! � ��� ����� " << wheat << " �������!" << std::endl;
            continue;
        }

        wheat -= seed * ACRE_SEED_COEF;
        harvestedByAcre = randomInt(MIN_ACRE_HARVEST, MAX_ACRE_HARVEST);
        harvested = seed * harvestedByAcre;
        wheat += harvested;
        break;
    }
}

void arrive(const int starved, const int harvestedByAcre, const int wheat, int& population, int& arrived)
{
    arrived = starved / 2 + (5 - harvestedByAcre) * wheat / 600 + 1;
    if (arrived < 0) arrived = 0;
    else if (arrived > 50) arrived = 50;
    population += arrived;
}

void plague(int& population, bool& hasPlague)
{
    float random = randomFloat(0.0f, 1.0f);
    hasPlague = random <= PLAGUE_COEF;

    if (hasPlague) {
        population /= 2;
    }
}

void rats(int& rats, int& wheat)
{
    float random = randomFloat(MIN_RATS_COEF, MAX_RATS_COEF);
    rats = wheat * random;
    wheat -= rats;
}

float starvedAverage(const YearStat* const years)
{
    float result = 0.0f;

    for (int i = 0; i < YEARS_TOTAL; i++) {
        result += years[i].starvedPercent;
    }

    result /= YEARS_TOTAL;
    return result;
}

float acreBySettler(const YearStat& year)
{
    return year.acres / static_cast<float>(year.population);
}

void load(std::ifstream& file, YearStat* const years, int& lastSavedYear)
{
    if (!file.is_open()) {
        std::cerr << "Can't load file." << std::endl;
        return;
    }

    for (int i = 0; i < YEARS_TOTAL; i++) {
        file >> years[i];
    }

    file >> lastSavedYear;
}

void save(std::ofstream& file, const YearStat* const years, const int lastSavedYear)
{
    if (!file.is_open()) {
        std::cerr << "Can't save file." << std::endl;
        return;
    }

    for (int i = 0; i < YEARS_TOTAL; i++) {
        file << years[i] << '\n';
    }

    file << lastSavedYear;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    YearStat years[YEARS_TOTAL];
    int lastSavedYear = 1;

    std::ifstream input(FILE_NAME, std::ios::in);

    if (input.is_open()) {
        bool needLoad = inputYesNo("������ ���� ����������. ������ ��������� ���? (y/n) ");

        if (needLoad) {
            load(input, years, lastSavedYear);
            input.close();
            std::cout << "���� ���������!\n\n";
        }
        else {
            // save file with default values
            input.close();
            std::ofstream file(FILE_NAME, std::ios::out);
            file.close();
            std::cout << "������, �������� ����� ����.\n\n";
        }
    }

    years[0].population = 100;
    years[0].acres = 1000;
    years[0].wheat = 2800;

    int yearIndex = lastSavedYear;

    while (yearIndex < YEARS_TOTAL)
    {
        const YearStat previousYear = years[yearIndex - 1];
        YearStat& year = years[yearIndex];

        if (year.acrePrice == 0) {
            year.acrePrice = randomInt(MIN_ACRE_PRICE, MAX_ACRE_PRICE);
        }

        year.population = previousYear.population;
        year.acres = previousYear.acres;
        year.wheat = previousYear.wheat;

        std::cout << "��� ����������, �������� �������� ����\n"
            << "� ���� " << yearIndex << " ������ ����������� ���������\n";

        if (previousYear.starved > 0) {
            std::cout << previousYear.starved << " ������� ������ � ������";

            if (previousYear.arrived > 0) {
                std::cout << ", � " << previousYear.arrived << " ������� ������� � ��� ������� �����;\n";
            }
            else {
                std::cout << ";\n";
            }
        }
        else if (previousYear.arrived > 0) {
            std::cout << previousYear.arrived << " ������� ������� � ��� ������� �����;\n";
        }

        if (previousYear.hasPlague) {
            std::cout << "���� ���������� �������� ���������\n";
        }

        std::cout << "��������� ������ ������ ���������� " << year.population << " �������;\n";

        if (previousYear.rats > 0) {
            std::cout << "����� ��������� " << previousYear.rats << " ������� �������, ������� " << previousYear.wheat << " ������� � �������;\n";
        }

        std::cout << "����� ������ �������� " << year.acres << " �����;\n"
            << "1 ��� ����� ����� ������ " << year.acrePrice << " ������;\n"
            << "� ��� " << year.wheat << " ������� �������.\n\n"
            << "��� ���������, ����������?\n";

        bool needSave = inputYesNo("������� ��������� ���� � ���������� �����? (y/n) ");

        if (needSave) {
            std::ofstream output(FILE_NAME, std::ios::out);
            save(output, years, yearIndex);
            output.close();
            return 0;
        }

        trade(year.acres, year.wheat, year.acrePrice);
        bool starvedLose = false;
        feed(year.population, year.wheat, year.starved, year.starvedPercent, starvedLose);

        if (starvedLose) {
            return 0;
        }

        seed(year.population, year.acres, year.wheat, year.hatvestedByAcre, year.harvested);
        arrive(year.starved, year.hatvestedByAcre, year.wheat, year.population, year.arrived);
        plague(year.population, year.hasPlague);
        rats(year.rats, year.wheat);

        if (year.population <= 0) {
            std::cout << "����������, � ����� ������ ������ �� �������� ���������! �� ���������!" << std::endl;
        }

        std::cout << "\n-------------------\n";
        yearIndex++;
    }

    float P = starvedAverage(years);
    float L = acreBySettler(years[YEARS_TOTAL - 1]);

    if (P > 0.33f && L < 7) {
        std::cout << "��-�� ����� ���������������� � ����������,\n" <<
            "����� ������� ����, � ������ ��� �� ������.\n" <<
            "������ �� ��������� ������� ������ ������������� � ��������\n";
    }
    else if (P > 0.1f && L < 9) {
        std::cout << "�� ������� ��������\n" <<
            "�����, ������� ������ � ����� ��������.����� �������� �\n" <<
            "�����������, � ����� ������ �� ������ ������ ��� ����������\n";
    }
    else if (P > 0.03f && L < 10) {
        std::cout << "�� ���������� ������ �������, � ���,\n" <<
            "�������, ���� ���������������, �� ������ ������ �� ������� ��� ��\n" <<
            "����� ������ �����\n";
    }
    else {
        std::cout << "����������! ���� �������,\n" <<
            "�������� � ���������� ������ �� ���������� �� �����\n";
    }

    return 0;
}
