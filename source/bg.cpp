#include <bg.hpp>
#include <cmath>

namespace BP {

static float MakeOffset(float x) {
  float y = cos(x) * 42;
  return y - floor(y);
}

static void Wave(int index, R7Vec2 position, R7Vec2 size, float time) {
  float offset = MakeOffset(index) * 62;
  float x_position =
      position.x + size.x / 8 * ((index % 11) - 1) + cos(offset + time) * 10;
  float y_position = position.y + size.y / 8 * (index / 11) + 40 +
                     sin(offset + time) * 10 + 30;
  float color_effect = 1 - exp(-(index / 11) / 3.0f);

  // As this is 3ds dont go out of the box
  int shrink = 0;
  if (y_position >= position.y + size.y - 90) {
    shrink = y_position - (position.y + size.y - 90);
  }
  // Just to make sure...
  y_position = std::min(y_position, position.y + size.y - (90 - shrink));
  UI7::GetBackgroundList()->AddTriangle(
      R7Vec2(x_position, y_position),
      R7Vec2(x_position + 300, y_position + (90 - shrink)),
      R7Vec2(x_position - 300, y_position + (90 - shrink)),
      RD7::Color::RGBA(.94f - .17f * color_effect, .61f - .25f * color_effect,
                       .36f + .38f * color_effect)
          .toRGBA());
}

void DrawWavedBg(R7Vec2 position, R7Vec2 size, float time) {
  UI7::GetBackgroundList()->AddRectangle(position, size, 0xff64c9fd);
  int i = 0;
  for (; i < 44; i++) Wave(i, position, size, time);
}
}  // namespace BP