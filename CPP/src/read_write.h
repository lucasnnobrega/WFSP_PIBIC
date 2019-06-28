#ifndef read_write
#define read_write


typedef struct Files_Content {
  int number_of_symbols;
  int m;
  int *priorities;
} File_content;

int write(char verbose);
int write_res(char verbose, int objective, int number_of_symbols);
File_content *read_instances(char* relative_file_path);

#endif // read_write