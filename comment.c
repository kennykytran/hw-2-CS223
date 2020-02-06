#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#define MAXIMUM 80

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

void CommentRemove(char *c){
       int i, j;

       for(i = j= 0; c[j] ;){
         if(c[j]=='/' && c[j+1] == '/') {
           for(j= j+2; c[j] && c[j++]!= '\n';);}
         else if (c[j] == '/' && c[j+1] == '*'){
           for(j = j+2; c[j] && c[++j] && (c[j-1]!='*' || c[j]!='/' || !j++); );}
         else {c[i++] = c[j++];}
       }
       c[i]='\0';
}

int main(int argc, const char * argv[]) {

FILE* fin;
FILE* fout;

int index, c;
index = c = 0;
char input[MAXIMUM];

while((c = fgetc(fin)) != EOF && index < MAXIMUM){
  input[index] = fgetc(fin);
  ++index;}

CommentRemove(input);

fputs(input, fout);

closefiles(2, fin, fout);

return 0;
}
