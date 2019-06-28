#ifndef read_write
#define read_write


typedef struct Files_Content {
  int number_of_symbols;
  int m;
  int *priorities;
} File_content;

int write(char verbose);
int write_res(char verbose,
              int objective,
              int number_of_symbols,
              int occupied_positions,
              int total_positions,
              int* avaliable_copies,
              int* used_copies,
              int* priorities,
              int* Di,
              int* Df,
              int* Pi,
              int* Pf)
File_content *read_instances(char* relative_file_path);

#endif // read_write