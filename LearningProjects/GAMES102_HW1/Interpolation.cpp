#pragma once
#include "Interpolation.h"

bool CompareVec2ByX(const glm::vec2& a, const glm::vec2& b)
{
	return a.x < b.x;
}

Interpolation::Interpolation(std::vector<glm::vec2> inputs)
	:observationPoints(std::move(inputs))
{
	std::sort(observationPoints.begin(),observationPoints.end(),CompareVec2ByX);
}

float Interpolation::GetBaseFunction2D(float x, float x_0, float x_1, float x_2)
{
	float numerator = (x - x_1) * (x - x_2);
	float denominator = (x_0 - x_1) * (x_0 - x_2);
	return numerator / denominator;
}

void Interpolation::GetDifferenceQuotient2D(glm::vec2 P_1, glm::vec2 P_2, glm::vec2 P_3)
{
	// 一阶差商
	float f_P1_P2 = (P_2.y - P_1.y) / (P_2.x - P_1.x);
	float f_P2_P3 = (P_3.y - P_2.y) / (P_3.x - P_2.x);
	differenceEveryLevel.push_back(f_P1_P2);
	// 二阶差商
	float f_P_123 = (f_P2_P3 - f_P1_P2) / (P_3.x - P_1.x);
	differenceEveryLevel.push_back(f_P_123);
}

void Interpolation::GetEstimateNum2D(float input, InterpolationMethod method)
{
	// y = l_0(x) * y_0 + l_1(x) * y_1 + l_2(x) * y_2
	if (method == Lagrange)
	{
		float l_0 = GetBaseFunction2D(input, observationPoints[0].x, observationPoints[1].x, observationPoints[2].x);
		float l_1 = GetBaseFunction2D(input, observationPoints[1].x, observationPoints[0].x, observationPoints[2].x);
		float l_2 = GetBaseFunction2D(input, observationPoints[2].x, observationPoints[0].x, observationPoints[1].x);
		float y = l_0 * observationPoints[0].y + l_1 * observationPoints[1].y + l_2 * observationPoints[2].y;
		interpolatePoints.push_back(glm::vec2(input, y));
	}
	// y = f[x_0] + f[x_0,x_1]*(x - x_0) + f[x_0,x_1,x_2]*(x - x_0)*(x - x_1)
	if (method == NewTon)
	{
		float y = observationPoints[0].y + differenceEveryLevel[0] * (input - observationPoints[0].x) +
			differenceEveryLevel[1] * (input - observationPoints[0].x) * (input - observationPoints[1].x);
		interpolatePoints.push_back(glm::vec2(input, y));
	}

}