#ifndef read_write
#define read_write

int write(char verbose);
int write_res(char verbose, int objective, int number_of_symbols);
int read_instances(char* relative_file_path);

#endif // read_write