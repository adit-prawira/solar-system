#pragma once
#include <config/constant.hpp>
#include <engines/physics/constants.hpp>

namespace Engines::Maths {
  class Converter {
    public:
      static float meterToPixel(const float meter, const float *magnification = nullptr);
      static float scaleDistanceBySurfaceSize(const float meter, const float size, const float magnification = 1.0f);
  };
}