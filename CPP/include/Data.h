#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include <stdio.h>

class Data
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

#endif
