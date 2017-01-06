#include "window.hpp"
#include <sstream>
#include <stdexcept>

Window::Window(const std::string &title,
         int w, int h,
         Uint32 flags,
         int x, int y)
{
  handle = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
  if (!handle)
  {
    std::ostringstream strm;
    strm << "Window::Window(\"" << title << "\", " << w << ", " << h << ", " << flags << ", " << x << ", " << y << "): " << SDL_GetError();
    throw std::runtime_error(strm.str());
  }
}

Window::~Window()
{
  SDL_DestroyWindow(handle);
}

SDL_Window *Window::operator&()
{
  return handle;
}

const SDL_Window *Window::operator&() const
{
  return handle;
}
