/* FILE NAME: LOADPRIM.C
 * PROGRAMMER: VG4
 * DATE: 18.06.2016
 * PURPOSE: Render handle functions.
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"

/* Load object transform matrix */
MATR VG4_RndObjLoadMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

/* Load object from '*.g3d' file function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL VG4_RndObjLoad( vg4OBJ *Obj, CHAR *FileName )
{
  INT p, i, NumOfV, NumOfI, NumOfPrimitives;
  DWORD Sign, size;
  FILE *F;
  vg4VERTEX *V;
  INT *I;
  MATR MInv;
  CHAR MtlFile[300], Mtl[300];

  VG4_RndObjCreate(Obj);

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  MInv = MatrTranspose(MatrInverse(VG4_RndObjLoadMatrix));

  /* File structure:
   *   4b Signature: "G3D\0"    CHAR Sign[4];
   *   4b NumOfPrimitives       INT NumOfPrimitives;
   *   300b material file name: CHAR MtlFile[300];
   *   repeated NumOfPrimitives times:
   *     4b INT NumOfV; - vertex count
   *     4b INT NumOfI; - index (triangles * 3) count
   *     300b material name: CHAR Mtl[300];
   *     repeat NumOfV times - vertices:
   *         !!! float point -> FLT
   *       typedef struct
   *       {
   *         VEC  P;  - Vertex position
   *         VEC2 T;  - Vertex texture coordinates
   *         VEC  N;  - Normal at vertex
   *         VEC4 C;  - Vertex color
   *       } VERTEX;
   *     repeat (NumOfF / 3) times - facets (triangles):
   *       INT N0, N1, N2; - for every triangle (N* - vertex number)
   */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }
  fread(&NumOfPrimitives, 4, 1, F);
  fread(MtlFile, 1, 300, F);
  VG4_RndLoadMaterials(MtlFile);

  /* Allocate memory for primitives */
  if ((Obj->Prims = malloc(sizeof(vg4PRIM) * NumOfPrimitives)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Obj->NumOfPrims = NumOfPrimitives;

  for (p = 0; p < NumOfPrimitives; p++)
  {
    /* Read primitive info */
    fread(&NumOfV, 4, 1, F);
    fread(&NumOfI, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    size = sizeof(vg4VERTEX) * NumOfV + sizeof(INT) * NumOfI;
    if ((V = malloc(size)) == NULL)
    {
      while (p-- > 0)
        VG4_RndPrimFree(&Obj->Prims[p]);
      free(Obj->Prims);
      memset(Obj, 0, sizeof(vg4OBJ));
      fclose(F);
      return FALSE;
    }
    memset(V, 0, size);
    I = (INT *)(V + NumOfV);
    /* Read primitive data */
    fread(V, 1, size, F);

    /* Transform vertex */
    for (i = 0; i < NumOfV; i++)
    {
      V[i].P = VecMulMatr43(V[i].P, VG4_RndObjLoadMatrix);
      V[i].N = VecMulMatr43(V[i].N, MInv);
    }

    VG4_RndPrimCreate(&Obj->Prims[p], V, NumOfV, I, NumOfI);
    Obj->Prims[p].MtlNo = VG4_RndFindMaterial(Mtl);
    Obj->Prims[p].Id = p;

    free(V);
  }
  fclose(F);
  return TRUE;
} /* End of 'VG4_RndObjLoad' function */

/* END OF 'LOADPRIM.C' FILE */
