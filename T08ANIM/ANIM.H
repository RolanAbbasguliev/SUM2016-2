/* FILE NAME: ANIM.H
 * PROGRAMMER: VG4
 * DATE: 18.06.2016
 * PURPOSE: Animation system definitions.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* Maximum unit count */
#define VG4_MAX_UNITS 10000

/* Forward reference to unit type */
typedef struct tagvg4UNIT vg4UNIT;

/* Animation context representation type */
typedef struct
{
  HWND hWnd;                     /* Frame window */
  HDC hDC;                       /* Drawing window context */
  HGLRC hGLRC;                   /* Rendering context */

  INT W, H;                      /* Frame size */
  vg4UNIT *Units[VG4_MAX_UNITS]; /* Animation units array */
  INT NumOfUnits;                /* Number of animation units */

  /* Timer data */
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per seond value */
  BOOL IsPause;                  /* Pause flag */

  /* Input system */
  INT
    Mx, My,                      /* Mouse cursor position */
    Mz,                          /* Mouse wheel absolute value */
    Mdx, Mdy, Mdz;               /* Delta values of mouse axes */
  BYTE
    Keys[256],                   /* Current key states */
    OldKeys[256],                /* Previous frame key states */
    KeysClick[256];              /* Click key flags */
  BYTE
    JBut[32];                    /* Joystick button state */
  INT JPov;                      /* Joystick point-of-view control [0,1..8] */
  DBL
    JX, JY, JZ, JR;              /* Joystick axes */
} vg4ANIM;

/* Animation unit representation type */
struct tagvg4UNIT
{
  /* Unit initialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Init)( vg4UNIT *Uni, vg4ANIM *Ani );

  /* Unit deinitialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Close)( vg4UNIT *Uni, vg4ANIM *Ani );

  /* Unit inter frame events handle function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Response)( vg4UNIT *Uni, vg4ANIM *Ani );

  /* Unit render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Render)( vg4UNIT *Uni, vg4ANIM *Ani );
};

/***
 * Animation system support
 ***/

/* Global mouse wheel delta value */
extern INT VG4_MouseWheel;

/* Global animation context */
extern vg4ANIM VG4_Anim;

/* Animation system initialization function.
 * ARGUMENTS:
 *   - work window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID VG4_AnimInit( HWND hWnd );

/* Animation system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_AnimClose( VOID );

/* Animation system resize window handle function.
 * ARGUMENTS:
 *   - new frame size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID VG4_AnimResize( INT W, INT H );

/* Animation system resize window handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_AnimCopyFrame( VOID );

/* Animation system render frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_AnimRender( VOID );

/* Add new unit to animation system function.
 * ARGUMENTS:
 *   - pointer to new unit:
 *       vg4UNIT *Uni;
 * RETURNS: None.
 */
VOID VG4_AnimAddUnit( vg4UNIT *Uni );

/* End animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_AnimDoExit( VOID );

/* Set/reset full screen mode function.
 * ARGUMENTS: None.
 * RETUNRS: None.
 */
VOID VG4_AnimFlipFullScreen( VOID );

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_AnimUnitCreate( INT Size );

/***
 * Render support
 ***/

/* Vertex representation type */
typedef struct
{
  VEC  P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC  N;  /* Normal at vertex */
  VEC4 C;  /* Vertex color */
} vg4VERTEX;

/* Primitive representation type */
typedef struct
{
  /* OpenGL buffers */
  INT
    VA,       /* Vertex array */
    VBuf,     /* Vertex buffer */
    IBuf;     /* Index buffer */
  INT NumOfI; /* Facets index array size */
  MATR M;     /* Primitive transformation matrix */
  INT MtlNo;  /* Material number */
  INT Id;     /* Primitive Id */
} vg4PRIM;

/* Grid geometry representation type */
typedef struct
{
  INT W, H;     /* Grid size in vertices */
  vg4VERTEX *V; /* Grid points */
} vg4GRID;

/* Object representation type */
typedef struct
{
  vg4PRIM *Prims; /* Primitives array */
  INT NumOfPrims; /* Primitives array size */
  MATR M;         /* Object transformation matrix */
} vg4OBJ;

/* Material representation type */
typedef struct
{
  CHAR Name[300]; /* Material name */
  VEC Ka, Kd, Ks; /* Illumination coefficients */
  FLT Ph, Trans;  /* Shininess and Phong, transparency */
  INT TexW, TexH; /* Textiure image size */
  INT TexNo;      /* For load: bytes per pixel, in use: OpenGL texture no */
} vg4MTL;

/* Global transformation matrices */
extern MATR
  VG4_RndMatrWorld, /* World (object) transformation matrix */
  VG4_RndMatrView,  /* Camera view transform */
  VG4_RndMatrProj;  /* Projection transform */

/* Projection parameters */
extern DBL
  VG4_RndProjDist, /* Near clip plane */
  VG4_RndFarClip,  /* Far clip plane */
  VG4_RndProjSize; /* Project plane size */

/* Load object transform matrix */
extern MATR VG4_RndObjLoadMatrix;

/* Shader support */
extern UINT VG4_RndPrg;

/* Materials array */
#define VG4_MAX_MATERIALS 10000
extern vg4MTL VG4_RndMaterials[VG4_MAX_MATERIALS];
extern INT VG4_RndNumOfMaterials;

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_RndSetProj( VOID );

/* Primitive create function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       vg4PRIM *Pr;
 *   - vertex array:
 *       vg4VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *I;
 *   - INT array size:
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID VG4_RndPrimCreate( vg4PRIM *Pr, vg4VERTEX *V, INT NumOfV, INT *I, INT NumOfI );

/* Primitive draw function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       vg4PRIM *Pr;
 * RETURNS: None.
 */
VOID VG4_RndPrimDraw( vg4PRIM *Pr );

/* Primitive free function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       vg4PRIM *Pr;
 * RETURNS: None.
 */
VOID VG4_RndPrimFree( vg4PRIM *Pr );

/* Object create function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID VG4_RndObjCreate( vg4OBJ *Obj );

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID VG4_RndObjFree( vg4OBJ *Obj );

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID VG4_RndObjDraw( vg4OBJ *Obj );

/* Load object from '*.g3d' file function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL VG4_RndObjLoad( vg4OBJ *Obj, CHAR *FileName );

/* Grid creation function.
 * ARGUMENTS:
 *   - grid pointer:
 *       vg4GRID *G;
 *   - grid size in vertices:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL VG4_GridCreate( vg4GRID *G, INT W, INT H );

/* Grid free function.
 * ARGUMENTS:
 *   - grid pointer:
 *       vg4GRID *G;
 * RETURNS: None.
 */
VOID VG4_GridFree( vg4GRID *G );

/* Grid to primitive convert function.
 * ARGUMENTS:
 *   - grid pointer:
 *       vg4GRID *G;
 *   - primitive pointer:
 *       vg4PRIM *Pr;
 * RETURNS: None.
 */
VOID VG4_Grid2Prim( vg4GRID *G, vg4PRIM *Pr );

/* Grid vertex normals evaluation function.
 * ARGUMENTS:
 *   - grid pointer:
 *       vg4GRID *G;
 * RETURNS: None.
 */
VOID VG4_GridEvalNormals( vg4GRID *G );

/* Create height field primitive from image function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       vg4PRIM *Pr;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL VG4_RndPrimCreateLandscape( vg4PRIM *Pr, CHAR *FileName );

/* Create height field primitive from image function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       vg4PRIM *Pr;
 *   - radius:
 *       FLT R;
 *   - split counts:
 *       INT N, M;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL VG4_RndPrimCreateSphere( vg4PRIM *Pr, FLT R, INT N, INT M );

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT VG4_RndFindMaterial( CHAR *Name );

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID VG4_RndLoadMaterials( CHAR *FileName );

/* Make material from image function.
 * ARGUMENTS:
 *   - image pointer:
 *       vg4IMG *Im;
 *   - material name:
 *       CHAR *MtlName;
 * RETURNS:
 *   (INT) new material number.
 */
INT VG4_RndMtlFromImage( vg4IMG *Im, CHAR *MtlName );

/* Shader program load function.
 * ARGUMENTS:
 *   - shader files prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (UINT) shader program index or 0 if error is occured.
 */
UINT VG4_RndShaderLoad( CHAR *FileNamePrefix );

/* Shader program load function.
 * ARGUMENTS:
 *   - shader program Id:
 *       UINT Prg;
 * RETURNS: None.
 */
VOID VG4_RndShaderFree( UINT Prg );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
