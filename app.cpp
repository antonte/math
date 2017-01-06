#include "app.hpp"
#include "level.hpp"
#include "menu_screen.hpp"
#include "sdl.hpp"
#include "select_level.hpp"

int App::exec()
{
  Sdl sdl(SDL_INIT_EVERYTHING);
  Window window("Math", Width, Height);
  Renderer renderer(window);
  SDL_SetRenderDrawBlendMode(&renderer, SDL_BLENDMODE_BLEND);
  for (;;)
  {
    switch (MenuScreen::exec(renderer))
    {
    case MenuScreen::Play:
      {
        int level = 0;
        for (;;)
        {
          level = SelectLevel::exec(renderer, level);
          if (level < 0)
            break;
          auto res = Level::exec(renderer, level);
          if (res >= 29)
            ++level;
        }
      }
      break;
    case MenuScreen::Quit:
      return 0;
    default:
      break;
    }
  }
  return 0;
}
