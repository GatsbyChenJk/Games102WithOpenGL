#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include "KMeans.h"

using namespace std;

enum TrainingAlgorithm
{
    LSM,
    GD,
    BP
};

class RBFNetwork {
private:
    int inputSize;      // 输入维度
    int hiddenSize;    // 隐藏层（RBF 节点）数量
    int outputSize;    // 输出维度

    vector<vector<double>> centers;                          // RBF 节点中心
    vector<double> spreads;                                       // RBF 节点扩展参数
    vector<vector<double>> weights;                         // 隐藏层到输出层的权重

    double gaussian(double distance, double spread)
    {
        return exp(-(distance * distance) / (2 * spread * spread));
    }

public:
    RBFNetwork(int input, int hidden, int output);
    RBFNetwork(int input, int hidden, int output, vector<vector<double>> inputs);
    // 前馈计算
    void feedForward(const vector<double>& input, vector<double>& output);
    // 训练 RBF 网络（使用简单的最小二乘法更新权重）
    void trainWithLSM(const vector<vector<double>>& inputs, const vector<vector<double>>& targets, double learningRate);
    // using GD to train
    void trainWithGD(const vector<vector<double>>& inputs, const vector<vector<double>>& targets, double learningRate);
    // using SGD and Backward to train
    void trainWithBP(const vector<vector<double>>& inputs, const vector<vector<double>>& targets, double learningRate);

    void training(const vector<vector<double>>& inputs, const vector<vector<double>>& targets, double learningRate, TrainingAlgorithm method = LSM, int times = 5);
};
