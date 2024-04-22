#pragma once
#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum InterpolationMethod
{
	Lagrange,
	NewTon
};

class Interpolation
{
public:
	std::vector<glm::vec2> observationPoints;
	std::vector<glm::vec2> interpolatePoints;
	int outPutPointsNum;
	float ErrorRate;
	

public:
	Interpolation() = default;
	Interpolation(std::vector<glm::vec2> inputs);
	~Interpolation() = default;
	// Lagrange Method
	float GetBaseFunction2D(float x, float x_0, float x_1, float x_2);

	// Newton Method
	float GetDifferenceQuotient2D(glm::vec2 P_1, glm::vec2 P_2, glm::vec2 P_3);

	// calculation
	float GetEstimateNum2D(int pointNum, InterpolationMethod method);
	float ComputeErrorRate2D();
};



#endif // !INTERPOLATION_H
