#ifndef MACRO_HPP
#define MACRO_HPP

#define max(a, b) (a < b ? b : a)
#define lerp(a, b, t) (a + t * (b - a))
#define inverseLerp(a, b, t) ((t - a) / (b - a))

#endif