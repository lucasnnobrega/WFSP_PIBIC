#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <string.h>

#include <ilcplex/ilocplex.h>

#include "../include/read_write.h"

using namespace std;

// Driver function to sort the 2D vector
// on basis of a particular column
bool sortByH(const vector<int> &v1,
             const vector<int> &v2)
{
  return v1[2] < v2[2];
}

void draftToLogger(IloModel modelo,
                   IloArray<IloArray<IloBoolVarArray>> y,
                   IloCplex cplex,
                   int number_of_symbols,
                   int n,
                   int TMAX,
                   int *M,
                   int *priorities,
                   IloIntVarArray D,
                   char verbose)
{
  IloEnv env = modelo.getEnv();

  vector<vector<int>> sequence;
  int a = 0;

  for (int i = 0; i < n; i++)
  { // 2 == n
    for (int k = 0; k < M[i]; k++)
    {
      for (int h = 0; h < TMAX - k; h++)
      {
        //cplex.out() << " OUT = " << cplex.getValue(y[i][k][h]) << endl;
        // If the value is equal to 1, save in the sequence vector
        //if (verbose == 'v')
        //  cplex.out() << cplex.getValue(y[i][k][h]);
        if (cplex.getValue(y[i][k][h]) == 1)
        //if (a < 4)
        {
          vector<int> aux;
          aux.push_back(i);
          aux.push_back(k);
          aux.push_back(h);

          sequence.push_back(aux);
          a++;
        }
      }
    }
  }

  if (verbose == 'v')
    cout << endl
         << endl
         << endl;

  sort(sequence.begin(), sequence.end(), sortByH);

  for (int i = 0; i < sequence.size(); i++)
  {
    for (int j = 0; j < sequence[i].size(); j++)
    {
      std::cout << sequence[i][j] << " ";
    }
    std::cout << "\n";
    std::cout << "símbolo: " << sequence[i][0] << std::endl;
    std::cout << "k-ésima cópia: " << sequence[i][1] << std::endl;
    std::cout << "posição h: " << sequence[i][2] << std::endl;
    std::cout << "\n";
  }

  Write_content content;

  content.objective = cplex.getObjValue();

  content.number_of_symbols = number_of_symbols; // OK

  int aux = 0;
  for (auto i = sequence.begin(); i < sequence.end(); i++)
  {
    aux++;
  }

  content.occupied_positions = aux; //OK sequence.size()

  content.total_positions = TMAX; // OK

  content.avaliable_copies = M; // OK Max number of copies of symbol x_i € X

  content.sequence = sequence;

  /***************************************************************************/
  // Array that storage the number of used copies of a symbol
  int used_copies[n];

  // First create a empty vector with only zeros
  for (int i = 0; i < n; i++)
  {
    used_copies[i] = 0;
  }

  // Add one to the usage value
  for (int i = 0; i < sequence.size(); i++)
  {
    // i = [i, k, h]
    // i = [x_i, k-th copy, h position]
    used_copies[sequence[i][0]]++;
  }
  content.used_copies = used_copies;
  /***************************************************************************/

  content.priorities = priorities; // OK

  /***************************************************************************/
  // Distância Inicial
  int Di[n];
  for (int i = 0; i < n; i++)
  {
    Di[i] = 0;
  }
  content.Di = Di; // Still a doubt
  /***************************************************************************/
  // Df == D_i ?
  // Maior distancia entre 2 copias consecutivas
  int Df[n];
  for (auto i = 0; i < n; i++)
  {
    Df[i] = cplex.getValue(D[i]);
  }
  content.Df = Df;
  /***************************************************************************/
  int Pi[n];
  for (int i = 0; i < n; i++)
  {
    Pi[n] = priorities[i] * Di[i];
  }

  content.Pi = Pi;
  /***************************************************************************/
  int Pf[n];
  for (int i = 0; i < n; i++)
  {
    Pf[n] = priorities[i] * Df[i];
  }

  content.Pf = Pf;

  write_res(verbose, content);
}

int write(char verbose)
{
  if (verbose == 'v')
    std::cout << "Criado o arquivo" << std::endl;

  ofstream myfile;
  myfile.open("./data/example.txt");
  myfile << "Writing this to a file.\n";
  myfile.close();
  return 0;
}

File_content *read_instances(const char *relative_file_path, char verbose)
{
  string linhas[5];

  File_content *content;
  content = (File_content *)malloc(sizeof(File_content));

  //TODO: colocar o nome das instancias dinamico
  ifstream myfile(relative_file_path);
  if (myfile.is_open())
  {
    int count = 4;
    for (int i = 0; i < count; i++)
    {
      getline(myfile, linhas[i]);
      if (verbose == 'v')
        cout << "Linha: " << linhas[i] << '\n';
    }
    if (verbose == 'v')
      cout << endl
           << endl;

    int number_of_symbols = linhas[0][0] - '0';
    int m = linhas[1][0] - '0';

    int *myarray = new int[sizeof(linhas[2])];
    int *myarray_clean = new int[sizeof(linhas[2])];
    string to_compare = " ";

    for (int i = 0; i < linhas[2].length(); i++)
    {
      myarray[i] = linhas[2][i] - '0';
    }

    int count_two_dig = 0;

    for (int i = 0; i < linhas[2].length(); i++)
    {
      if (verbose == 'v')
      {
        cout << "iteração n: " << i << endl;
        cout << myarray[i] << " | " << myarray[i + 1] << endl;
      }

      if (myarray[i + 1] >= 0 && myarray[i + 1] <= 9 && myarray[i] > 0)
      {
        if (verbose == 'v')
          cout << "Numero Completo " << myarray[i] << myarray[i + 1] << endl;
        myarray_clean[i + count_two_dig] = myarray[i] * 10 + myarray[i + 1];
        count_two_dig += -2;
      }
      else if (myarray[i + 1] < 0 && myarray[i] > 0)
      {
        if (verbose == 'v')
          cout << "Numero unico: " << myarray[i] << '\n';
        myarray_clean[i + count_two_dig] = myarray[i];
        count_two_dig += -1;
      }
      else
      {
        if (verbose == 'v')
          cout << "" << myarray[i] << '\n';
      }

      cout << endl;
    }
    if (verbose == 'v')
      cout << "end iteration" << endl
           << endl
           << endl;

    for (int i = 0; i < linhas[2].length(); i++)
    {
      if (verbose == 'v')
        cout << "counter: " << i << " number: " << myarray[i] << " new " << myarray_clean[i] << '\n';
    }

    cout << "\n\n############################\n\nLast For:\n";

    //TODO: Fix the constant here
    for (int i = 0; i < number_of_symbols; i++)
    {
      cout << "counter: " << i << " number: " << myarray[i] << " new " << myarray_clean[i] << '\n';
    }

    cout << "\n\n############################\n\nOthers Values:\n";
    cout << "Number of symbols: " << number_of_symbols << endl;
    cout << "Number m: " << m << endl;

    content->number_of_symbols = number_of_symbols;
    content->m = m;
    content->priorities = myarray_clean;

    myfile.close();
  }

  else
  {
    cout << "Unable to open file";
    exit(1);
  }

  return content;
}

int write_res(char verbose, Write_content content_to_write)
{

  int objective = content_to_write.objective;
  int number_of_symbols = content_to_write.number_of_symbols;
  int occupied_positions = content_to_write.occupied_positions;
  int total_positions = content_to_write.total_positions;
  vector<vector<int>> sequence = content_to_write.sequence;
  int *avaliable_copies = content_to_write.avaliable_copies;
  int *used_copies = content_to_write.used_copies;
  int *priorities = content_to_write.priorities;
  int *Di = content_to_write.Di;
  int *Df = content_to_write.Df;
  int *Pi = content_to_write.Pi;
  int *Pf = content_to_write.Pf;

  if (verbose == 'v')
  {
    std::cout << "Inside Write function" << endl;
    std::cout << "Lista de variáveis:" << endl;
    std::cout << " objective          " << objective << endl;
    std::cout << " number_of_symbols  " << number_of_symbols << endl;
    std::cout << " occupied_positions " << occupied_positions << endl;
    std::cout << " total_positions    " << total_positions << endl;
    std::cout << " VECTOR: sequence   " << endl;
    std::cout << " ARRAY: avaliable_copies  " << avaliable_copies << endl;
    std::cout << " ARRAY: used_copies       " << used_copies << endl;
    std::cout << " ARRAY: priorities        " << priorities << endl;
    std::cout << " ARRAY: Di                " << Di << endl;
    std::cout << " ARRAY: Df                " << Df << endl;
    std::cout << " ARRAY: Pi                " << Pi << endl;
    std::cout << " ARRAY: Pf                " << Pf << endl;
  }

  ofstream myfile;
  myfile.open("./data/res_lnn.txt");

  for (int i = 1; i < number_of_symbols; i++)
  {
    myfile << "Number" << i << "\n";
  }
  myfile << "\n**********************************************\n";
  myfile << "Objective: " << objective << "\n\n";

  // ITS WRONG!!!
  myfile << "Sequence:\n";
  for (int i = 0; i < sequence.size(); i++)
  {
    myfile << sequence[i][0] + 1 << " - ";
    cout << sequence[i][0] + 1 << "-";
  }
  myfile << "\n\n";

  myfile << "Occupied positions: " << occupied_positions << " of " << total_positions << "\n\n";

  for (int i = 0; i < number_of_symbols; i++)
  {
    myfile << "symbol " << i + 1 << "--> Priority:  " << priorities[i]
           << ", Di:  " << Di[i] << ", Df:  " << Df[i] << ", Pi: " << Pi[i] << ", Pf: " << Pf[i] << "\n";
  }

  myfile << "\n(Available copies, Used copies)\n";

  for (int i = 0; i < number_of_symbols; i++)
  {
    myfile << "symbol " << i + 1 << " -->  " << avaliable_copies[i] << ",  " << used_copies[i] << "\n";
  }

  myfile << "\n**********************************************\n";
  myfile.close();
  return 0;
}