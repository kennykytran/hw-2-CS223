#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#define TAB_INTERVAL 8
#define MAX_TAB_INTERVAL 100

bool open_io_files(int argc, const char* argv[], FILE** fin, FILE** fout,
                   int min_expected_argc, int max_expected_argc,
                   const char* usage) {
  /* open an input file, and optionally an output file */
  if (argc < min_expected_argc || argc > max_expected_argc) {
    fprintf(stderr, "%s\n", usage);
    return false;
  }

  *fin = fopen(argv[1], "r");
  if (*fin == NULL) {
    fprintf(stderr, "failed to open input file: '%s'\n", argv[1]);
    return false;
  }
            // In this case, we don't want to open output file
  if (fout == NULL) {  return true; }  // everything cool

  *fout = fopen(argv[2], "w");
  if (*fout == NULL) {  // output file failed to open
    fprintf(stderr, "failed to open output file: '%s'\n", argv[2]);
    fprintf(stderr, "closing already open input file: '%s'\n", argv[1]);
    fclose(*fin);
    return false;
  }

  return true;
}

    // must include <stdarg.h>  -- see files.h
void closefiles(int n, ...) {   // uses varargs (variable # of args)
  va_list pargs;
  va_start (pargs, n);    // initialize the list ptr

  for (int i = 0; i < n; i++)
    fclose(va_arg (pargs, FILE*));  // get next argument

  va_end (pargs);   // clean up
}

int main(int argc, const char * argv[]) {
  FILE* fin;
  FILE* fout;

        // set tab_interval or use default
  int tab_interval = argc == 4 ? atoi(argv[3]) : TAB_INTERVAL;
  if (tab_interval > 80) {
    fprintf(stderr, "tabstops must be <= 80 characters\n");
    exit(2);
  }

       // open input and output files
  if (!open_io_files(argc, argv, &fin, &fout, 3, 4,                   "Usage: ./entab inputfile outputfile tab_interval (optional)\n")) {
    exit(1);
  }

  //process to entab

     int c,s,t;
     int index = 1;
     c = s = t = 0;

     while((c=fgetc(fin))!=EOF){
      if(c==' '){
         while((c=fgetc(fin))!=EOF && c==' ') index++;

         s = index % TAB_INTERVAL;
         t = index / TAB_INTERVAL;

         while(t-- > 0) fputc('\t' , fout);

         while(s-- > 0) fputc(' ' , fout);}

     if (c != ' ') {fputc(c, fout);}

    index = 1;}

  closefiles(2, fin, fout);  // must say number of files

  return 0;
}
