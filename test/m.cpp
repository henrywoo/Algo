#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <malloc.h> 
#include <vector>
#include <algorithm>
using namespace std;

int main(int argc, char* argv[]){
  int address;
  char *q;
  char *p = (char *)malloc(sizeof(char)* 1);
  printf("%d\n",strlen("1"));
  q = strcpy(p, "1");
  printf("p:%s(%p),q:%s(%p)\n", p, p, q, q);
  address = (int)p;
  printf("int:%p", address);
  printf("the string is %s\n", (char *)address);

  p = (char *)malloc(sizeof(char)* 300000);
  free(p);
  //p++;
  return 0;
}