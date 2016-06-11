/* FILE NAME: ANIM.H
 * PROGRAMMER: VG4
 * DATE: 11.06.2016
 * PURPOSE: Animation system definitions.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

/* Maximum unit count */
#define VG4_MAX_UNITS 10000

/* Forward reference to unit type */
typedef struct tagvg4UNIT vg4UNIT;

/* Animation context representation type */
typedef struct
{
  HWND hWnd;                     /* Frame window */
  HDC hDC;                       /* Drawing context */
  INT W, H;                      /* Frame size */
  HBITMAP hFrame;                /* Frame image */
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
 * ARGUMENTS:
 *   - device context to be copy to:
 *       HDC hDC;
 * RETURNS: None.
 */
VOID VG4_AnimCopyFrame( HDC hDC );

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

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
