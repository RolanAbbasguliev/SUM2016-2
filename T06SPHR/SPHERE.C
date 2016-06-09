/* FILE NAME: SPHERE.C
 * PROGRAMMER: VG4
 * DATE: 07.06.2016
 * PURPOSE: Sphere drawing project.
 *          Drawing module.
 */

#include <math.h>
#include <time.h>

#include "sphere.h"

/* Sphere grid size */
#define N 100
#define M 200

/* PI constant */
#define PI 3.14159265358979

/* Base float point type */
typedef double DBL;

/* Space vector representation type */
typedef struct
{
  DBL X, Y, Z; /* Vector coordinates */
} VEC;

/* Dot product of two vectors function.
 * ARGUMENTS:
 *   - vectors to be dot producted:
 *       VEC A, B;
 * RETURNS:
 *   (DBL) dot product value.
 */
DBL VecDotVec( VEC A, VEC B )
{
  return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
} /* End of 'VecDotVec' function */

/* Vector set by components function.
 * ARGUMENTS:
 *   - vector components:
 *       DBL A, B, C;
 * RETURNS:
 *   (VEC) constructed vector.
 */
VEC VecSet( DBL A, DBL B, DBL C )
{
  VEC r = {A, B, C};

  return r;
} /* End of 'VecSet' function */

/* Vector normalization function.
 * ARGUMENTS:
 *   - vector to be normalize:
 *       VEC V;
 * RETURNS:
 *   (VEC) normalized vector value.
 */
VEC VecNormalize( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len != 1 && len != 0)
    len = sqrt(len), V.X /= len, V.Y /= len, V.Z /= len;
  return V;
} /* End of 'VecNormalize' function */

/* Rotate vector function.
 * ARGUMENTS:
 *   - vector to be rotated:
 *       VEC P;
 *   - vector rotated around:
 *       VEC A;
 *   - rotation angle in degree:
 *       DBL Angle;
 * RETURNS:
 *   (VEC) rotated vector value.
 */
VEC Rotate( VEC P, VEC A, DBL Angle )
{
  DBL si, co;

  A = VecNormalize(A);

  Angle *= PI / 180;
  si = sin(Angle);
  co = cos(Angle);

  return VecSet(P.X * (co + A.X * A.X * (1 - co)) +
                P.Y * (A.Y * A.X * (1 - co) + A.Z * si) +
                P.Z * (A.Z * A.X * (1 - co) - A.Y * si),
                P.X * (A.X * A.Y * (1 - co) - A.Z * si) +
                P.Y * (co + A.Y * A.Y * (1 - co)) + 
                P.Z * (A.Z * A.Y * (1 - co) + A.X * si),
                P.X * (A.X * A.Z * (1 - co) + A.Y * si) +
                P.Y * (A.Y * A.Z * (1 - co) - A.X * si) + 
                P.Z * (co + A.Z * A.Z * (1 - co)));
} /* End of 'Rotate' function */

/* Rotate vector function.
 * ARGUMENTS:
 *   - vector to be rotated:
 *       VEC V;
 *   - rotation angle in degree:
 *       DBL Angle;
 * RETURNS:
 *   (VEC) rotated vector value.
 */
VEC Rot( VEC V, DBL Angle )
{
  DBL A = Angle * PI / 180;
  VEC r;

  r.X = V.X;
  r.Y = V.Y * cos(A) - V.Z * sin(A);
  r.Z = V.Y * sin(A) + V.Z * cos(A);
  return r;
} /* End of 'Rot' function */

/***
 * Sphere handle functions
 ***/

/* Image representation type */
typedef struct
{
  HBITMAP hBmGlobe;
  DWORD *Bits;
  INT W, H;
} IMG;

IMG Globe;

/* Load sphere texture function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID LoadSphere( VOID )
{
  HDC hDC = GetDC(NULL), hMemDC, hMemDC1;
  HBITMAP hBm;
  BITMAP bm;
  BITMAPINFOHEADER bmih;

  /* Load image from file */
  hMemDC = CreateCompatibleDC(hDC);
  hBm = LoadImage(NULL, "GLOBE.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  GetObject(hBm, sizeof(bm), &bm);
  Globe.W = bm.bmWidth;
  Globe.H = bm.bmHeight;
  SelectObject(hMemDC, hBm);

  /* Fill DIB info */
  memset(&bmih, 0, sizeof(bmih));
  bmih.biSize = sizeof(BITMAPINFOHEADER);
  bmih.biBitCount = 32;                   /* BGRx - DWORD */
  bmih.biPlanes = 1;                      /* always */
  bmih.biCompression = BI_RGB;            /* 0 - no compression */
  bmih.biWidth = bm.bmWidth;              /* width */
  bmih.biHeight = -bm.bmHeight;           /* height + first coded row is 0 */
  bmih.biSizeImage = bm.bmWidth * bm.bmHeight * 4;  /* image size in bytes */

  Globe.hBmGlobe = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS,
                 (VOID **)&Globe.Bits, NULL, 0);
  hMemDC1 = CreateCompatibleDC(hDC);
  SelectObject(hMemDC1, Globe.hBmGlobe);

  BitBlt(hMemDC1, 0, 0, Globe.W, Globe.H, hMemDC, 0, 0, SRCCOPY);

  DeleteDC(hMemDC);
  DeleteDC(hMemDC1);
  DeleteObject(hBm);

  ReleaseDC(NULL, hDC);
} /* End of 'LoadSphere' function */

/* Draw sphere function.
 * ARGUMENTS:
 *   - drawing device context:
 *       HDC hDC;
 *   - sphere center coordinates:
 *       INT Xc, Yc;
 *   - sphere radius:
 *       INT R;
 * RETURNS: None.
 */
VOID DrawSphere( HDC hDC, INT Xc, INT Yc, INT R )
{
  INT i, j, x, y;
  DBL phi, theta, phase = clock() / 1000.0;
  static VEC G[N][M];

  /* Evaluate sphere grid */
  for (i = 0; i < N; i++)
  {
    theta = i * PI / (N - 1);
    for (j = 0; j < M; j++)
    {
      phi = j * 2 * PI / (M - 1) + phase;

      G[i][j].X = R * sin(theta) * cos(phi);
      G[i][j].Y = R * sin(theta) * sin(phi);
      G[i][j].Z = R * cos(theta);

      G[i][j] = Rotate(G[i][j], VecSet(1, 1, 1), 30 * sin(phase));
    }
  }

  /* Draw grid */
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(255, 255, 255));
  SetDCBrushColor(hDC, RGB(255, 255, 255));

  /* Draw sphere by lines */
  /* horizontal */
  /*
  for (i = 0; i < N; i++)
  {
    x = Xc + G[i][0].X;
    y = Yc - G[i][0].Z;
    MoveToEx(hDC, x, y, NULL);
    for (j = 1; j < M; j++)
    {
      x = Xc + G[i][j].X;
      y = Yc - G[i][j].Z;
      LineTo(hDC, x, y);
    }
  }
  */
  /* vertical */
  /*
  for (j = 0; j < M; j++)
  {
    x = Xc + G[0][j].X;
    y = Yc - G[0][j].Z;
    MoveToEx(hDC, x, y, NULL);
    for (i = 1; i < N; i++)
    {
      x = Xc + G[i][j].X;
      y = Yc - G[i][j].Z;
      LineTo(hDC, x, y);
    }
  }
  */
  /* Draw sphere by points */
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < M; j++)
    {
      INT img_x, img_y;
      COLORREF c;
      BYTE r, g, b;

      x = Xc + G[i][j].X;
      y = Yc - G[i][j].Z;
      /* SetPixelV(hDC, x, y, GetPixel(Globe.hDC, j, i)); */
      img_x = j * (Globe.W - 1) / (M - 1);
      img_y = i * (Globe.H - 1) / (N - 1);
      c = Globe.Bits[img_x + img_y * Globe.W]; /* GetPixel(Globe.hDC, img_x, img_y); */
      r = GetRValue(c);
      g = GetGValue(c);
      b = GetBValue(c);
      c = RGB(b, g, r);
      /*
      SetDCPenColor(hDC, c);
      SetDCBrushColor(hDC, c);
      */
      if (G[i][j].Y < 0)
        SetPixelV(hDC, x, y, c);
        /* Ellipse(hDC, x - 8, y - 8, x + 8, y + 8); */
    }
  }
} /* End of 'DrawSphere' function */

/* Draw quadrilateral function.
 * ARGUMENTS:
 *   - drawing device context:
 *       HDC hDC;
 *   - corner points:
 *       POINT P0, P1, P2, P3;
 *   - color:
 *       DWORD Color;
 * RETURNS: None.
 */
VOID DrawQuad( HDC hDC, POINT P0, POINT P1, POINT P2, POINT P3, DWORD Color )
{
  INT s =
    (P0.x - P1.x) * (P0.y + P1.y) +
    (P1.x - P2.x) * (P1.y + P2.y) +
    (P2.x - P3.x) * (P2.y + P3.y) +
    (P3.x - P0.x) * (P3.y + P0.y);

  if (s > 0)
  {
    POINT pts[4];
    
    pts[0] = P0;
    pts[1] = P1;
    pts[2] = P2;
    pts[3] = P3;

    Polygon(hDC, pts, 4);
  }
} /* End of 'DrawQuad' function */

/* Draw sphere by quadrilaterals function.
 * ARGUMENTS:
 *   - drawing device context:
 *       HDC hDC;
 *   - sphere center coordinates:
 *       INT Xc, Yc;
 *   - sphere radius:
 *       INT R;
 * RETURNS: None.
 */
VOID DrawSphereFacets( HDC hDC, INT Xc, INT Yc, INT R )
{
  INT i, j, x, y;
  DBL phi, theta, phase = clock() / 1000.0;
  POINT p0, p1, p2, p3;
  static VEC G[N][M];
  static POINT Ps[N][M];

  /* Evaluate sphere grid */
  for (i = 0; i < N; i++)
  {
    theta = i * PI / (N - 1);
    for (j = 0; j < M; j++)
    {
      phi = j * 2 * PI / (M - 1) + phase;

      G[i][j].X = R * sin(theta) * cos(phi);
      G[i][j].Y = R * sin(theta) * sin(phi);
      G[i][j].Z = R * cos(theta);

      G[i][j] = Rotate(G[i][j], VecSet(1, 1, 1), 30 * sin(phase));
    }
  }

  /* Eval screen projectiosn */
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      Ps[i][j].x = Xc + G[i][j].X;
      Ps[i][j].y = Yc - G[i][j].Z;
    }

  /* Draw grid */
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(255, 0, 255));
  SetDCBrushColor(hDC, RGB(255, 255, 255));

  /* Draw sphere by factes */
  for (i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M - 1; j++)
    {
      INT img_x, img_y;
      COLORREF c;
      BYTE r, g, b;

      img_x = j * (Globe.W - 1) / (M - 1);
      img_y = i * (Globe.H - 1) / (N - 1);
      c = Globe.Bits[img_x + img_y * Globe.W];
      r = GetRValue(c);
      g = GetGValue(c);
      b = GetBValue(c);
      c = RGB(b, g, r);

      SetDCPenColor(hDC, c);
      SetDCBrushColor(hDC, c);
      p0 = Ps[i][j];
      p1 = Ps[i][j + 1];
      p2 = Ps[i + 1][j + 1];
      p3 = Ps[i + 1][j];
      DrawQuad(hDC, p0, p1, p2, p3, 0); /* quadrilateral */
    }
  }
} /* End of 'DrawSphereFacets' function */

/* END OF 'SPHERE.C' FILE */
