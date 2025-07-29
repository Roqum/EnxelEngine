#pragma once


#ifdef ENXEL_BUILD
#define ENXEL_API __declspec(dllexport)
#else
#define ENXEL_API __declspec(dllimport)
#endif

class ENXEL_API Enxel
{
public:
	void StartRenderer();
};