#ifndef PERLIN_H
#define PERLIN_H

#include <vector>
#include <cstdlib>

class Noise {
  private:
    unsigned height, width;
    std::vector<double> data;

    void generateNoise() {
      for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
          data[y * width + x] = (rand() % 32786) / 32786.0;
        }
      }
    }

  public:
    Noise() : Noise(128, 128) { }

    Noise(unsigned height, unsigned width) {
      this->height = height;
      this->width = width;

      data.resize(height * width);
      generateNoise();
    }

    unsigned getNoise(int y, int x) {
      if (y * width + x > height * width) {
        std::cerr << "ERR: y: " << y << " x: " << x << "\n";
        return 0;
      }
      return static_cast<unsigned>(data[y * width + x] * 255.9);
    }

    unsigned getSmoothNoise(double x, double y) {
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

      return unsigned(value * 255.9);
    }
};

#endif // PERLIN_H
