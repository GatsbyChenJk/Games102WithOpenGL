#include "RBFNetwork.h"

RBFNetwork::RBFNetwork(int input, int hidden, int output)
	:inputSize(input), hiddenSize(hidden), outputSize(output)
{
    // ��ʼ�� RBF �ڵ����ġ���չ������Ȩ��
    centers.resize(hiddenSize, vector<double>(inputSize));
    spreads.resize(hiddenSize);
    weights.resize(hiddenSize, vector<double>(outputSize));

    // �����ʼ�����ĺ�Ȩ��
    for (int i = 0; i < hiddenSize; i++) {
        for (int j = 0; j < inputSize; j++) {
            centers[i][j] = ((double)rand() / RAND_MAX); // �����ʼ������
        }
        spreads[i] = 1.0; // Ĭ����չ����
        for (int j = 0; j < outputSize; j++) {
            weights[i][j] = ((double)rand() / RAND_MAX); // �����ʼ��Ȩ��
        }
    }
}

// ǰ������
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

// ѵ�� RBF ���磨ʹ�ü򵥵���С���˷�����Ȩ�أ�
void RBFNetwork::train(const vector<vector<double>>& inputs, const vector<vector<double>>& targets, double learningRate)
{
    // �������ʵ�ּ򵥵�����ѵ��
    int dataCount = inputs.size();
    for (int n = 0; n < dataCount; n++)
    {
        vector<double> output;
        feedForward(inputs[n], output);

        // ����Ȩ��
        for (int j = 0; j < outputSize; j++) 
        {
            for (int i = 0; i < hiddenSize; i++)
            {
                weights[i][j] += learningRate * (targets[n][j] - output[j]) * output[j]; // Ȩ�ظ���
            }
        }
    }
}
