#include "level.hpp"
#include "app.hpp"
#include "event_handler.hpp"
#include "rect.hpp"
#include "renderer.hpp"

static int generateNum(int level)
{
  auto res = 0;
  switch (level / 4)
  {
  case 0:
    res = res * 10 + rand() % 9 + 1;
    break;
  case 1:
    res = res * 10 + rand() % 2 + 1;
    res = res * 10 + rand() % 10;
    break;
  case 2:
    res = res * 10 + rand() % 5 + 1;
    res = res * 10 + rand() % 10;
    break;
  case 3:
    res = res * 10 + rand() % 9 + 1;
    res = res * 10 + rand() % 10;
    break;
  }
  return res;
}

int Level::exec(Renderer &r, int level)
{
  const auto &bg = r.texture("bg.bmp");
  const auto &digits = r.texture("digits.bmp");
  const auto &star = r.texture("star.bmp");
  const auto &correct = r.texture("correct.bmp");
  const auto &wrong = r.texture("wrong.bmp");
  std::string opFileName;
  switch (level % 4)
  {
  case 0:
    opFileName = "plus.bmp";
    break;
  case 1:
    opFileName = "minus.bmp";
    break;
  case 2:
    opFileName = "multiply.bmp";
    break;
  case 3:
    opFileName = "divide.bmp";
    break;
  }
  const auto &op = r.texture(opFileName);
  const auto TotalQuestions = 30;
  int questionN;
  auto startTime = SDL_GetTicks();
  int num1;
  int num2;
  int res;
  int score = 0;
  bool done;
  bool quit = false;
  bool isCorrect = true;
  Uint32 flashTime = 0xffffffffu;
  int selectedButton = -1;
  
  EventHandler e;
  e.draw = [&]()
    {
      SDL_SetRenderDrawColor(&r, 0xff, 0xff, 0xe0, 0xff);
      r.clear();
      bg.draw(makeRect(0, 0, App::Width, App::Height));
      auto sec = (SDL_GetTicks() - startTime) / 1000;
      auto x = 800 + 32 * 8;
      auto n = sec % 60;
      x -= 32;
      digits.draw(makeRect(x, 0, 32, 64), makeRect(n % 10 * 32, 0, 32, 64));
      x -= 32;
      digits.draw(makeRect(x, 0, 32, 64), makeRect(n / 10 * 32, 0, 32, 64));
      x -= 32;
      x -= 32;
      n = sec / 60 % 60;
      digits.draw(makeRect(x, 0, 32, 64), makeRect(n % 10 * 32, 0, 32, 64));
      x -= 32;
      digits.draw(makeRect(x, 0, 32, 64), makeRect(n / 10 * 32, 0, 32, 64));
      x -= 32;
      x -= 32;
      n = sec / 60 / 60 % 60;
      digits.draw(makeRect(x, 0, 32, 64), makeRect(n % 10 * 32, 0, 32, 64));
      x -= 32;
      digits.draw(makeRect(x, 0, 32, 64), makeRect(n / 10 * 32, 0, 32, 64));
      x -= 32;
      x = 464;
      for (auto n = TotalQuestions; n > 0; n /= 10)
      {
        digits.draw(makeRect(x, 0, 32, 64), makeRect(n % 10 * 32, 0, 32, 64));
        x -= 32;
      }
      x = 393;
      for (auto n = questionN; n > 0; n /= 10)
      {
        x -= 32;
        digits.draw(makeRect(x, 0, 32, 64), makeRect(n % 10 * 32, 0, 32, 64));
      }
      op.draw(makeRect(655 - 64, 227 - 64 / 2, 64, 64));
      x = App::Width - 32;
      for (auto n = num1; n > 0; n /= 10)
      {
        x -= 32;
        digits.draw(makeRect(x, 64 + 50, 32, 64), makeRect(n % 10 * 32, 0, 32, 64));
      }
      x = App::Width - 32;
      for (auto n = num2; n > 0; n /= 10)
      {
        x -= 32;
        digits.draw(makeRect(x, 224 + 50, 32, 64), makeRect(n % 10 * 32, 0, 32, 64));
      }
      x = App::Width - 32;
      for (auto n = res; n > 0; n /= 10)
      {
        x -= 32;
        digits.draw(makeRect(x, 389 + 50, 32, 64), makeRect(n % 10 * 32, 0, 32, 64));
      }
      x = App::Width - 4 * 32;
      int stars = 0;
      if (score >= TotalQuestions - 1)
        stars = 3;
      else if (score >= TotalQuestions * 3 / 4)
        stars = 2;
      else if (score >= TotalQuestions / 2)
        stars = 1;
      for (auto i = 0; i < stars; ++i)
      {
        star.draw(makeRect(x, 16, 32, 32));
        x += 32;
      }
      if (selectedButton >= 0 && (selectedButton % 4 != 3 || selectedButton == 15))
      {
        SDL_SetRenderDrawColor(&r, 0x0, 0x40, 0x0, 0x40);
        SDL_Rect tmp;
        if (selectedButton == 12 || selectedButton == 13)
          tmp = makeRect(12 % 4 * 164, 12 / 4 * 164 + 64, 164 * 2, 164);
        else
          tmp = makeRect(selectedButton % 4 * 164, selectedButton / 4 * 164 + 64, 164, 164);
        SDL_RenderFillRect(&r, &tmp);
      }
      if (flashTime != 0xffffffffu)
      {
        if (SDL_GetTicks() < flashTime)
        {
          SDL_SetRenderDrawColor(&r, 0x0, 0x0, 0x0, 0x40);
          auto tmp = makeRect(0, 0, App::Width, App::Height);
          SDL_RenderFillRect(&r, &tmp);
          if (isCorrect)
            correct.draw(makeRect(0, 0, App::Width, App::Height));
          else
            if (SDL_GetTicks() % 250 < 125)
              wrong.draw(makeRect(0, 0, App::Width, App::Height));
        }
        else
        {
          flashTime = 0xffffffffu;
          done = true;
        }
      }
      r.present();
    };
  e.buttonDown = [&](const SDL_MouseButtonEvent &e)
    {
      selectedButton = e.x / 164 + (e.y - 64) / 164 * 4;
    };
  auto buttonEvent = [&](int idx)
    {
      switch (idx)
      {
      case 0:
        res = res * 10 + 7;
        break;
      case 1:
        res = res * 10 + 8;
        break;
      case 2:
        res = res * 10 + 9;
        break;
      case 3:
        break;
      case 4:
        res = res * 10 + 4;
        break;
      case 5:
        res = res * 10 + 5;
        break;
      case 6:
        res = res * 10 + 6;
        break;
      case 7:
        break;
      case 8:
        res = res * 10 + 1;
        break;
      case 9:
        res = res * 10 + 2;
        break;
      case 10:
        res = res * 10 + 3;
        break;
      case 11:
        break;
      case 12:
      case 13:
        res = res * 10 + 0;
        break;
      case 14:
        res /= 10;
        break;
      case 15:
        flashTime = SDL_GetTicks() + 1500;
        switch (level % 4)
        {
        case 0:
          isCorrect = (res == num1 + num2);
          break;
        case 1:
          isCorrect = (res == num1 - num2);
          break;
        case 2:
          isCorrect = (res == num1 * num2);
          break;
        case 3:
          isCorrect = (res == num1 / num2);
          break;
        }
        if (isCorrect)
          ++score;
        break;
      }
      if (res > 100000000)
        res /= 10;
    };
  e.buttonUp = [&](const SDL_MouseButtonEvent &e)
    {
      selectedButton = -1;
      if (flashTime != 0xffffffffu && SDL_GetTicks() < flashTime)
        return;
      buttonEvent(e.x / 164 + (e.y - 64) / 164 * 4);
      if (e.x > App::Width - 64 && e.y > App::Height - 64)
      {
        quit = true;
        done = true;
      }
    };
  e.keyUp = [&](const SDL_KeyboardEvent &e)
    {
      selectedButton = -1;
    };
  e.keyDown = [&](const SDL_KeyboardEvent &e)
    {
      switch (e.keysym.sym)
      {
      case SDLK_0:
      case SDLK_KP_0:
        selectedButton = 12;
        buttonEvent(selectedButton);
        break;
      case SDLK_1:
      case SDLK_KP_1:
        selectedButton = 8;
        buttonEvent(selectedButton);
        break;
      case SDLK_2:
      case SDLK_KP_2:
        selectedButton = 9;
        buttonEvent(selectedButton);
        break;
      case SDLK_3:
      case SDLK_KP_3:
        selectedButton = 10;
        buttonEvent(selectedButton);
        break;
      case SDLK_4:
      case SDLK_KP_4:
        selectedButton = 4;
        buttonEvent(selectedButton);
        break;
      case SDLK_5:
      case SDLK_KP_5:
        selectedButton = 5;
        buttonEvent(selectedButton);
        break;
      case SDLK_6:
      case SDLK_KP_6:
        selectedButton = 6;
        buttonEvent(selectedButton);
        break;
      case SDLK_7:
      case SDLK_KP_7:
        selectedButton = 0;
        buttonEvent(selectedButton);
        break;
      case SDLK_8:
      case SDLK_KP_8:
        selectedButton = 1;
        buttonEvent(selectedButton);
        break;
      case SDLK_9:
      case SDLK_KP_9:
        selectedButton = 2;
        buttonEvent(selectedButton);
        break;
      case SDLK_KP_BACKSPACE:
      case SDLK_BACKSPACE:
        selectedButton = 14;
        buttonEvent(selectedButton);
        break;
      case SDLK_KP_ENTER:
      case SDLK_RETURN:
      case SDLK_RETURN2:
        selectedButton = 15;
        buttonEvent(selectedButton);
        break;
      }
    };
  e.quit = [&]()
    {
      quit = true;
    };

  for (questionN = 1; questionN <= TotalQuestions; ++questionN)
  {
    num1 = generateNum(level);
    num2 = generateNum(level);
    res = 0;
    switch (level % 4)
    {
    case 0:
    case 2:
      break;
    case 1:
      num1 = num2 + num1;
      break;
    case 3:
      num1 = num2 * num1;
      break;
    }
    e.exec(done);
    if (quit)
      break;
  }
  
  return score;
}
