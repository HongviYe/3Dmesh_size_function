#pragma once

#pragma once
#include <cmath>

namespace ykxEps
{
	const double MyEps = 1e-10;
}


// a == b
#define Equ(a, b) ((fabs((a)-(b)))<(ykxEps::MyEps))

// a > b
#define More(a, b) (((a)-(b))>(ykxEps::MyEps))

// a < b
#define Less(a, b) (((a)-(b))<(-ykxEps::MyEps))

// a >= b
#define MoreEqu(a, b) (((a)-(b))>(-ykxEps::MyEps))

// a <= b
#define LessEqu(a, b) (((a)-(b))<(ykxEps::MyEps))
