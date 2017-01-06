#pragma once
#include "texture.hpp"
#include <SDL.h>
#include <memory>
#include <string>
#include <unordered_map>

class Window;
class Renderer
{
public:
  Renderer(Window &, Uint32 flags = 0, int index = -1);
  ~Renderer();
  SDL_Renderer *operator&();
  const SDL_Renderer *operator&() const;
  void clear();
  void drawCircle(float x, float y, float r);
  Texture &texture(const std::string &);
  void present();
private:
  SDL_Renderer *handle;
  std::unordered_map<std::string, std::unique_ptr<Texture> > textures;
};
