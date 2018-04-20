#pragma once
class CMath
{
public:
	CMath();
	~CMath();
public:
	static float EaseIn(float t, float b, float c, float d);
	static float EaseOut(float t, float b, float c, float d);
};

