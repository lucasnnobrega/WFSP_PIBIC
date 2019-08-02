#ifndef READ_WRITE
#define READ_WRITE

#include <vector>

#define GREEN "\e[32m"
#define RESET "\e[0m"

using namespace std;

typedef struct Files_Content
{
  int number_of_symbols;
  int m;
  int *priorities;
} File_content;

typedef struct Write_Content //log
{
  int objective;
  int number_of_symbols;
  int occupied_positions;
  int total_positions;
  std::vector<std::vector<int>> sequence;
  int *avaliable_copies;
  int *used_copies;
  int *priorities;
  int *Di;
  int *Df;
  int *Pi;
  int *Pf;
} Write_content;

int write(char verbose);

int write_res(char verbose, Write_content content_to_write);

bool sortByH(const vector<int> &v1, const vector<int> &v2);

void draftToLogger(IloModel modelo,
                   IloArray<IloArray<IloBoolVarArray>> y,
                   IloCplex cplex,
                   int number_of_symbols,
                   int n,
                   int TMAX,
                   int *M,
                   int *priorities,
                   IloIntVarArray D,
                   char verbose);

File_content *read_instances(const char *relative_file_path, char verbose);

#endif // read_write