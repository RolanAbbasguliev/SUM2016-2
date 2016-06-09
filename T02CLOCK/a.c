#include <stdio.h>

int main( int argc, char *argv[] )
{
  int x;

  scanf("%d", &x);
  printf("%d", (x % 10) * 10 + x / 10);
  return 0;
}
