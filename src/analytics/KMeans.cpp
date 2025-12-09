#include "KMeans.h"

KMeans::KMeans(int k, int maxIters) : m_k(k), m_maxIters(maxIters) {}

double KMeans::distance(const Point &a, const Point &b) {
  return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

void KMeans::fit(std::vector<Point> &points) {
  if (points.empty())
    return;

  // Inicialización Aleatoria (Random)
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, points.size() - 1);

  m_centroids.clear();
  for (int i = 0; i < m_k; ++i) {
    Point p = points[dis(gen)];
    p.cluster = i;
    m_centroids.push_back(p);
  }

  for (int iter = 0; iter < m_maxIters; ++iter) {
    bool changed = false;

    // Asignar puntos al centroide más cercano
    for (auto &p : points) {
      double minDesc = std::numeric_limits<double>::max();
      int bestCluster = -1;

      for (int i = 0; i < m_k; ++i) {
        double d = distance(p, m_centroids[i]);
        if (d < minDesc) {
          minDesc = d;
          bestCluster = i;
        }
      }

      if (p.cluster != bestCluster) {
        p.cluster = bestCluster;
        changed = true;
      }
    }

    // Actualizar centroides
    std::vector<Point> newCentroids(m_k, {0, 0, 0});
    std::vector<int> counts(m_k, 0);

    for (const auto &p : points) {
      if (p.cluster >= 0 && p.cluster < m_k) {
        newCentroids[p.cluster].x += p.x;
        newCentroids[p.cluster].y += p.y;
        counts[p.cluster]++;
      }
    }

    for (int i = 0; i < m_k; ++i) {
      if (counts[i] > 0) {
        m_centroids[i].x = newCentroids[i].x / counts[i];
        m_centroids[i].y = newCentroids[i].y / counts[i];
      }
    }

    if (!changed)
      break;
  }
}

const std::vector<Point> &KMeans::getCentroids() const { return m_centroids; }
