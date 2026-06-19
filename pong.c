#include <stdio.h>

void draw_field(int p1_y, int p2_y, int ball_x, int ball_y, int score1,
                int score2) {
  // Очистка терминала с помощью ANSI escape-последовательности (system()
  // запрещен)
  printf("\033[H\033[J");
  printf("        Игрок 1: %d                          Игрок 2: %d\n", score1,
         score2);

  // Поле 80 на 25
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 80; x++) {
      if (y == 0 || y == 24) {
        putchar('-'); // Верхняя и нижняя границы
      } else if (y == ball_y && x == ball_x) {
        putchar('O'); // Мяч (1 символ)
      } else if (x == 2 && (y == p1_y - 1 || y == p1_y || y == p1_y + 1)) {
        putchar('|'); // Ракетка левого игрока (3 символа)
      } else if (x == 77 && (y == p2_y - 1 || y == p2_y || y == p2_y + 1)) {
        putchar('|'); // Ракетка правого игрока (3 символа)
      } else if (x == 39) {
        putchar('|'); // Сетка по центру
      } else {
        putchar(' '); // Пустое пространство
      }
    }
    putchar('\n');
  }
}

int main() {
  // Начальные координаты ракеток (центр по Y)
  int p1_y = 12, p2_y = 12;
  // Начальные координаты мяча и его направление
  int ball_x = 40, ball_y = 12;
  int ball_dx = -1, ball_dy = -1;
  // Счет игры
  int score1 = 0, score2 = 0;

  // Главный игровой цикл (до 21 очка)
  while (score1 < 21 && score2 < 21) {
    draw_field(p1_y, p2_y, ball_x, ball_y, score1, score2);

    // Чтение ввода без указателей (без scanf)
    int input = getchar();
    if (input == EOF)
      break;
    if (input == '\n')
      continue;

    // Очистка остатка буфера до переноса строки,
    // чтобы одно нажатие Enter считалось ровно одним ходом.
    int temp = input;
    while (temp != '\n' && temp != EOF) {
      temp = getchar();
    }

    // Логика управления ракетками (только если ход валидный)
    int valid_turn = 0;
    if ((input == 'A' || input == 'a') && p1_y > 2) {
      p1_y--;
      valid_turn = 1;
    } else if ((input == 'Z' || input == 'z') && p1_y < 22) {
      p1_y++;
      valid_turn = 1;
    } else if ((input == 'K' || input == 'k') && p2_y > 2) {
      p2_y--;
      valid_turn = 1;
    } else if ((input == 'M' || input == 'm') && p2_y < 22) {
      p2_y++;
      valid_turn = 1;
    } else if (input == ' ') {
      valid_turn = 1;
    } // Пропуск хода

    // Если введен правильный символ, игра делает шаг (шаг мяча)
    if (valid_turn == 1) {
      int next_x = ball_x + ball_dx;
      int next_y = ball_y + ball_dy;

      // Отскок от верхней и нижней стен
      if (next_y == 0 || next_y == 24) {
        ball_dy = -ball_dy;
        next_y = ball_y + ball_dy;
      }

      // Отскок от ракеток
      if (next_x == 2 && (next_y >= p1_y - 1 && next_y <= p1_y + 1)) {
        ball_dx = -ball_dx;
        next_x = ball_x + ball_dx;
      } else if (next_x == 77 && (next_y >= p2_y - 1 && next_y <= p2_y + 1)) {
        ball_dx = -ball_dx;
        next_x = ball_x + ball_dx;
      }

      // Проверка забитого гола
      if (next_x <= 0) {
        score2++;
        ball_x = 40;
        ball_y = 12;
        ball_dx = 1; // Возвращаем мяч в центр
      } else if (next_x >= 79) {
        score1++;
        ball_x = 40;
        ball_y = 12;
        ball_dx = -1;
      } else {
        ball_x = next_x;
        ball_y = next_y;
      }
    }
  }

  // Вывод победителя
  printf("\033[H\033[J"); // Очистка перед финальным экраном
  if (score1 == 21) {
    printf("Поздравляем! Игрок 1 победил!\n");
  } else {
    printf("Поздравляем! Игрок 2 победил!\n");
  }
  return 0;
}