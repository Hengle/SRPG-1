#include "stdafx.h"
#include "Math.h"


CMath::CMath()
{
}


CMath::~CMath()
{
}

float CMath::EaseIn(float t, float b, float c, float d)
{
	t /= d;
	return c*t*t + b;
}

float CMath::EaseOut(float t, float b, float c, float d) 
{
	t /= d;
	return -c * t*(t - 2) + b;
};