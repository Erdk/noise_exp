#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include <random>

#include <QColor>

enum NoiseType {
  NOISE,
  SMOOTHNOISE,
  TURBULENCE,
  CLOUD,
  MARBLE
};

class Noise {
  private:
    std::random_device r;
    std::mt19937 gen;
    std::uniform_real_distribution<> distribution;

    int height, width;
    std::vector<double> data;

    double _getSmoothNoise(double x, double y) {
      double fractX = x - int(x);
      double fractY = y - int(y);

      int x1 = (int(x) + width) % width;
      int y1 = (int(y) + height) % height;

      int x2 = (x1 + width - 1) % width;
      int y2 = (y1 + height - 1) % height;

      double value = 0.0;
      value += fractX * fractY * data[y1 * width + x1];
      value += (1 - fractX) * fractY * data[y1 * width + x2];
      value += fractX * (1 - fractY) * data[y2 * width + x1];
      value += (1 - fractX) * (1 - fractY) * data[y2 * width + x2];

      return value;
    }

    int _getTurbulence(double x, double y, double size) {
      double value = 0.0;
      double initialSize = size;

      while (size >= 1.0) {
        value += _getSmoothNoise(x / size, y / size) * size;
        size /= 2.0;
      }

      return static_cast<int>(128.0 * value / initialSize);
    }

    void map(double& x, double& y) {
      x = x * width;
      y = y * height;
    }

  public:
    Noise() : Noise(256, 256) { }

    Noise(int height, int width) {
      gen = std::mt19937(r());
      distribution = std::uniform_real_distribution<>(0.0, 1.0);

      this->height = height;
      this->width = width;

      data.resize(height * width);
      generateNoise();
    }

    void generateNoise() {
      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          data[y * width + x] = distribution(gen);
        }
      }
    }

    QColor getNoise(double y, double x) {
      map(x, y);
      int grey = static_cast<int>(data[static_cast<int>(y) * width + static_cast<int>(x)] * 255.9);
      return QColor(grey, grey, grey);
    }

    QColor getSmoothNoise(double x, double y) {
      map(x, y);
      int grey = static_cast<int>(_getSmoothNoise(x, y) * 255.9);
      return QColor(grey, grey, grey);
    }

    QColor getTurbulence(double x, double y, double size) {
      map(x, y);
      int grey = _getTurbulence(x, y, size);
      return QColor(grey, grey, grey);
    }

    QColor getCloud(double x, double y) {
      map(x, y);
      int L = 192 + _getTurbulence(x, y, 64) / 4;
      return QColor::fromHsl(169, 255, L);
    }

    QColor getMarble(double x, double y, double xPeriod = 5.0, double yPeriod = 10.0, double turbPower = 5.0, double turbSize = 32.0) {
      map(x, y);
      //double xPeriod = 5.0;
      //double yPeriod = 10.0;
      //double turbPower = 5.0;
      //double turbSize = 32.0;

      double xyValue = x * xPeriod / width + y * yPeriod / height + turbPower * _getTurbulence(x, y, turbSize) / 256.0;
      int sineValue = static_cast<int>(255.9 * std::fabs(std::sin(xyValue * 3.14159)));

      return QColor(sineValue, sineValue, sineValue);
    }
};

#endif // NOISE_H
