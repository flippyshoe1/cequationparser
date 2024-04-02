#include "equation.h"
#include <stdio.h>

int main() {
  printf("enter equation to be solved: ");
  char buff[512];
  scanf("%511s", buff);

  printf("result from equation_solve: %d\n", equation_solve(buff));
  
  return 0;
}
