#pragma once
#include <config/constant.hpp>
#include <engines/physics/constants.hpp>

namespace Engines::Maths {
  class Converter {
    public:
      static float meterToPixel(const double meter, const double *magnification = nullptr);
  };
}