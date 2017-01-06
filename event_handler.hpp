#pragma once
#include <functional>
#include <SDL.h>

class EventHandler
{
public:
  std::function<void()> tick;
  std::function<void()> draw;
  std::function<void(const SDL_MouseMotionEvent &)> motion;
  std::function<void(const SDL_MouseButtonEvent &)> buttonUp;
  std::function<void(const SDL_MouseButtonEvent &)> buttonDown;
  std::function<void(const SDL_KeyboardEvent &)> keyDown;
  std::function<void(const SDL_KeyboardEvent &)> keyUp;
  std::function<void()> quit;
  int exec(bool &done);
};
