/***********************************************************************

  quaternion.cpp

  A quaternion class 

  -------------------------------------------------------------------

  Feb 1998, Paul Rademacher (rademach@cs.unc.edu)
  
************************************************************************/

#include "quaternion.h"
#include <math.h>
#include "stdinc.h"

/******************************************* constructors **************/

quat::quat( void )
{
  *this = quat_identity();
}

quat::quat(const float x, const float y, const float z, const float w)
{
  v.set( x, y, z );
  s = w;
}

quat::quat( vec3 _v, float _s )
{
  set( _v, _s );
}

quat::quat( float _s, vec3 _v )
{
  set( _v, _s );
}

quat::quat( const float *d )
{
  v[0] = d[0];
  v[1] = d[1];
  v[2] = d[2];
  s    = d[3];
}

quat::quat( const double *d )
{
  v[0] = (float)d[0];
  v[1] = (float)d[1];
  v[2] = (float)d[2];
  s    = (float)d[3];
}

quat::quat( const quat &q )
{
  v = q.v;
  s = q.s;
}

void quat::set( vec3 _v, float _s )
{
  v = _v;
  s = _s;
}

quat& quat::operator = (const quat& q)
{ 
  v = q.v;  s = q.s; return *this; 
}


/* ... */


/******** quat friends ************/

quat operator + (const quat &a, const quat &b)
{
  return quat( a.s+b.s, a.v+b.v );
}

quat operator - (const quat &a, const quat &b)
{
  return quat( a.s-b.s, a.v-b.v );
}

quat operator - (const quat &a )
{
  return quat( -a.s, -a.v );
}

quat operator * ( const quat &a, const quat &b)
{
  return quat( a.s*b.s - a.v*b.v, a.s*b.v + b.s*a.v + a.v^b.v );
}

quat operator * ( const quat &a, const float t)
{
  return quat( a.v * t, a.s * t );
}

quat operator * ( const float t, const quat &a )
{
  return quat( a.v * t, a.s * t );
}


mat4  quat::to_mat4( void )
{
  float t, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;
//  vec3  a, c, b, d;
  
  t  = 2.0f / (v*v + s*s);
  
  xs = v[VX]*t;   ys = v[VY]*t;   zs = v[VZ]*t;
  wx = s*xs;      wy = s*ys;      wz = s*zs;
  xx = v[VX]*xs;  xy = v[VX]*ys;  xz = v[VX]*zs;
  yy = v[VY]*ys;  yz = v[VY]*zs;  zz = v[VZ]*zs;
  
  mat4 matrix( 1.0f-(yy+zz), xy+wz,       xz-wy,       0.0f,
	       xy-wz,       1.0f-(xx+zz), yz+wx,       0.0f,
	       xz+wy,       yz-wx,       1.0f-(xx+yy), 0.0f,
	       0.0f,         0.0f,         0.0f,         1.0f );
  
  return matrix;
}


/************************************************* quat_identity() *****/
/* Returns quaternion identity element                                 */

quat quat_identity( void )
{
  return quat( vec3( 0.0, 0.0, 0.0 ), 1.0 );
}


/************************************************ quat_slerp() ********/
/* Quaternion spherical interpolation                                 */

quat quat_slerp( quat from, quat to, float t )
{
  quat to1;
  double omega, cosom, sinom, scale0, scale1;

  /* calculate cosine */
  cosom = from.v * to.v + from.s + to.s;

  /* Adjust signs (if necessary) */
  if ( cosom < 0.0 ) {
    cosom = -cosom;
    to1 = -to;
  }
  else
  {
    to1 = to;
  }

  /* Calculate coefficients */
  if ((1.0 - cosom) > FUDGE ) {
    /* standard case (slerp) */
    omega = acos( cosom );
    sinom = sin( omega );
    scale0 = sin((1.0 - t) * omega) / sinom;
    scale1 = sin(t * omega) / sinom;
  }
  else {
    /* 'from' and 'to' are very close - just do linear interpolation */
    scale0 = 1.0 - t;
    scale1 = t;      
  }

  return ((float)scale0) * from + ((float)scale1) * to1;
}


/********************************************** set_angle() ************/
/* set rot angle (degrees)                                             */

void  quat::set_angle( float f )
{
  vec3 axis = get_axis();

  s = (float)cos( DEG2RAD( f ) / 2.0 );

  v =axis * (float)sin(DEG2RAD(f) / 2.0);
}


/********************************************** scale_angle() ************/
/* scale rot angle (degrees)                                             */

void  quat::scale_angle( float f )
{
  set_angle( f * get_angle() );
}


/********************************************** get_angle() ************/
/* get rot angle (degrees).  Assumes s is between -1 and 1             */

float quat::get_angle( void )
{
  return (float)RAD2DEG( 2.0 * acos( s ) );
}


/********************************************* get_axis() **************/

vec3  quat::get_axis( void )
{
  float scale;

  scale = (float)sin( acos( s ) );
  if ( scale < FUDGE AND scale > -FUDGE )
    return vec3( 0.0, 0.0, 0.0 );
  else
    return  v / scale;
}


/******************************************* quat::print() ************/

void   quat::print( FILE *dest, char *name )
{
  fprintf( dest, "%s:   v:<%3.2f %3.2f %3.2f>  s:%3.2f\n", name,
	   v[0], v[1], v[2], s );
}	
