#include <engines/maths/converter.hpp>

namespace Engines::Maths {
  
  float Converter::meterToPixel(const float meter, const float *magnification){
     // Assuming the sun is the largest entity

    const float usable_screen_width = Config::SCR_WIDTH - Config::SCR_MARGIN;
    const float s = usable_screen_width/Engines::Physics::Constants::DISTANCE_SUN_PLUTO_M;
    
    return meter * s * (magnification!=nullptr?*magnification:1.0f);
  }

  float Converter::scaleDistanceBySurfaceSize(const float meter, const float magnification){
    const float usable_screen_width = Config::SCR_WIDTH - Config::SCR_MARGIN;
    const float s = usable_screen_width/Engines::Physics::Constants::DISTANCE_SUN_PLUTO_M;
    
    return meter * s * magnification;
  }
}