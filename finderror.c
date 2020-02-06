
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#define MAX 80

int gline(char line[],int lim);

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


void closefiles(int n, ...) {   // uses varargs (variable # of args)
  va_list pargs;
  va_start (pargs, n);    // initialize the list ptr

  for (int i = 0; i < n; i++)
    fclose(va_arg (pargs, FILE*));  // get next argument

  va_end (pargs);   // clean up
}


int main(int argc, const char * argv[]) {

  printf("Press ctrl-d to see result of test.\n");

  FILE* fin;
  FILE* fout;

  int n=0;
  int len=0;
  int squote=1;
  int doubleq=1;
  int braces=0;
  int brackets=0;
  int pthesis=0;
  char line[MAX];

  while ((len = gline(line, MAX)) > 0 ){
      n=0;
      while(n < len) {
	       if( line[n] == '['){
	          braces++;}
	       if( line[n] == ']'){
	          braces--;}
	       if( line[n] == '('){
	          pthesis++;}
	       if( line[n] == ')'){
	          pthesis--;}
	       if( line[n] == '\''){
	          squote *= -1;}
	       if( line[n] == '"'){
	          doubleq *= -1;}
	    n++;}
    }

  if(doubleq !=1)printf("Error: missing double quote mark\n");
  if(squote !=1)printf("Error: missing single quote mark\n");
  if(pthesis !=0)printf("Error: missing parenthesis\n");
  if(braces !=0)printf("Error: missing braces mark\n");
  if(brackets !=0)printf("Error: missing brackets mark\n");
  else printf ("No Syntax Error\n");

  return 0;
}


int gline(char s[],int lim) {
  int i,c;

  for(i=0; i<lim-1 && (c=getchar()) != EOF && c != '\n';++i) s[i] = c;
  if( c == '\n') { s[i] = c;
    ++i;}
  else s[i] = '\0';

  return i;
}
