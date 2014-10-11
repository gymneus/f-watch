/*
 * Copyright (C) 2014 Julian Lewis
 * @author Grzegorz Daniluk <grzegorz.daniluk@cern.ch>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/gpl-3.0-standalone.html
 * or you may search the http://www.gnu.org website for the version 3 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include "sincos.h"

#define SIN_STEP (255/90.0)

const int SIN_LUP[256] = {0,402,804,1206,1608,2010,2412,2814,3216,3617,
4019,4420,4821,5222,5623,6023,6424,6824,7223,7623,8022,8421,8820,9218,
9616,10014,10411,10808,11204,11600,11996,12391,12785,13179,13573,13966,
14359,14751,15142,15533,15924,16313,16703,17091,17479,17866,18253,18639,
19024,19408,19792,20175,20557,20939,21319,21699,22078,22456,22834,23210,
23586,23960,24334,24707,25079,25450,25820,26189,26557,26925,27291,27656,
28020,28383,28745,29106,29465,29824,30181,30538,30893,31247,31600,31952,
32302,32651,32999,33346,33692,34036,34379,34721,35061,35400,35738,36074,
36409,36743,37075,37406,37736,38064,38390,38715,39039,39361,39682,40001,
40319,40635,40950,41263,41575,41885,42194,42500,42806,43109,43411,43712,
44011,44308,44603,44897,45189,45479,45768,46055,46340,46624,46905,47185,
47464,47740,48014,48287,48558,48827,49095,49360,49624,49885,50145,50403,
50659,50913,51166,51416,51664,51911,52155,52398,52638,52877,53113,53348,
53580,53811,54039,54266,54490,54713,54933,55151,55367,55582,55794,56003,
56211,56417,56620,56822,57021,57218,57413,57606,57797,57985,58171,58356,
58537,58717,58895,59070,59243,59414,59582,59749,59913,60075,60234,60391,
60546,60699,60850,60998,61144,61287,61429,61567,61704,61838,61970,62100,
62227,62352,62475,62595,62713,62829,62942,63053,63161,63267,63371,63472,
63571,63668,63762,63853,63943,64030,64114,64196,64276,64353,64428,64500,
64570,64638,64703,64765,64826,64883,64939,64992,65042,65090,65136,65179,
65219,65258,65293,65327,65357,65386,65412,65435,65456,65475,65491,65504,
65515,65524,65530,65533};

float small_sin(int deg)
{
	int q = deg<0?-1:1;
	deg = ABS(deg);

	if (deg<=90)
		return q*SIN_LUP[(int)(deg*SIN_STEP)]/65535.0;
	deg -= 90;
	if (deg<=90)
		return q*SIN_LUP[255-(int)(deg*SIN_STEP)]/65535.0;
	deg -= 90;
	if (deg<=90)
		return -q*SIN_LUP[(int)(deg*SIN_STEP)]/65535.0;
	deg -= 90;
	if (deg<=90)
		return -q*SIN_LUP[255-(int)(deg*SIN_STEP)]/65535.0;

	return 0xff;
}

float small_cos(int deg)
{
	deg = ABS(deg);

	if (deg<=90)
		return SIN_LUP[255-(int)(deg*SIN_STEP)]/65535.0;
	deg -= 90;
	if (deg<=90)
		return -SIN_LUP[(int)(deg*SIN_STEP)]/65535.0;
	deg -= 90;
	if (deg<=90)
		return -SIN_LUP[255-(int)(deg*SIN_STEP)]/65535.0;
	deg -= 90;
	if (deg<=90)
		return SIN_LUP[(int)(deg*SIN_STEP)]/65535.0;

	return 0xff;
}

int small_atan(float y, float x)
{
	float coeff_1 = PI/4.0;
	float coeff_2 = 3.0*coeff_1;
	float r;
	float angle;

	if (x>=0) {
		r = (x - ABS(y)) / (x+ABS(y));
		angle = coeff_1 - coeff_1*r;
	} else {
		r = (x + ABS(y)) / (ABS(y)-x);
		angle = coeff_2 - coeff_1*r;
	}

	/*convert from radians to degrees*/
	angle *= 180/PI;

	if (y<0) return (int)-angle;
	else return (int)angle;
}

unsigned int small_sqrt(unsigned int val)
{
	unsigned int rem = 0, root = 0;
	int i;

	for(i=0; i<16; ++i) {
		root <<= 1;
		rem = ((rem<<2)+(val>>30));
		val <<= 2;
		++root;
		if(root <= rem) {
			rem -= root;
			++root;
		} else
			--root;
	}
	return (root>>1);
}
