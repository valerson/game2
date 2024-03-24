#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#define H 25
#define W 80
#define TIME 1
#define VAMPIRE_FIN 700

void input_start_matrix(char **matrix);                          // ������������ ��������� ������� �� �����
void output_matrix(char **matrix);                               // ������ �������
void step_of_life(char **matrix, char **next_matrix);            //  ������������ ������� �� ��������� ���
int check_moore_locality(char **matrix, int i, int j);           /* �������� ����������� ����
                                                                  (��� ������ ������� ������)*/
int check_matrix(char **matrix, char **next_matrix, int *count); // ��������� ��������� ������� � �������
void new_start_matrix(char **matrix, char **next_matrix);        // ������������ ��������� ������� ������� �������
void finish_pic(void);
void start_pic(void);
void text_in_start_pic(void);
void inic_ncurses(void);
void vampire_pic(void);

int main(void)
{
    int death = 0, count = 0;
    char start_key, end_key, inp_time;
    inic_ncurses(); // ������ ���� ��������� ����
    // ������ ������� - ���� - ��� ������� ������� � ������� ����� - ��� ���������� ���� �� ��������� ���
    char **pole = malloc(H * sizeof(char *));
    for (int i = 0; i < H; i += 1)
    {
        pole[i] = malloc(W * sizeof(char));
    }

    char **next_pole = malloc(H * sizeof(char *));
    for (int i = 0; i < H; i += 1)
    {
        next_pole[i] = malloc(W * sizeof(char));
    }

    clear();

    input_start_matrix(pole);
    start_pic();
    do
    {
        start_key = getch();
    } while (start_key != ' ');

    clear();
    output_matrix(pole);
    attrset(COLOR_PAIR(4)); // ���������� �����
    mvprintw(10, 5, "\t\tThis is the start field from the file you uploaded.\t\t");
    mvprintw(12, 5, "\t\tSelect the speed (from 1-fast to 3-slow) and let's start.\t\t");
    attrset(COLOR_PAIR(1)); // ���������� �����

    do
    {
        inp_time = getch();
    } while (inp_time != '1' && inp_time != '2' && inp_time != '3');

    clear();
    halfdelay(TIME * ((int)inp_time - 48)); // ��������� ������� �������� ����� - ����� ��������
    do
    {
        output_matrix(pole);

        getch(); // ��� ����� ����������� �������� halfdaley
        clear();

        step_of_life(pole, next_pole);

        death = check_matrix(pole, next_pole, &count);

        output_matrix(next_pole);
        clear();
        new_start_matrix(pole, next_pole);

        if (count == VAMPIRE_FIN)
        {
            death = 1;
        }

    } while (death == 0);
    clear();
    if (count != VAMPIRE_FIN)
    {
        finish_pic();
        do
        {
            end_key = getch();
        } while (end_key != 27); // 27 -��� ������ Esc
        clear();
    }
    else
    {
        vampire_pic();
        do
        {
            end_key = getch();
        } while (end_key != 27); // 27 -��� ������ Esc}
        clear();
    }

    for (int i = 0; i < H; i += 1)
    {
        free(pole[i]);
        free(next_pole[i]);
    }
    free(pole);
    free(next_pole);

    endwin(); // ����� �� ncurses
    return 0;
}

void input_start_matrix(char **matrix)
{
    char temp;

    for (int i = 0; i < H; i += 1)
    {
        for (int j = 0; j < W; j += 1)
        {
            scanf("%c", &temp);
            if (temp == '1')
            {
                matrix[i][j] = '#';
            }
            else if (temp == '0')
            {
                matrix[i][j] = ' ';
            }
        }
    }
    freopen("/dev/tty", "r", stdin);
}

void output_matrix(char **matrix)
{
    for (int i = 0; i < H; i += 1)
    {
        for (int j = 0; j < W; j += 1)
        {
            printw("%c", matrix[i][j]);
        }
        printw("\n");
    }
}

void step_of_life(char **matrix, char **next_matrix)
{
    for (int i = 0; i < H; i += 1)
    {
        for (int j = 0; j < W; j += 1)
        {
            int check = check_moore_locality(matrix, i, j); // ���� ��� �������� � ����� ������ ����������
            if (matrix[i][j] == ' ')
            {
                // ������ �������
                if (check == 3)
                {
                    // ������ ���������
                    next_matrix[i][j] = '#';
                }
                else
                {
                    // ������ ������
                    next_matrix[i][j] = ' ';
                }
            }
            else if (matrix[i][j] == '#')
            {
                // ������ �����
                if (check == 2 || check == 3)
                {
                    // ������ �������� �����
                    next_matrix[i][j] = '#';
                }
                else
                {
                    next_matrix[i][j] = ' ';
                }
            }
        }
    }
}

int check_moore_locality(char **matrix, int i, int j)
{
    int moore = 0;

    if (matrix[(i + 1) % H][j] == '#')
    {
        moore += 1;
    }
    if (matrix[(H + i - 1) % H][j] ==
        '#')
    { // H+i-1 - ��� � ����� ��� ���� ���� ���� ������, �� ����� ������� �� H ��� �� W
        moore += 1;
    }
    if (matrix[i][(j + 1) % W] == '#')
    {
        moore += 1;
    }
    if (matrix[i][(W + j - 1) % W] == '#')
    {
        moore += 1;
    }
    if (matrix[(i + 1) % H][(j + 1) % W] == '#')
    {
        moore += 1;
    }
    if (matrix[(i + 1) % H][(W + j - 1) % W] == '#')
    {
        moore += 1;
    }
    if (matrix[(H + i - 1) % H][(j + 1) % W] == '#')
    {
        moore += 1;
    }
    if (matrix[(H + i - 1) % H][(W + j - 1) % W] == '#')
    {
        moore += 1;
    }

    return moore;
}

int check_matrix(char **matrix, char **next_matrix, int *count)
{
    int result = 1;
    for (int i = 0; i < H; i += 1)
    {
        for (int j = 0; j < W; j += 1)
        {
            if (matrix[i][j] != next_matrix[i][j])
            {
                result = 0;
                break;
            }
        }
        if (result == 0)
        {
            *count += 1;
            break;
        }
    }
    // printw("result=%d", result);
    return result;
}

void new_start_matrix(char **matrix, char **next_matrix)
{
    for (int i = 0; i < H; i += 1)
    {
        for (int j = 0; j < W; j += 1)
        {
            matrix[i][j] = next_matrix[i][j];
        }
    }
}

void finish_pic(void)
{
    char temp;
    char **finish = malloc(H * sizeof(char *));
    for (int i = 0; i < H; i += 1)
    {
        finish[i] = malloc(W * sizeof(char));
    }

    FILE *file = fopen("finish-pic.txt", "r");
    if (!file)
    {
        attrset(COLOR_PAIR(2)); // ���������� �����
        printw("No file, sorry. There will be no beautiful picture at the end./n");
        exit(1);
    }

    for (int i = 0; i < H; i += 1)
    {
        for (int j = 0; j < W; j += 1)
        {
            fscanf(file, "%c", &temp);
            if (temp == '1')
            {
                finish[i][j] = '#';
            }
            else
            {
                finish[i][j] = ' ';
            }
            attrset(COLOR_PAIR(4)); // ���������� �����
            printw("%c", finish[i][j]);
        }
        printw("\n");
    }
    attrset(COLOR_PAIR(1)); // ���������� �����
    mvprintw(20, 25, "To finish press \"ESC\"");

    fclose(file);

    for (int i = 0; i < H; i += 1)
    {
        free(finish[i]);
    }
    free(finish);
}

void inic_ncurses(void)
{
    initscr();   // ������������ ncurses
    noecho();    // ������� ��������� ������� ����� �����
    curs_set(0); // ������� �� ����� ������������

    // �������� �������
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
}

void start_pic(void)
{
    char temp;
    char **start = malloc(H * sizeof(char *));
    for (int i = 0; i < H; i += 1)
    {
        start[i] = malloc(W * sizeof(char));
    }

    FILE *file = fopen("start-pic.txt", "r");
    if (!file)
    {
        attrset(COLOR_PAIR(2)); // ���������� �����
        printw("No file, sorry. There will be no beautiful picture at the end./n");
        exit(1);
    }

    for (int i = 0; i < H; i += 1)
    {
        for (int j = 0; j < W; j += 1)
        {
            fscanf(file, "%c", &temp);
            if (temp == '1')
            {
                start[i][j] = '#';
            }
            else
            {
                start[i][j] = ' ';
            }

            attrset(COLOR_PAIR(4)); // ���������� �����
            printw("%c", start[i][j]);
            attrset(COLOR_PAIR(1)); // ���������� �����
        }
        printw("\n");
    }
    text_in_start_pic();
    fclose(file);

    for (int i = 0; i < H; i += 1)
    {
        free(start[i]);
    }
    free(start);
}

void text_in_start_pic(void)
{
    attrset(COLOR_PAIR(1)); // ���������� �����
    mvprintw(13, 25, "To start the game press \"SPACE\"");
    attrset(COLOR_PAIR(1)); // ���������� �����

    mvprintw(17, 5, "TYAP-LYAP EDITION MART 2024");
    mvprintw(19, 5, "TEAM:");
    mvprintw(20, 5, "LEAD - ROGERFAW");
    mvprintw(21, 5, "GLOSSUVI");
    mvprintw(22, 5, "FROSTDRA");
    attrset(COLOR_PAIR(5)); // ���������� �����
    mvprintw(24, 20, "ORIGINAL IDEA -  JOHN HORTON CONWAY - RIP");
    attrset(COLOR_PAIR(1)); // ���������� �����
}

void vampire_pic(void)
{
    char temp;
    char **vampi = malloc(H * sizeof(char *));
    for (int i = 0; i < H; i += 1)
    {
        vampi[i] = malloc(W * sizeof(char));
    }
    FILE *file = fopen("vampiro-pic.txt", "r");
    if (!file)
    {
        attrset(COLOR_PAIR(2)); // ���������� �����
        printw("No file, sorry. There will be no beautiful picture at the end./n");
        exit(1);
    }
    for (int i = 0; i < H; i += 1)
    {
        for (int j = 0; j < W; j += 1)
        {
            fscanf(file, "%c", &temp);
            if (temp == '1')
            {
                vampi[i][j] = '#';
            }
            else
            {
                vampi[i][j] = ' ';
            }

            if (i > 13 && j > 51)
            {
                attrset(COLOR_PAIR(1)); // ���������� �����
                printw("%c", vampi[i][j]);
            }
            else
            {
                attrset(COLOR_PAIR(2)); // ���������� �����
                printw("%c", vampi[i][j]);
                attrset(COLOR_PAIR(1)); // ���������� �����
            }
        }
        printw("\n");
    }
    attrset(COLOR_PAIR(1)); // ���������� �����
    mvprintw(23, 25, "To finish press \"ESC\"");
    fclose(file);
    for (int i = 0; i < H; i += 1)
    {
        free(vampi[i]);
    }
    free(vampi);
}
