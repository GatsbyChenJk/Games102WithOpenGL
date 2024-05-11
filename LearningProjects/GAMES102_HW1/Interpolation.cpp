#pragma once
#include "Interpolation.h"


Interpolation::Interpolation(std::vector<glm::vec2> inputs)
	:observationPoints(std::move(inputs))
{
	auto CompareVec2ByX = [](const glm::vec2& a, const glm::vec2& b) {return a.x < b.x;};
	std::sort(observationPoints.begin(),observationPoints.end(),CompareVec2ByX);
}

void Interpolation::GetMutiCoef(int dimension)
{
	if (dimension == 2)
	{
		glm::mat2 A_mat;
		auto GenerateVanMat = [this,dimension](glm::mat2& vanMat)
		{
			for (int i = 0; i < dimension; ++i) {
				// 计算每列的值
				for (int j = 0; j < dimension; ++j) {
					vanMat[j][i] = glm::pow(observationPoints[i].x, j);
				}
			}
			return vanMat;
		};
		A_mat = GenerateVanMat(A_mat);
		glm::vec2 Vec_b = glm::vec2(
			this->observationPoints[0].y,
			this->observationPoints[1].y);
		glm::vec2 Vec_Coef = glm::inverse(A_mat) * Vec_b;
		differenceEveryLevel.push_back(Vec_Coef.x);
		differenceEveryLevel.push_back(Vec_Coef.y);
	}

	if (dimension == 3)
	{
		glm::mat3 A_mat;
		auto GenerateVanMat = [this, dimension](glm::mat3& vanMat)
		{
			for (int i = 0; i < dimension; ++i) {
				// 计算每列的值
				for (int j = 0; j < dimension; ++j) {
					vanMat[j][i] = glm::pow(observationPoints[i].x, j);
				}
			}
			return vanMat;
		};
		A_mat = GenerateVanMat(A_mat);
		glm::vec3 Vec_b = glm::vec3(
			this->observationPoints[0].y,
			this->observationPoints[1].y,
			this->observationPoints[2].y);
		glm::vec3 Vec_Coef = glm::inverse(A_mat) * Vec_b;
		differenceEveryLevel.push_back(Vec_Coef.x);
		differenceEveryLevel.push_back(Vec_Coef.y);
	}
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
	// y = a_0 + a_1 * x + a_2 * x^2 + ... + a_n * x^n
	if (method == MutiCoef)
	{
		GetMutiCoef(observationPoints.size());
		int n = differenceEveryLevel.size();
		float y = 0;
		for (int i = n - 1;i >= 0;i--)
		{
			y += differenceEveryLevel[i] * static_cast<float>(pow(input, i));
		}
		interpolatePoints.push_back(glm::vec2(input,y));
	}

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
		GetDifferenceQuotient2D(observationPoints[0], observationPoints[1], observationPoints[2]);
		float y = observationPoints[0].y + differenceEveryLevel[0] * (input - observationPoints[0].x) +
			differenceEveryLevel[1] * (input - observationPoints[0].x) * (input - observationPoints[1].x);
		interpolatePoints.push_back(glm::vec2(input, y));
	}
	std::cout << "interpolated points:\n";
	for (auto& result : interpolatePoints)
	{
		std::cout << "(" << result.x << "," << result.y << ")\n";
	}
}