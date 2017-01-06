#include "rect.hpp"

SDL_Rect makeRect(int x, int y, int w, int h)
{
  SDL_Rect res;
  res.x = x;
  res.y = y;
  res.w = w;
  res.h = h;
  return res;
}
