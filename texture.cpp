#include "texture.hpp"
#include "renderer.hpp"
#include <stdexcept>

static SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &fileName)
{
  auto surface = SDL_LoadBMP(fileName.c_str());
  if (!surface)
  {
    throw std::runtime_error(std::string("SDL_LoadBMP(") + fileName + "): " + SDL_GetError());
  }
  auto res = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return res;
}

Texture::Texture(Renderer &renderer, const std::string &fileName):
  renderer(renderer),
  handle(loadTexture(&renderer, fileName))
{
}
         
Texture::~Texture()
{
  SDL_DestroyTexture(handle);
}
         
SDL_Texture *Texture::operator&()
{
  return handle;
}
         
const SDL_Texture *Texture::operator&() const
{
  return handle;
}

void Texture::draw(const std::optional<SDL_Rect> &dest, const std::optional<SDL_Rect> &src) const
{
  SDL_RenderCopy(&renderer, handle, src ? &(*src) : nullptr, dest ? &(*dest) : nullptr);
}

void Texture::draw(const std::optional<SDL_Rect> &dest, const std::optional<SDL_Rect> &src, double angle, const std::optional<SDL_Point> &center, const SDL_RendererFlip flip) const
{
  SDL_RenderCopyEx(&renderer, handle, src ? &(*src) : nullptr, dest ? &(*dest) : nullptr, angle, center ? &(*center) : nullptr, flip);
}
