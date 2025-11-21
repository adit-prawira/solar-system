#pragma once
namespace Engines::Physics::Constants {
  constexpr float MASS_SUN_KG = 1.989e30f;

  constexpr float RADIUS_SUN_M = 6.963e8f;

  constexpr float MASS_MERCURY_KG = 3.301e23f;
  constexpr float RADIUS_MERCURY_M = 2.4397e6f;

  constexpr float MASS_VENUS_KG = 4.867e24f;
  constexpr float RADIUS_VENUS_M = 6.0518e6f;

  constexpr float MASS_EARTH_KG = 5.972e24f;

  constexpr float RADIUS_EARTH_M = 6.371e6f;

  constexpr float MASS_MARS_KG = 6.417e23f;
  constexpr float RADIUS_MARS_M = 3.3895e6f;
  
  constexpr float MASS_JUPITER_KG = 1.898e27f;
  constexpr float RADIUS_JUPITER_M = 6.991e7f;

  constexpr float MASS_SATURN_KG = 5.683e26f;
  constexpr float RADIUS_SATURN_M = 5.823e7;

  constexpr float MASS_URANUS_KG = 8.681e25f;
  constexpr float RADIUS_URANUS_M = 2.536e7f;

  constexpr float MASS_NEPTUNE_KG = 1.024e26f;
  constexpr float RADIUS_NEPTUNE_M = 2.462e7f;

  constexpr float MASS_PLUTO_KG = 1.309e22f;
  constexpr float RADIUS_PLUTO_M = 1.188e6f;

  constexpr float GRAVITY = 9.81f;
  constexpr float SPEED_OF_LIGHT = 299792458.0;
  constexpr float GRAVITATIONAL_CONSTANT = 6.67430e-11f;

  constexpr float METER_PER_PIXEL = 1e8f/100.0f;
  constexpr float DISTANCE_SUN_MERCURY_M = 5.79e10f;
  constexpr float DISTANCE_SUN_VENUS_M   = 1.0821e11f;
  constexpr float DISTANCE_SUN_EARTH_M   = 1.496e11f;
  constexpr float DISTANCE_SUN_MARS_M    = 2.279e11f;
  constexpr float DISTANCE_SUN_JUPITER_M = 7.785e11f;
  constexpr float DISTANCE_SUN_SATURN_M  = 1.433e12f;
  constexpr float DISTANCE_SUN_URANUS_M  = 2.877e12f;
  constexpr float DISTANCE_SUN_NEPTUNE_M = 4.503e12f;
  constexpr float DISTANCE_SUN_PLUTO_M   = 5.906e12f;

  static constexpr float UNIT_SCALE = 1e-6;
}