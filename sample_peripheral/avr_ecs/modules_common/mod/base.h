/* Copyright 2011 Matt Bennett

    This file is part of uavcamera.

    uavcamera is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    uavcamera is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with uavcamera.  If not, see <http://www.gnu.org/licenses/>.

*/

// Base definitions and types

#ifndef __BASE_H
#define __BASE_H

// Definitions

#define P0                  1013.25f    // standard pressure (in mb) at sea level
#define T0                  288.15f     // standard temperature (in K) [15 deg C]
#define L0                  0.0065f     // standard temperature lapse rate (in K/m)
#define R_AIR               287.053f    // specific gas constant for dry air (in J/(kg.K))
#define GRAVITY             9.81f       // in ms^-2

#define TRUE                1u
#define FALSE               0u

#ifndef NULL
  #define NULL              0u
#endif

#define DEG_TO_RAD          0.0174532925199433f

#define HALF_PI             1.5707963267948966f
#define THREE_QUARTERS_PI   2.3561944901923449f
#define ONE_POINT_FIVE_PI   4.7123889803846899f
#define TWO_PI              6.2831853071795862f
#define PI                  3.1415926535897931f

typedef unsigned char       uint8_t;
typedef signed char         int8_t;
typedef unsigned int	      uint16_t;
typedef signed int	        int16_t;
typedef unsigned long int   uint32_t;
typedef signed long int     int32_t;
typedef float               float32_t;
typedef double              float64_t;
typedef unsigned char       boolean_t;

// Public globals

// Function prototypes

#endif // __BASE_H
