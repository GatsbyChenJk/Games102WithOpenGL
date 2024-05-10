#include "Fitting.h"

Fitting::Fitting(std::vector<glm::vec2> inputs)
	:observationPoints(std::move(inputs))
{
	auto CompareVec2ByX = [](const glm::vec2& a, const glm::vec2& b) {return a.x < b.x;};
	std::sort(observationPoints.begin(), observationPoints.end(), CompareVec2ByX);
}

void Fitting::GetCoef2D(FittingMethod method)
{
	if (method == Linear)
	{
		enum SumType
		{
			Single_x,
			Single_y,
			Pow_x,
			Mul_xy
		};
		// sum up func
		auto SumUp = [](std::vector<glm::vec2> Vec_i, SumType type)
		{
			float sum = 0;
			for (auto it = Vec_i.begin();it != Vec_i.end();it++)
			{
				if (type == Single_x)
					sum += it->x;
				else if (type == Single_y)
					sum += it->y;
				else if (type == Mul_xy)
					sum += it->x * it->y;
				else if (type == Pow_x)
					sum += pow(it->x, 2);
				else
					break;
			}
			return sum;
		};

		float N = observationPoints.size();
		float Sum_A = SumUp(observationPoints, Mul_xy);
		float Sum_B = SumUp(observationPoints, Pow_x);
		float Sum_C = SumUp(observationPoints, Single_x);
		float Sum_D = SumUp(observationPoints, Single_y);

		float A_hat = (N * Sum_A - Sum_C * Sum_D) / (N * Sum_B - Sum_C * Sum_C);
		float B_hat = Sum_D / N - A_hat * Sum_C / N;
		coef_w.push_back(A_hat);
		coef_b = B_hat;
	}
}

void Fitting::GetEstimateNum2D(float input, FittingMethod method)
{
	if (method == Linear)
	{
		GetCoef2D(Linear);
		float EstimateY_i = coef_w[0] * input + coef_b;
		fittingPoints.push_back(glm::vec2(input, EstimateY_i));
	}
}
	
