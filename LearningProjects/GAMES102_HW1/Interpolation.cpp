#pragma once
#include "Interpolation.h"

Interpolation::Interpolation()
{
}

Interpolation::Interpolation(std::vector<glm::vec2> inputs)
	:observationPoints(std::move(inputs))
{
}

float Interpolation::GetBaseFunction2D(float x, float x_0, float x_1, float x_2)
{
	float numerator = (x - x_1) * (x - x_2);
	float denominator = (x_0 - x_1) * (x_0 - x_2);
	return numerator / denominator;
}

float Interpolation::GetDifferenceQuotient2D(glm::vec2 P_1, glm::vec2 P_2)
{
	float numerator = P_1.y - P_2.y;
	float denominator = P_1.x - P_2.x;
	return numerator / denominator;
}

float Interpolation::GetEstimateNum2D(int pointNum, InterpolationMethod method)
{
	// y = l_0(x)*y_0 + l_1(x)*y_1 + l_2(x)*y_2
	if (method == Lagrange)
	{

	}
	if (method == NewTon)
	{

	}

}