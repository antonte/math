#include "sdl.hpp"
#include <stdexcept>
#include <string>
#include <sstream>

Sdl::Sdl(Uint32 flags)
{
  auto res = SDL_Init(flags);
  if (res != 0)
  {
    std::ostringstream strm;
    strm << "Sdl::Sdl(" << flags << "): " << res << " " << SDL_GetError();
    throw std::runtime_error(strm.str());
  }
}
Sdl::~Sdl()
{
  SDL_Quit();
}
