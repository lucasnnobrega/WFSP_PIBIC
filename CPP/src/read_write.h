#ifndef read_write
#define read_write


typedef struct Files_Content {
  int number_of_symbols;
  int m;
  int *priorities;
} File_content;

typedef struct Write_Content {
  int objective;
  int number_of_symbols;
  int occupied_positions;
  int total_positions;
  int* avaliable_copies;
  int* used_copies;
  int* priorities;
  int* Di;
  int* Df;
  int* Pi;
  int* Pf;
} Write_content;

int write(char verbose);

int write_res(char verbose, Write_content content_to_write);

File_content *read_instances(char* relative_file_path);

#endif // read_write