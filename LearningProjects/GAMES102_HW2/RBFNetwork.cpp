#include "RBFNetwork.h"

RBFNetwork::RBFNetwork(int input, int hidden, int output)
	:inputSize(input), hiddenSize(hidden), outputSize(output)
{
    // 初始化 RBF 节点中心、扩展参数和权重
    centers.resize(hiddenSize, vector<double>(inputSize));
    spreads.resize(hiddenSize);
    weights.resize(hiddenSize, vector<double>(outputSize));

    // 随机初始化中心和权重
    for (int i = 0; i < hiddenSize; i++) {
        for (int j = 0; j < inputSize; j++) {
            centers[i][j] = log(exp(i) + sin(i));
        }
        double cmax_i = centers[i][0];
        for (int j = 0; j < centers[i].size();j++)
        {
            if (cmax_i < centers[i][j])
                cmax_i = centers[i][j];
        }
        spreads[i] = cmax_i / sqrt(2 * hiddenSize);
        for (int j = 0; j < outputSize; j++) 
        {
            weights[i][j] = ((double)rand() / RAND_MAX); // 随机初始化权重
        }
    }
}

RBFNetwork::RBFNetwork(int input, int hidden, int output,
    vector<vector<double>> inputs)
    :inputSize(input), hiddenSize(hidden), outputSize(output)
{
    // 初始化 RBF 节点中心、扩展参数和权重
    centers.resize(hiddenSize, vector<double>(inputSize));
    spreads.resize(hiddenSize);
    weights.resize(hiddenSize, vector<double>(outputSize));

    KMeans kmeans(hiddenSize);
    kmeans.fit(inputs);
    centers = kmeans.getCenters();

    for (int i = 0;i < hiddenSize;i++)
    {
        double cmax_i = centers[i][0];
        for (int j = 0; j < centers[i].size();j++)
        {
            if (cmax_i < centers[i][j])
                cmax_i = centers[i][j];
        }
        spreads[i] = cmax_i / sqrt(2 * hiddenSize);
        for (int j = 0; j < outputSize; j++)
        {
            weights[i][j] = ((double)rand() / RAND_MAX); // 随机初始化权重
        }
    }
}

// 前馈计算
// compute outputs of every input point -> store result in "output" vector 
// (x_i, f(x_i)) -> (input[i],output[i])
void RBFNetwork::feedForward(const vector<double>& input, vector<double>& output) 
{
    // temp vec which store output from activate func
    vector<double> rbfOutputs(hiddenSize);
    for (int i = 0; i < hiddenSize; i++)
    {
        double distance = 0.0;
        for (int j = 0; j < inputSize; j++)
        {
            distance += pow(input[j] - centers[i][j], 2);
        }
        rbfOutputs[i] = gaussian(sqrt(distance), spreads[i]);
    }
    // compute outputs
    output.resize(outputSize);
    for (int j = 0; j < outputSize; j++)
    {
        output[j] = 0.0;
        for (int i = 0; i < hiddenSize; i++) 
        {
            output[j] += rbfOutputs[i] * weights[i][j];
        }
    }
}

// 训练 RBF 网络（使用简单的最小二乘法更新权重）
void RBFNetwork::trainWithLSM(const vector<vector<double>>& inputs, const vector<vector<double>>& targets, double learningRate)
{
    // 这里可以实现简单的批量训练
    int dataCount = inputs.size();
    for (int n = 0; n < dataCount; n++)
    {
        vector<double> output;
        feedForward(inputs[n], output);

        // 更新权重
        for (int j = 0; j < outputSize; j++) 
        {
            for (int i = 0; i < hiddenSize; i++)
            {
                weights[i][j] += learningRate * (targets[n][j] - output[j]) * output[j]; // 权重更新
            }
        }
    }
}

void RBFNetwork::trainWithGD(const vector<vector<double>>& inputs, const vector<vector<double>>& targets, double learningRate)
{
    for (int n = 0;n < inputs.size();n++)
    {
        vector<double> output;
        feedForward(inputs[n], output);
        // 更新权重
        for (int j = 0; j < outputSize; j++)
        {
            for (int i = 0; i < hiddenSize; i++)
            {
                weights[i][j] -= learningRate * (weights[i][j]*output[j] - targets[n][j]) * output[j]; // 权重更新
            }
        }
    }

}

void RBFNetwork::trainWithBP(const vector<vector<double>>& inputs, const vector<vector<double>>& targets, double learningRate)
{
    for (int n = 0;n < inputs.size();n++)
    {
        vector<double> output;
        feedForward(inputs[n], output);
        
    }
}

void RBFNetwork::training(const vector<vector<double>>& inputs, const vector<vector<double>>& targets, double learningRate,TrainingAlgorithm method,int times)
{
    for (int i = 0;i < times;i++)
    {
        if (method == LSM)
            this->trainWithLSM(inputs, targets, learningRate);
        else if (method == GD)
            this->trainWithGD(inputs, targets, learningRate);
    }
}
