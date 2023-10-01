#pragma once
#include<random>

class Random {
public:
    /// @brief Initialize with random seed
    static void init();

    /// @brief Inidialize with specified seed
    /// @param seed seed value
    static void init(int seed);

    /// @brief generate random integer [min, max)
    /// @param min minimum
    /// @param max maximum
    /// @return random interger
    int randomInt(int min, int max);

    /// @brief generate random integer [0, max)
    /// @param max maximum
    /// @return random integer
    int randomInt(int max);

    /// @brief generate random real number [0, 1)
    /// @return random real number
    double randomDouble();
    
    /// @brief generate random real number [min, max)
    /// @param max maximum
    /// @param min maximum
    /// @return random real number
    double randomDouble(double min, double max);
private:
    static std::mt19937 generator;
};