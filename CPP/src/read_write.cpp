#include <iostream>
#include <fstream>
using namespace std;

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

int read_instances() {
  string line;
  string lines[5];
  
  //TODO: colocar o nome das instancias dinamico
  ifstream myfile ("./data/csp_instances/ins_05_10_1.txt");
  if (myfile.is_open())
  {
    int count = 4;
    for (int i = 0; i < count; i++)
    {
      getline(myfile, lines[i]);
      cout << lines[i] << '\n';
    }
    /*
    while ( getline (myfile,line) )
    {
      cout << line << '\n';
    }
    */
    //TODO: COMO SEPARAR CADA LINHA PARA CADA VARIAVEL
    //size_t lenght = line.copy(line, 100, 0);
    //lines[]

    cout << endl;    

    for (int i = 0; i < count; i++)
    {
      cout << "counter: " << i << " line: \"" << lines[i] << "\"" << '\n';
    }

    cout << endl;

    int* myarray = new int[sizeof(lines[2])]; 
    string to_compare = " ";

    for (int i = 0; i < lines[2].length(); i++)
    {
      //if((lines[2][i] - '0') < 0 || (lines[2][i] - '0') > 9)

      myarray[i] = lines[2][i] - '0';


      cout << lines[2][i] << '\n';
    }
    

    cout << endl;

    for (int i = 0; i < lines[2].length(); i++)
    {
      cout << myarray[i] << '\n';
    }

    myfile.close();
  }

  else cout << "Unable to open file"; 

  return 0;
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