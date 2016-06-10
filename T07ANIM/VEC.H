/* FILE NAME: VEC.H
 * PROGRAMMER: VG4
 * DATE: 08.06.2016
 * PURPOSE: Space vetor handle library.
 */

#ifndef __VEC_H_
#define __VEC_H_

#include <stdlib.h>
#include <math.h>

#include <windows.h>

/* Base float point type */
typedef double DBL;

/* Angle transform constants */
#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))
#define R2D(A) ((A) * (180.0 / PI))

/* Multiplier for degree to radian conversions */
#define DEGREE2RADIANL 0.01745329251994329576L
extern DBL MultiplierDegree2Radian;

/* Space vector representation type */
typedef struct tagVEC
{
  DBL X, Y, Z; /* Vector components */
} VEC;

/* Space 4 components vector representation type */
typedef struct tagVEC4
{
  VEC V; /* Vector part */
  DBL W; /* 'w' homogenius components */
} VEC4;

/* Transformation matrix representation type */
typedef struct tagMATR
{
  DBL A[4][4]; /* Matrix elements */
} MATR;

/* Color representation type */
typedef struct tagCOLOR
{
  DBL R, G, B, A; /* color components (+ alpha channel [0..1]) */
} COLOR;

/* Color (3 components) representation type */
typedef struct tagCOLOR3
{
  DBL R, G, B; /* color components [0..1] */
} COLOR3;

/* Random number obtain in range [0.0 .. 1.0] function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (DBL) result random number.
 */
__inline DBL Rnd0( VOID )
{
  return (DBL)rand() / RAND_MAX;
} /* End of 'Rnd0' function */

/* Random number obtain in range [-1.0 .. 1.0] function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (DBL) result random number.
 */
__inline DBL Rnd1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
} /* End of 'Rnd1' function */

/***
 * Color handle functions
 ***/

/* Construct color function.
 * ARGUMENTS:
 *   - color components:
 *       DBL R, G, B, A;
 * RETURNS:
 *   (COLOR) constructed structure.
 */
__inline COLOR ColorSet( DBL R, DBL G, DBL B, DBL A )
{
  COLOR c = {R, G, B, A};

  return c;
} /* End of 'ColorSet' function */

/* Construct 3 components color function.
 * ARGUMENTS:
 *   - color components:
 *       DBL R, G, B;
 * RETURNS:
 *   (COLOR) constructed structure.
 */
__inline COLOR3 Color3Set( DBL R, DBL G, DBL B )
{
  COLOR3 c = {R, G, B};

  return c;
} /* End of 'ColorSet' function */

/* Convert color RGB to monochrome function.
 * ARGUMENTS:
 *   - source color:
 *       COLOR C;
 * RETURNS:
 *   (DBL) monochrome value.
 */
__inline DBL ColorMono( COLOR C )
{
  return C.R * 0.30 + C.G * 0.59 + C.B * 0.11;
} /* End of 'ColorMono' function */

/***
 * Vector handle functions
 ***/

/* Construct vector function.
 * ARGUMENTS:
 *   - vertor coordinates:
 *       DBL X, Y, Z;
 * RETURNS:
 *   (VEC) constructed structure.
 */
__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */

/* Construct 4 components vector function.
 * ARGUMENTS:
 *   - vertor Cartesian coordinates:
 *       VEC V;
 *   - vertor 'w' coordinate:
 *       DBL W;
 * RETURNS:
 *   (VEC4) constructed structure.
 */
__inline VEC4 Vec4Set( VEC V, DBL W )
{
  VEC4 v;

  v.V = V;
  v.W = W;
  return v;
} /* End of 'Vec4Set' function */

/* Add two vector function.
 * ARGUMENTS:
 *   - vetors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

/* Subtract two vectors function.
 * ARGUMENTS:
 *   - vectors to be subtracted:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
} /* End of 'VecSubVec' function */

/* Multiply vector by number function.
 * ARGUMENTS:
 *   - vector to be multiply:
 *       VEC V;
 *   - number to be multiply:
 *       DBL N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulNum( VEC V, DBL N )
{
  return VecSet(V.X * N, V.Y * N, V.Z * N);
} /* End of 'VecMulNum' function */

/* Divide vector by number function.
 * ARGUMENTS:
 *   - vector to be divided:
 *       VEC V;
 *   - number to be divide by:
 *       DBL N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecDivNum( VEC V1, DBL N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
} /* End of 'VecDivNum' function */

/* Negate vector function.
 * ARGUMENTS:
 *   - vector to be negate:
 *       VEC V;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecNeg' function */

/* Dot product of two vectors function.
 * ARGUMENTS:
 *   - producted vectors:
 *       VEC V1, V2;
 * RETURNS:
 *   (DBL) result number.
 */
__inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* End of 'VecDotVec' function */

/* Cross product of two vectors function.
 * ARGUMENTS:
 *   - producted vectors:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,
                V1.Z * V2.X - V1.X * V2.Z,
                V1.X * V2.Y - V1.Y * V2.X);
} /* End of 'VecCrossVec' function */

/* Squared length vector obtain functiopn.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 * RETURNS:
 *   (DBL) result number.
 */
__inline DBL VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
} /* End of 'VecLen2' function */

/* Length vector obtain functiopn.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 * RETURNS:
 *   (DBL) result number.
 */
__inline DBL VecLen( VEC V )
{
  DBL len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;

  if (len != 0 && len != 1)
    return sqrt(len);
  return len;
} /* End of 'VecLen' function */

/* Normalize vector function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 * RETURNS:
 *   (VEC) result normalized vector.
 */
__inline VEC VecNormalize( VEC V )
{
  DBL len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;

  if (len != 0 && len != 1)
    return len = sqrt(len), VecSet(V.X / len, V.Y / len,  V.Z / len);
  return V;
} /* End of 'VecNormalize' function */

/* Vector by matrix multiplication (with homogenious devide) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr( VEC V, MATR M )
{
  DBL w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VecMulMatr' function */

/* Vector by matrix multiplication (only orientation) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr3( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
} /* End of 'VecMulMatr3' function */

/* Vector by matrix multiplication (without projection) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr43( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
} /* End of 'VecMulMatr43' function */

/***
 * Matrix handle functions
 ***/

/* Unit (identity) matrix */
#define VG4_UNIT_MATR \
{                 \
  {               \
    {1, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {0, 0, 1, 0}, \
    {0, 0, 0, 1}  \
  }               \
}

/* Identity (unit) matrix setup function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrIdentity( VOID )
{
  MATR m = VG4_UNIT_MATR;

  return m;
} /* End of 'MatrIdentity' function */

/* Translation transformation matrix setup function.
 * ARGUMENTS:
 *   - shift values by axes (vector):
 *       VEC Dv;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrTranslate( VEC Dv )
{
  MATR m =
  {
    {
      {   1,    0,    0,  0},
      {   0,    1,    0,  0},
      {   0,    0,    1,  0},
      {Dv.X, Dv.Y, Dv.Z,  1}
    }
  };

  return m;
} /* End of 'MatrTranslate' function */

/* Scaling transformation matrix setup function.
 * ARGUMENTS:
 *   - scale values by axes (vetor):
 *       VEC Sv;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrScale( VEC Sv )
{
  MATR m =
  {
    {
      {Sv.X,    0,    0,  0},
      {   0, Sv.Y,    0,  0},
      {   0,    0, Sv.Z,  0},
      {   0,    0,    0,  1}
    }
  };

  return m;
} /* End of 'MatrScale' function */

/* Rotation around X axis transformation matrix setup function.
 * ARGUMENTS:
 *   - rotation ange in degrees:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateX( DBL AngleInDegree )
{
  DBL sine, cosine;
  MATR m = VG4_UNIT_MATR;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  m.A[1][1] = cosine;
  m.A[2][2] = cosine;
  m.A[1][2] = sine;
  m.A[2][1] = -sine;
  return m;
} /* End of 'MatrRotateX' function */

/* Rotation around Y axis transformation matrix setup function.
 * ARGUMENTS:
 *   - rotation ange in degrees:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateY( DBL AngleInDegree )
{
  DBL sine, cosine;
  MATR m = VG4_UNIT_MATR;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  m.A[0][0] = cosine;
  m.A[2][2] = cosine;
  m.A[2][0] = sine;
  m.A[0][2] = -sine;
  return m;
} /* End of 'MatrRotateY' function */

/* Rotation around Z axis transformation matrix setup function.
 * ARGUMENTS:
 *   - rotation ange in degrees:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  DBL sine, cosine;
  MATR m = VG4_UNIT_MATR;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  m.A[0][0] = cosine;
  m.A[1][1] = cosine;
  m.A[0][1] = sine;
  m.A[1][0] = -sine;
  return m;
} /* End of 'MatrRotateZ' function */

/* Rotation around arbitrary radius-vector transformation matrix setup function.
 * ARGUMENTS:
 *   - rotation ange in degrees:
 *       DBL AngleInDegree;
 *   - vector rotate around:
 *       VEC R;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotate( DBL AngleInDegree, VEC R )
{
  DBL sine, cosine;
  MATR m;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  R = VecNormalize(R);
  m.A[0][0] = cosine + R.X * R.X * (1 - cosine);
  m.A[0][1] = R.X * R.Y * (1 - cosine) - R.Z * sine;
  m.A[0][2] = R.X * R.Z * (1 - cosine) + R.Y * sine;
  m.A[0][3] = 0;
  m.A[1][0] = R.Y * R.X * (1 - cosine) + R.Z * sine;
  m.A[1][1] = cosine + R.Y * R.Y * (1 - cosine);
  m.A[1][2] = R.Y * R.Z * (1 - cosine) - R.X * sine;
  m.A[1][3] = 0;
  m.A[2][0] = R.Z * R.X * (1 - cosine) - R.Y * sine;
  m.A[2][1] = R.Z * R.Y * (1 - cosine) + R.X * sine;
  m.A[2][2] = cosine + R.Z * R.Z * (1 - cosine);
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
} /* End of 'MatrRotate' function */
 
/* Multiplication of two matrix function.
 * ARGUMENTS:
 *   - matrix to be multiply:
 *       MATR M1, M2;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  /* INT i, j, k; */
  MATR r;

  /* Cycle version
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.A[i][j] = 0, k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  */

  r.A[0][0] = M1.A[0][0] * M2.A[0][0] + M1.A[0][1] * M2.A[1][0] + M1.A[0][2] * M2.A[2][0] + M1.A[0][3] * M2.A[3][0];
  r.A[0][1] = M1.A[0][0] * M2.A[0][1] + M1.A[0][1] * M2.A[1][1] + M1.A[0][2] * M2.A[2][1] + M1.A[0][3] * M2.A[3][1];
  r.A[0][2] = M1.A[0][0] * M2.A[0][2] + M1.A[0][1] * M2.A[1][2] + M1.A[0][2] * M2.A[2][2] + M1.A[0][3] * M2.A[3][2];
  r.A[0][3] = M1.A[0][0] * M2.A[0][3] + M1.A[0][1] * M2.A[1][3] + M1.A[0][2] * M2.A[2][3] + M1.A[0][3] * M2.A[3][3];

  r.A[1][0] = M1.A[1][0] * M2.A[0][0] + M1.A[1][1] * M2.A[1][0] + M1.A[1][2] * M2.A[2][0] + M1.A[1][3] * M2.A[3][0];
  r.A[1][1] = M1.A[1][0] * M2.A[0][1] + M1.A[1][1] * M2.A[1][1] + M1.A[1][2] * M2.A[2][1] + M1.A[1][3] * M2.A[3][1];
  r.A[1][2] = M1.A[1][0] * M2.A[0][2] + M1.A[1][1] * M2.A[1][2] + M1.A[1][2] * M2.A[2][2] + M1.A[1][3] * M2.A[3][2];
  r.A[1][3] = M1.A[1][0] * M2.A[0][3] + M1.A[1][1] * M2.A[1][3] + M1.A[1][2] * M2.A[2][3] + M1.A[1][3] * M2.A[3][3];

  r.A[2][0] = M1.A[2][0] * M2.A[0][0] + M1.A[2][1] * M2.A[1][0] + M1.A[2][2] * M2.A[2][0] + M1.A[2][3] * M2.A[3][0];
  r.A[2][1] = M1.A[2][0] * M2.A[0][1] + M1.A[2][1] * M2.A[1][1] + M1.A[2][2] * M2.A[2][1] + M1.A[2][3] * M2.A[3][1];
  r.A[2][2] = M1.A[2][0] * M2.A[0][2] + M1.A[2][1] * M2.A[1][2] + M1.A[2][2] * M2.A[2][2] + M1.A[2][3] * M2.A[3][2];
  r.A[2][3] = M1.A[2][0] * M2.A[0][3] + M1.A[2][1] * M2.A[1][3] + M1.A[2][2] * M2.A[2][3] + M1.A[2][3] * M2.A[3][3];

  r.A[3][0] = M1.A[3][0] * M2.A[0][0] + M1.A[3][1] * M2.A[1][0] + M1.A[3][2] * M2.A[2][0] + M1.A[3][3] * M2.A[3][0];
  r.A[3][1] = M1.A[3][0] * M2.A[0][1] + M1.A[3][1] * M2.A[1][1] + M1.A[3][2] * M2.A[2][1] + M1.A[3][3] * M2.A[3][1];
  r.A[3][2] = M1.A[3][0] * M2.A[0][2] + M1.A[3][1] * M2.A[1][2] + M1.A[3][2] * M2.A[2][2] + M1.A[3][3] * M2.A[3][2];
  r.A[3][3] = M1.A[3][0] * M2.A[0][3] + M1.A[3][1] * M2.A[1][3] + M1.A[3][2] * M2.A[2][3] + M1.A[3][3] * M2.A[3][3];
 
  return r;
} /* End of 'MatrMulMatr' function */
 
/* Matrix transpose function.
 * ARGUMENTS:
 *   - source matrix:
 *       MATR M;
 * RETURNS:
 *   (MATR) result transposed matrix.
 */
__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];
  return r;
} /* End of 'MatrTranspose' function */

/* Matrix 3x3 determinant obtain function.
 * ARGUMENTS:
 *   - matrix elements:
 *       DBL A11, A12, A13,
 *           A21, A22, A23,
 *           A31, A32, A33;
 * RETURNS:
 *   (DBL) determinant value.
 */
__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                            DBL A21, DBL A22, DBL A23,
                            DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 -
         A11 * A23 * A32 -
         A12 * A21 * A33 +
         A12 * A23 * A31 +
         A13 * A21 * A32 - 
         A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */

/* Matrix determinant obtain function.
 * ARGUMENTS:
 *   - source matrix:
 *       MATR M;
 * RETURNS:
 *   (DBL) determinant value.
 */
__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) - 
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm' function */

/* Matrix inverse function.
 * ARGUMENTS:
 *   - source matrix:
 *       MATR M;
 * RETURNS:
 *   (MATR) result inverted matrix.
 */
__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);

  if (det == 0)
  {
    MATR m = VG4_UNIT_MATR;

    return m;
  }

  /* Build adjoint matrix */
  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][3] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3]) / det;
 
  r.A[1][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2]) / det;
  return r;
} /* End of 'MatrInverse' function */

/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {               Right.X,                Up.X,              -Dir.X, 0},
      {               Right.Y,                Up.Y,              -Dir.Y, 0},
      {               Right.Z,                Up.Z,              -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

/* Orthographics projection matrix setup function.
 * ARGUMENTS:
 *   - orthographic box side facets coordinates:
 *       DBL Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrOrtho( DBL Left, DBL Right, DBL Bottom, DBL Top, DBL Near, DBL Far )
{
  MATR m =
  {
    {
      {              2 / (Right - Left),                                0,                            0, 0},
      {                               0,               2 / (Top - Bottom),                            0, 0},
      {                               0,                                0,            -2 / (Far - Near), 0},
      {-(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), 1}
    }
  };

  return m;
} /* End of 'MatrOrtho' function */

/* Perspective (frustum) projection matrix setup function.
 * ARGUMENTS:
 *   - frustum side facets coordinates:
 *       DBL Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrFrustum( DBL Left, DBL Right, DBL Bottom, DBL Top, DBL Near, DBL Far )
{
  MATR m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };

  return m;
} /* End of 'MatrFrustum' function */

#endif /* __VEC_H_ */

/* END OF 'VEC.H' FILE */

