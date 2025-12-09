#pragma once
#include <cmath>
#include <limits>
#include <random>
#include <vector>


struct Point {
  double x, y;
  int cluster;
};

class KMeans {
public:
  KMeans(int k, int maxIters = 100);
  void fit(std::vector<Point> &points);
  const std::vector<Point> &getCentroids() const;

private:
  int m_k;
  int m_maxIters;
  std::vector<Point> m_centroids;

  double distance(const Point &a, const Point &b);
};
