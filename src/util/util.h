#ifndef	__UTIL_H__
#define	__UTIL_H__

/* ================ */

enum{
	X = 0,
	Y,
	Z,
	S,
	W = Z,
	H = S,
	XY = 2,
	XYZ = 3,
	XYZS = 4,
	XYWH = 4,
};

/* ================ */

#ifndef PI
#define PI (3.1415926535897932f)
#endif

#ifndef SQRT
#define SQRT (1.41421356f)
#endif

/* ================ */

#endif	/* __UTIL_H__ */

