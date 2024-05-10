#pragma once
#ifndef FITTING_H
#define FITTING_H

#include <cmath>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum FittingMethod
{
	Linear,
	Quad,
	Cubic
};

class Fitting
{
public:
	std::vector<glm::vec2> observationPoints;
	std::vector<glm::vec2> fittingPoints;
	std::vector<float> coef_w;
	float coef_b;
public:
	Fitting() = default;
	Fitting(std::vector<glm::vec2> inputs);
	~Fitting() = default;
private:
	void GetCoef2D(FittingMethod method);
public:
	void GetEstimateNum2D(float input, FittingMethod method);
};

#endif // FITTING_H
