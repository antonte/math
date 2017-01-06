#pragma once
#include <SDL.h>
#include <string>

class Window
{
public:
  Window(const std::string &title,
         int w, int h,
         Uint32 flags = 0,
         int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED);
  ~Window();
  SDL_Window *operator&();
  const SDL_Window *operator&() const;
private:
  SDL_Window *handle;
};
