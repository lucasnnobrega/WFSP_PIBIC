#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <vector>
#include <stdio.h>

class Logger
{
private:
    int n_of_symbols;
    int m;
    std::vector<int> priorities;

public:
    void readData(char *filePath);

    int getNSymbols();

    int getM();

    int getItemWeight(unsigned int item);
};

#endif //Logger.h
