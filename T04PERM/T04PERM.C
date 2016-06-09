/* File Name: T04PERM
 * Programmer: RA3
 * Date: 04.05.2016
 */                           

#include <stdio.h>
#define N 5

int P[N], Parity = 0;

/* Start of 'SavePerm' function */
void SavePerm( void )
{
  int i;

  FILE *F;
  F = fopen("perm.log", "a");
  if (F == NULL)
    return;
  for (i = 0; i < N; i++)
    fprintf(F, "%2i", P[i]);
  if (Parity)
    fprintf(F, " - odd\n");
  else
    fprintf(F, " - even\n");
  fclose(F);
}
/* End of 'SavePerm' fuction */

/* start of 'Swap' fuction */
void Swap( int *A, int *B)
{
  int tmp = *A;
  *A = *B;
  *B = tmp;
}
/* End of 'Swap' fuction */


/* Start of 'Go' function */  
void Go( int Pos )
{
  int i, x, SaveParity;

  if (Pos == N)
  {
    SavePerm();
    return;
  }
  else 
  {
    /* First occurance */
    Go(Pos + 1);
    SaveParity = Parity;
    for (i = Pos + 1; i < N; i++)
    {
      /* Swap every position */
      Swap(&P[Pos], &P[i]);
      Parity = !Parity;
      Go(Pos + 1);
    }
    /* Restore start order */
    x = P[Pos];
    for (i = Pos + 1; i < N; i++)
      P[i - 1] = P[i];
    P[N - 1] = x;
    Parity = SaveParity;
  }
}
/* End of 'Go' function */

/* Start of 'Main' function */
void main( void )
{
  int i;

  for (i = 0; i < N; i++)
    P[i] = i  + 1;
  Go(0);
}
/*End of 'Main' function */






