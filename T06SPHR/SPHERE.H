/* FILE NAME: SPHERE.H
 * PROGRAMMER: VG4
 * DATE: 07.06.2016
 * PURPOSE: Sphere drawing project.
 *          Drawing declaration module.
 */

#ifndef __SPHERE_H_
#define __SPHERE_H_

#pragma warning(disable: 4244)

#include <windows.h>

/* Window message handle function (CALLBACK version).
 *   - drawing device context:
 *       HDC hDC;
 *   - sphere center coordinates:
 *       INT X, Y;
 *   - sphere radius:
 *       INT R;
 * RETURNS: None.
 */
VOID DrawSphere( HDC hDC, INT Xc, INT Yc, INT R );

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
VOID DrawSphereFacets( HDC hDC, INT Xc, INT Yc, INT R );

/* Load sphere texture function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID LoadSphere( VOID );

#endif /* __SPHERE_H_ */

/* END OF 'SPHERE.C' FILE */
