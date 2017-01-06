#include "renderer.hpp"
#include "window.hpp"
#include <stdexcept>
#include <sstream>
#include <vector>

Renderer::Renderer(Window &w, Uint32 flags, int index)
{
  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
  handle = SDL_CreateRenderer(&w, index, flags);
  if (!handle)
  {
    std::ostringstream strm;
    strm << "Renderer::Renderer(" << &w << ", " << flags << ", " << index << "): " << SDL_GetError();
    throw std::runtime_error(strm.str());
  }
}
Renderer::~Renderer()
{
  SDL_DestroyRenderer(handle);
}
SDL_Renderer *Renderer::operator&()
{
  return handle;
}
const SDL_Renderer *Renderer::operator&() const
{
  return handle;
}

void Renderer::drawCircle(float x, float y, float r)
{
  SDL_Rect rect;
  SDL_RenderGetViewport(handle, &rect);
  std::vector<SDL_Point> lines;
  for (int xx = std::max(-r, rect.x - x); xx < std::min(r, rect.x + rect.w - x); ++xx)
  {
    auto yy = sqrt(r * r - xx * xx);
    auto y1 = y - yy;
    auto y2 = y + yy;
    if (y1 < rect.y)
      y1 = rect.y;
    if (y2 > rect.h + rect.y)
      y2 = rect.h + rect.y;
    if (y1 > rect.h + rect.y)
      continue;
    if (y2 < rect.y)
      continue;
    if (xx % 2 == 0)
    {
      SDL_Point p1;
      p1.x = x + xx;
      p1.y = y1;
      SDL_Point p2;
      p2.x = x + xx;
      p2.y = y2;
      lines.push_back(p1);
      lines.push_back(p2);
    }
    else
    {
      SDL_Point p1;
      p1.x = x + xx;
      p1.y = y1;
      SDL_Point p2;
      p2.x = x + xx;
      p2.y = y2;
      lines.push_back(p2);
      lines.push_back(p1);
    }
  }
  SDL_RenderDrawLines(handle, lines.data(), lines.size());
}

Texture &Renderer::texture(const std::string &fileName)
{
  auto iter = textures.find(fileName);
  if (iter == std::end(textures))
  {
    auto tmp = textures.insert(std::make_pair(fileName, std::make_unique<Texture>(*this, fileName)));
    iter = tmp.first;
  }
  return *iter->second.get();
}

void Renderer::present()
{
  SDL_RenderPresent(handle);
}

void Renderer::clear()
{
  SDL_RenderClear(handle);
}
