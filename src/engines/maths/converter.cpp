#include <engines/maths/converter.hpp>

namespace Engines::Maths {
  
  float Converter::meterToPixel(const double meter, const double *magnification){
    // Internal computations in double for precision
    double mag = (magnification != nullptr) ? *magnification : 1.0;

    const double usable_screen_width = static_cast<double>(Config::SCR_WIDTH - Config::SCR_MARGIN);
    const double scale = usable_screen_width / static_cast<double>(Engines::Physics::Constants::DISTANCE_SUN_PLUTO_M);

    double pixels = meter * scale * mag;

    return static_cast<float>(pixels); // return as float for rendering
  }
}