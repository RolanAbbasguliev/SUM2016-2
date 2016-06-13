/* FILE NAME: UNITS.H
 * PROGRAMMER: VG4
 * DATE: 10.06.2016
 * PURPOSE: Unit samples creation functions.
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "anim.h"

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_UnitCreateBall( VOID );

/* Unit clock creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_UnitCreateClock( VOID );

/* Unit cube creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_UnitCreateCube( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
