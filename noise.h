#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include <cmath>
#include <cstdlib>

#include <QColor>

class Noise {
  private:
    unsigned height, width;
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

    unsigned _getTurbulence(double x, double y, double size) {
      double value = 0.0;
      double initialSize = size;

      while (size >= 1.0) {
        value += _getSmoothNoise(x / size, y / size) * size;
        size /= 2.0;
      }

      return static_cast<unsigned>(128.0 * value / initialSize);
    }

    void map(double& x, double& y) {
      x = x * width;
      y = y * height;
    }

  public:
    Noise() : Noise(256, 256) { }

    Noise(unsigned height, unsigned width) {
      this->height = height;
      this->width = width;

      data.resize(height * width);
      generateNoise();
    }

    void generateNoise() {
      for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
          data[y * width + x] = (rand() % 32786) / 32786.0;
        }
      }
    }

    QColor getNoise(double y, double x) {
      map(x, y);
      unsigned grey = static_cast<unsigned>(data[(int)y * width + x] * 255.9);
      return QColor(grey, grey, grey);
    }

    QColor getSmoothNoise(double x, double y) {
      map(x, y);
      unsigned grey = static_cast<unsigned>(_getSmoothNoise(x, y) * 255.9);
      return QColor(grey, grey, grey);
    }

    QColor getTurbulence(double x, double y, double size) {
      map(x, y);
      unsigned grey = _getTurbulence(x, y, size);
      return QColor(grey, grey, grey);
    }

    QColor getCloud(double x, double y) {
      map(x, y);

      unsigned L = 192 + _getTurbulence(x, y, 64) / 4;
      return QColor::fromHsl(169, 255, L);
    }

    QColor getMarble(double x, double y) {
      map(x, y);

      double xPeriod = 5.0;
      double yPeriod = 10.0;
      double turbPower = 5.0;
      double turbSize = 32.0;

      double xyValue = x * xPeriod / width + y * yPeriod / height + turbPower * _getTurbulence(x, y, turbSize) / 256.0;
      double sineValue = 256 * std::fabs(std::sin(xyValue * 3.14159));

      return QColor(sineValue, sineValue, sineValue);
    }
};

#endif // NOISE_H
