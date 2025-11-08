#include <engines/maths/converter.hpp>

namespace Engines::Maths {
  float Converter::meterToPixel(const float meter, const float *magnification){
    const float r_max_pixel = 0.03 * Config::SCR_WIDTH;

     // Assuming the sun is the largest entity
    const float s = r_max_pixel/Engines::Physics::Constants::RADIUS_SUN_M;
    
    return meter * s * (magnification!=nullptr?*magnification:1.0f);
  }

  float Converter::scaleDistanceBySurfaceSize(const float meter, const float size, const float magnification){
    float scale = size / Engines::Physics::Constants::DISTANCE_SUN_PLUTO_M;
    return meter * scale * magnification;
  }
}