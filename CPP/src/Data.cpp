#include "../include/Data.h"
#include <stdlib.h>

void Data::readData(char *filePath)
{
   FILE *f = fopen(filePath, "r");

   if (!f)
   {
      printf("Problem while reading instance.\n");
      exit(1);
   }

   if (fscanf(f, "%d", &n_of_symbols) != 1)
   {
      printf("Problem while reading instance.\n");
      exit(1);
   }

   if (fscanf(f, "%d", &m) != 1)
   {
      printf("Problem while reading instance.\n");
      exit(1);
   }

   //reading weights
   priorities = std::vector<int>(n_of_symbols, 0);
   for (int i = 0; i < n_of_symbols; i++)
   {
      if (fscanf(f, "%d", &priorities[i]) != 1)
      {
         printf("Problem while reading instance.\n");
         exit(0);
      }
   }
}

int Data::getNSymbols()
{
   return n_of_symbols;
}

int Data::getM()
{
   return m;
}

int Data::getItemWeight(unsigned int item)
{
   if (item >= n_of_symbols)
   {
      return 0;
   }

   return priorities[item];
}
