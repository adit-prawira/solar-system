# Solar System — improvements

All changes are in `include/simulation/celestial_body.hpp`, `include/app.hpp`, `src/app.cpp` unless noted.

---

## Slice 1 — Fix orbital decay (root cause bug)

**Files**: `celestial_body.hpp`

**Problem**: `calculateOrbitalVelocity()` returns real-world orbital velocity (m/s). `revolve()` applies gravitational acceleration scaled by `k = 5e4` but never scales velocity. The emergency velocity re-init inside `revolve()` _does_ use `sqrt(k)`. This mismatch makes planets ~224× too slow for the fake gravity → gradual in-spiral → runaway.

**Fix**:
- Add `static constexpr double ACCEL_SCALE = 5e4;` and `static constexpr double VEL_SCALE = std::sqrt(ACCEL_SCALE);` to the class.
- Replace all `k` references in `revolve()` with `ACCEL_SCALE`.
- Replace `* sqrt(k)` in the emergency re-init with `* VEL_SCALE`.
- In `calculateOrbitalVelocity()`, return `tangent * orbital_speed * CelestialBody::VEL_SCALE`.

---

## Slice 2 — Sun collision guard

**File**: `celestial_body.hpp` line 141

**Problem**: `min_distance = 1e3` (1000 m) is 700 000× smaller than the sun's radius (6.963e8 m). Planets pass through the sun and hit `a ∝ 1/r²` → `a ≈ 10¹⁸ m/s²` → immediate blow-up.

**Fix**:
```cpp
const double min_distance = Engines::Physics::Constants::RADIUS_SUN_M;
```
Optionally: when `distance < RADIUS_SUN_M`, zero velocity and stop integration (absorb the planet).

---

## Slice 3 — Visual consistency

**File**: `celestial_body.hpp`

**Problems**:
1. `getRenderPosition()` applies `visual_zoom = 1000.0f` **only to Z** — orbits visually stretch in Z.
2. `setHasOrbit(float)` takes `float` but stores `bool`.
3. `orbit_center` is `glm::vec3` (single precision) while physics runs in `glm::dvec3`.

**Fixes**:
- Apply `visual_zoom` to all three axes (x, y, z), not just z.
- Change `setHasOrbit(float)` → `setHasOrbit(bool)`.
- Change `orbit_center` to `glm::dvec3`, cast to `glm::vec3` at use site in `build()`.

---

## Slice 4 — Fixed timestep

**Files**: `app.hpp`, `app.cpp`

**Problem**: `celestial_body->revolve(stars, 1.0)` runs once per frame. Physics speed changes with display refresh rate.

**Fix**:
- Add `double physics_accumulator = 0.0;` and `const double fixed_dt = 1.0 / 60.0;` to `App`.
- Track `glfwGetTime()` each frame → compute `frame_delta_time`.
- Accumulate and consume `fixed_dt` chunks:
```
accumulator += frame_delta_time
while accumulator >= fixed_dt:
    for each body: body.revolve(stars, fixed_dt)
    accumulator -= fixed_dt
```

---

## Slice 5 — UX polish

**Trails are stored but never drawn**: Add `body->drawTrail()` call in render loop. Enable `.setIsShowTrail(true)` on one planet (e.g. Earth) for visual feedback.

**Pause**: Bind `SPACE` to toggle a `paused` flag that guards the `revolve()` calls. Use `GLFW_KEY_SPACE` + edge trigger (press-once, not hold).

**Speed control**: Bind `[` / `]` keys to adjust `time_scale` (range 0.1× – 10×). Multiply `fixed_dt` by this.

**Camera FOV**: Set projection matrix from a dynamic FOV value that `processMouseScroll` adjusts, so zoom feels immediate and responsive.
