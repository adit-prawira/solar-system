#include <engines/maths/converter.hpp>

namespace Engines::Maths {
  
  float Converter::meterToPixel(const float meter, const float *magnification){
    // Internal computations in double for precision
    double meter_d = static_cast<double>(meter);
    double mag = (magnification != nullptr) ? static_cast<double>(*magnification) : 1.0;

    const double usable_screen_width = static_cast<double>(Config::SCR_WIDTH - Config::SCR_MARGIN);
    const double scale = usable_screen_width / static_cast<double>(Engines::Physics::Constants::DISTANCE_SUN_PLUTO_M);

    double pixels = meter_d * scale * mag;

    return static_cast<float>(pixels); // return as float for rendering
  }
}