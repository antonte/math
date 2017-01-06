#include "select_level.hpp"
#include "app.hpp"
#include "event_handler.hpp"
#include "rect.hpp"
#include "renderer.hpp"

int SelectLevel::exec(Renderer &r, int level)
{
  const auto &plus = r.texture("plus.bmp");
  const auto &minus = r.texture("minus.bmp");
  const auto &multiply = r.texture("multiply.bmp");
  const auto &divide = r.texture("divide.bmp");
  const auto &digits = r.texture("digits.bmp");
  const auto &go = r.texture("go.bmp");
  const auto &esc = r.texture("esc.bmp");
  EventHandler e;
  bool done;
  e.draw = [&]()
    {
      SDL_SetRenderDrawColor(&r, 0xff, 0xff, 0xef, 0xff);
      r.clear();
      plus.draw(makeRect(App::Width / 2 - 128, App::Height / 2 - 128, 128, 128));
      minus.draw(makeRect(App::Width / 2, App::Height / 2 - 128, 128, 128));
      multiply.draw(makeRect(App::Width / 2 - 128, App::Height / 2, 128, 128));
      divide.draw(makeRect(App::Width / 2, App::Height / 2, 128, 128));
      auto x = 0;
      for (int i: { 10, 30, 60, 100 })
      {
        auto xx = (x++ + 1) * App::Width / 4 - 32 - 64;
        for (auto n = i; n > 0; n /= 10)
        {
          digits.draw(makeRect(xx, 64, 32, 64), makeRect(n % 10 * 32, 0, 32, 64));
          xx -= 32;
        }
      }
      if (level >= 0)
      {
        SDL_SetRenderDrawColor(&r, 0x0, 0x0, 0x0, 0x20);
        auto x = (level % 4) % 2 * 128 + App::Width / 2 - 128;
        auto y = (level % 4) / 2 * 128 + App::Height / 2 - 128;
        auto tmp = makeRect(x, y, 128, 128);
        SDL_RenderFillRect(&r, &tmp);
        tmp = makeRect((level / 4) * App::Width / 4, 64, App::Width / 4, 64);
        SDL_RenderFillRect(&r, &tmp);
        go.draw(makeRect(App::Width - 64 - 128, App::Height - 64 - 128, 128, 128));
      }
      esc.draw(makeRect(0, App::Height - 64, 64, 64));
      r.present();
    };
  e.buttonUp = [&](const SDL_MouseButtonEvent &e)
    {
      if (e.x < 64 && e.y > App::Height - 64)
      {
        done = true;
        level = -1;
      }
      for (int l = 0; l < 4; ++l)
      {
        auto x = l % 2;
        auto y = l / 2;
        if (e.x > x * 128 + App::Width / 2 - 128 && e.x < (x + 1) * 128 + App::Width / 2 - 128 &&
            e.y > y * 128 + App::Height / 2 - 128 && e.y < (y + 1) * 128 + App::Height / 2 - 128)
        {
          if (level < 0)
            level = 0;
          level = level / 4 * 4 + l;
          break;
        }
      }
      for (int l = 0; l < 4; ++l)
      {
        auto x = l * App::Width / 4;
        auto y = 64;
        if (e.x > x && e.x < x + App::Width / 4 &&
            e.y > y && e.y < y + 64)
        {
          if (level < 0)
            level = 0;
          level = l * 4 + level % 4;
          break;
        }
      }
      if (level >= 0)
      {
        if (e.x > App::Width - 64 - 128 && e.x < App::Width - 64 &&
            e.y > App::Height - 64 - 128 && e.y < App::Height - 64)
        {
          done = true;
        }
      }
    };
  e.quit = [&]()
    {
      level = -1;
    };
  e.exec(done);
  return level;
}
