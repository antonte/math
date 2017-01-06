#pragma once
#include "optional.hpp"
#include <SDL.h>
#include <string>

class Renderer;
class Texture
{
public:
  Texture(Renderer &, const std::string &);
  ~Texture();
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;
  SDL_Texture *operator&();
  const SDL_Texture *operator&() const;
  void draw(const std::optional<SDL_Rect> &dest, const std::optional<SDL_Rect> &src = std::nullopt) const;
  void draw(const std::optional<SDL_Rect> &dest, const std::optional<SDL_Rect> &src, double angle, const std::optional<SDL_Point> &center = std::nullopt, const SDL_RendererFlip flip = SDL_FLIP_NONE) const;
private:
  Renderer &renderer;
  SDL_Texture *handle;
};
