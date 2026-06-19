#include <ncurses.h>
#include <unistd.h>

void draw_interactive(int p1_y, int p2_y, int ball_x, int ball_y, int score1,
                      int score2) {
  clear(); // ncurses очистка экрана
  mvprintw(0, 30, "Player 1: %d | Player 2: %d", score1, score2);

  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 80; x++) {
      if (y == 0 || y == 24) {
        mvaddch(y + 1, x, '-'); // Сдвиг по Y на 1 из-за строки счета
      } else if (y == ball_y && x == ball_x) {
        mvaddch(y + 1, x, 'O');
      } else if (x == 2 && (y == p1_y - 1 || y == p1_y || y == p1_y + 1)) {
        mvaddch(y + 1, x, '|');
      } else if (x == 77 && (y == p2_y - 1 || y == p2_y || y == p2_y + 1)) {
        mvaddch(y + 1, x, '|');
      } else if (x == 39) {
        mvaddch(y + 1, x, '|');
      }
    }
  }
  refresh(); // Обновление кадра
}

int main() {
  // Инициализация ncurses
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE); // Не ждать ввода пользователя (для real-time)
  curs_set(0); // Скрыть курсор

  int p1_y = 12, p2_y = 12;
  int ball_x = 40, ball_y = 12;
  int ball_dx = -1, ball_dy = -1;
  int score1 = 0, score2 = 0;

  while (score1 < 21 && score2 < 21) {
    draw_interactive(p1_y, p2_y, ball_x, ball_y, score1, score2);

    // Считываем ВСЕ нажатия в буфере за этот кадр, чтобы управление было
    // плавным
    int input;
    while ((input = getch()) != ERR) {
      if ((input == 'A' || input == 'a') && p1_y > 2)
        p1_y--;
      else if ((input == 'Z' || input == 'z') && p1_y < 22)
        p1_y++;
      else if ((input == 'K' || input == 'k') && p2_y > 2)
        p2_y--;
      else if ((input == 'M' || input == 'm') && p2_y < 22)
        p2_y++;
    }

    // Физика мяча (обновляется независимо от нажатий клавиш)
    int next_x = ball_x + ball_dx;
    int next_y = ball_y + ball_dy;

    if (next_y == 0 || next_y == 24) {
      ball_dy = -ball_dy;
      next_y = ball_y + ball_dy;
    }

    if (next_x == 2 && (next_y >= p1_y - 1 && next_y <= p1_y + 1)) {
      ball_dx = -ball_dx;
      next_x = ball_x + ball_dx;
    } else if (next_x == 77 && (next_y >= p2_y - 1 && next_y <= p2_y + 1)) {
      ball_dx = -ball_dx;
      next_x = ball_x + ball_dx;
    }

    if (next_x <= 0) {
      score2++;
      ball_x = 40;
      ball_y = 12;
      ball_dx = 1;
      ball_dy = 1;
    } else if (next_x >= 79) {
      score1++;
      ball_x = 40;
      ball_y = 12;
      ball_dx = -1;
      ball_dy = 1;
    } else {
      ball_x = next_x;
      ball_y = next_y;
    }

    // Пауза, чтобы мяч не летал мгновенно
    usleep(100000);
  }

  // Завершение работы ncurses и вывод результата
  nodelay(stdscr, FALSE); // Теперь ждем нажатия, чтобы экран не закрылся сразу
  clear();
  if (score1 == 21) {
    mvprintw(12, 30, "Congratulations! Player 1 won!");
  } else {
    mvprintw(12, 30, "Congratulations! Player 1 won!");
  }
  mvprintw(14, 25, "Press any key to exit...");
  refresh();

  getch();  // Ждем нажатия
  endwin(); // Выход из режима ncurses

  return 0;
}