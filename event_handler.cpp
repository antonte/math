#include "event_handler.hpp"
#include <SDL.h>

int EventHandler::exec(bool &done)
{
  auto currentTime = SDL_GetTicks();
  for (done = false; !done;)
  {
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
      switch (e.type)
      {
      case SDL_KEYDOWN:
        if (keyDown)
          keyDown(e.key);
        break;
      case SDL_KEYUP:
        if (keyUp)
          keyUp(e.key);
        break;
      case SDL_MOUSEBUTTONUP:
        if (buttonUp)
          buttonUp(e.button);
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (buttonDown)
          buttonDown(e.button);
        break;
      case SDL_MOUSEMOTION:
        if (motion)
          motion(e.motion);
        break;
      case SDL_QUIT:
        if (quit)
          quit();
        done = true;
        break;
      }
    }
    auto oldTime = currentTime;
    currentTime = SDL_GetTicks();
    if (tick)
      for (; oldTime < currentTime; ++oldTime)
        tick();
    if (draw)
      draw();
  }
  return 0;
}
