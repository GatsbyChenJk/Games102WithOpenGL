#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <limits>

using namespace std;

class KMeans {
public:
    // KMeans 聚类的构造函数
    KMeans(int k, int maxIterations = 10) : k(k), maxIterations(maxIterations) {}

    void fit(vector<vector<double>>& data) {
        int n = data.size();
        int m = data[0].size();

        // 随机选择初始中心
        centers.resize(k, vector<double>(m));
        vector<bool> selected(n, false);
        for (int i = 0; i < k; ++i) {
            int index;
            do {
                index = rand() % n;
            } while (selected[index]);
            selected[index] = true;
            centers[i] = data[index];
        }

        vector<int> assignments(n);
        for (int iteration = 0; iteration < maxIterations; ++iteration) {
            // 分配每个点到最近的中心
            for (int i = 0; i < n; ++i) {
                double minDistance = numeric_limits<double>::max();
                for (int j = 0; j < k; ++j) {
                    double distance = euclideanDistance(data[i], centers[j]);
                    if (distance < minDistance) {
                        minDistance = distance;
                        assignments[i] = j;
                    }
                }
            }

            // 更新中心
            vector<vector<double>> newCenters(k, vector<double>(m, 0.0));
            vector<int> counts(k, 0);
            for (int i = 0; i < n; ++i) {
                int cluster = assignments[i];
                counts[cluster]++;
                for (int dim = 0; dim < m; ++dim) {
                    newCenters[cluster][dim] += data[i][dim];
                }
            }
            for (int j = 0; j < k; ++j) {
                if (counts[j] > 0) {
                    for (int dim = 0; dim < m; ++dim) {
                        newCenters[j][dim] /= counts[j];
                    }
                }
            }

            centers = newCenters;
        }
    }

    const vector<vector<double>>& getCenters() const {
        return centers;
    }

private:
    int k;
    int maxIterations;
    vector<vector<double>> centers;

    double euclideanDistance(const vector<double>& a, const vector<double>& b) {
        double sum = 0.0;
        for (size_t i = 0; i < a.size(); ++i) {
            sum += pow(a[i] - b[i], 2);
        }
        return sqrt(sum);
    }
};