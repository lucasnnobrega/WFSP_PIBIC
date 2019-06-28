#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;


typedef struct Files_Content {
  int number_of_symbols;
  int n;
  int *priorities;
} File_content;


int verbose(){
    return 0;
}

int write(char verbose) {
    if(verbose == 'v')
        std::cout << "Criado o arquivo" << std::endl; 
    
    ofstream myfile;
    myfile.open ("./data/example.txt");
    myfile << "Writing this to a file.\n";
    myfile.close();
    return 0;
}

File_content *read_instances(char* relative_file_path) {
  string linhas[5];
  

  File_content *content;
  content = (File_content*)malloc(sizeof(File_content));

  //TODO: colocar o nome das instancias dinamico
  ifstream myfile (relative_file_path);
  if (myfile.is_open())
  {
    int count = 4;
    for (int i = 0; i < count; i++)
    {
      getline(myfile, linhas[i]);
      cout << linhas[i] << '\n';
    }
    /*
    while ( getline (myfile,line) )
    {
      cout << line << '\n';
    }
    */

    cout << endl;    

    /*
    for (int i = 0; i < count; i++)
    {
      cout << "counter: " << i << " line: \"" << linhas[i] << "\"" << '\n';
    }
    */

    cout << endl;

    int number_of_symbols = linhas[0][0] - '0';
    int n = linhas[1][0] - '0';

    int* myarray = new int[sizeof(linhas[2])]; 
    int* myarray_clean = new int[sizeof(linhas[2])]; 
    string to_compare = " ";

    for (int i = 0; i < linhas[2].length(); i++) {  
      myarray[i] = linhas[2][i] - '0';
    }
    
    int count_two_dig = 0;
    
    for (int i = 0; i < linhas[2].length(); i++)
    {
      cout << "iteração n: " << i << endl;

      cout << myarray[i] << " | " << myarray[i + 1] << endl;


      if( myarray[i+1] >= 0 && myarray[i+1] <= 9 && myarray[i] > 0){
        cout << "Numero Completo " << myarray[i] << myarray[i+1] << endl;
        myarray_clean[i + count_two_dig] = myarray[i]*10+myarray[i+1];
        count_two_dig += -2;
      }
      else if(myarray[i+1] < 0 && myarray[i] > 0){
          cout << "Numero unico: " <<  myarray[i] << '\n';
          myarray_clean[i + count_two_dig] = myarray[i];
          count_two_dig += -1;
      }
      else cout << "" <<  myarray[i] << '\n';

      cout << endl ;
    }
    cout << "end iteration" << endl << endl;
    
    cout << endl;

    for (int i = 0; i < linhas[2].length(); i++)
    {
      cout << "counter: " << i << " number: " << myarray[i] << " new "<< myarray_clean[i] << '\n';
    }

    cout << "\n\n############################\n\nLast For:\n";

    //TODO: Fix the constant here
    for (int i = 0; i < number_of_symbols; i++)
    {
      cout << "counter: " << i << " number: " << myarray[i] << " new "<< myarray_clean[i] << '\n';
    }

    cout << "\n\n############################\n\nOthers Values:\n";
    cout << "Number of symbols: " << number_of_symbols << endl;
    cout << "Number n: " << n << endl;

    
    content->number_of_symbols = number_of_symbols;
    content->n = n;
    content->priorities = myarray_clean;
    

    myfile.close();
  }

  else{
    cout << "Unable to open file";
  } 

  return content;
}

int write_res(char verbose, int objective, int number_of_symbols) {

    if(verbose == 'v')
        std::cout << "Criado o arquivo" << std::endl; 
    
    ofstream myfile;
    myfile.open ("./data/res_lnn.txt");

    for (int i = 1; i < number_of_symbols; i++)
    {
        myfile << "Number" << i << "\n";
    }
    myfile << "\n**********************************************\n";
    myfile << "Objective: " << objective  << "\n\n";
    myfile << "Sequence:\n";
    for (int i = 1; i < number_of_symbols; i++)
    {
        myfile << i <<" - ";
    }
    myfile << "\n\n";

    myfile << "Occupied positions: " << 22 << " of " << 12 << "\n";

    for (int i = 1; i < number_of_symbols; i++)
    {
        myfile << "symbol " << i << "--> Priority:  10, Di:  5, Df:  4, Pi: 50, Pf: 40" << "\n";
    }

    myfile << "\n(Available copies, Used copies)\n";
    
    for (int i = 1; i < number_of_symbols; i++)
    {
        myfile << "symbol " << i << " -->  " << 5 << ",  " << 2 << "\n";  
    }
    
    myfile << "\n**********************************************\n";
    myfile.close();
    return 0;
}