#include <stdio.h>
#include <string.h>
#include <conio.h> // getch

#define ITERATIONS 10000

void resolve_grid(int grid[9][9]);
int get_single_possible_digit(int cell_possible_digits[9]);
void print_grid(int grid[9][9], int cross_index);

int main(int argc, char **argv)
{
    int grid[9][9];
    // Init grid
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            grid[i][j] = 0;

    // int grid[9][9] = {
    //     {0, 0, 0, 0, 0, 1, 2, 8, 0},
    //     {1, 7, 2, 3, 8, 0, 9, 4, 0},
    //     {4, 3, 0, 9, 6, 2, 7, 5, 0},
    //     {0, 1, 3, 6, 0, 4, 0, 0, 0},
    //     {0, 0, 0, 5, 3, 0, 4, 0, 0},
    //     {5, 0, 0, 0, 0, 0, 0, 0, 2},
    //     {0, 0, 0, 0, 0, 6, 3, 0, 0},
    //     {3, 8, 0, 0, 0, 0, 1, 0, 9},
    //     {0, 2, 5, 0, 9, 0, 0, 0, 4},
    // };

    printf("Bienvenue sur Sudoku Resolver!\n");
    printf("Entrez les chiffres apparents sur la grille ou 0 si la case est vide:\n");

    // Ask to complete the initial grid
    char c;
    int b_valid = 0;
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            print_grid(grid, y * 9 + x);
            // Ask for digit, repeat if user did not entered a digit.
            do
            {
                b_valid = 0;
                c = getch();
                if (c >= 48 && c <= 57) // 0-9 in ASCII
                {
                    grid[y][x] = c - 48; // ASCII digit to int
                    b_valid = 1;
                }
            } while (!b_valid);
        }
    }

    resolve_grid(grid);

    print_grid(grid, -1);

    return 0;
}

void resolve_grid(int grid[9][9])
{
    // A 9x9 grid with each possible digit on each cell.
    int possible_digits[9][9][9];
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            for (int k = 0; k < 9; k++)
                possible_digits[i][j][k] = 1;

    for (int i = 0; i < ITERATIONS; i++)
    {
        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {
                if (grid[y][x] != 0)
                    continue; // Already resolved this cell

                // Set possible digits for this cell

                // Remove same row digits
                for (int xx = 0; xx < 9; xx++)
                {
                    int digit = grid[y][xx];
                    if (digit != 0)
                        possible_digits[y][x][digit - 1] = 0;
                }

                // Remove column digits
                for (int yy = 0; yy < 9; yy++)
                {
                    int digit = grid[yy][x];
                    if (digit != 0)
                        possible_digits[y][x][digit - 1] = 0;
                }

                // Remove sub grid (3x3) digits
                int sub_grid_y = (y / 3) * 3; // y coord of the top left of the sub grid
                int sub_grid_x = (x / 3) * 3; // x coord of the top left of the sub grid
                for (int yy = sub_grid_y; yy < sub_grid_y + 3; yy++)
                {
                    for (int xx = sub_grid_x; xx < sub_grid_x + 3; xx++)
                    {
                        int digit = grid[yy][xx];
                        if (digit != 0)
                            possible_digits[y][x][digit - 1] = 0;
                    }
                }

                // Resolve this cell if there is only one possible digit
                int single_possible_digit = get_single_possible_digit(possible_digits[y][x]);
                if (single_possible_digit != 0)
                {
                    grid[y][x] = single_possible_digit;
                    continue;
                }

                // Check if this cell is the only one to have a possible digit in the sub grid (3x3)
                // eg. possible digits on the 3 remaining cells in a sub grid: 1 4, 5 1, 1 4. We know that second cell is 5.
                for (int p = 0; p < 9; p++)
                {
                    if (possible_digits[y][x][p] == 0)
                        continue;
                    int possible_digit = p + 1;
                    int b_unique_possible_digit = 1;

                    for (int yy = sub_grid_y; yy < sub_grid_y + 3; yy++)
                    {
                        for (int xx = sub_grid_x; xx < sub_grid_x + 3; xx++)
                        {
                            if (xx == x && yy == y)
                                continue; // skip current cell

                            if (grid[yy][xx] != 0)
                                continue; // skip resolved cell

                            if (possible_digits[yy][xx][possible_digit - 1] != 0)
                            {
                                // The cell on the sub grid has the same possible digit, skipping to next possible digit.
                                b_unique_possible_digit = 0;
                                break;
                            }
                        }
                        if (!b_unique_possible_digit)
                            break;
                    }

                    if (b_unique_possible_digit)
                    {
                        // This cell is the only one to have `possible_digit` in the sub grid, we can resolve it
                        grid[y][x] = possible_digit;
                        break;
                    }
                }
            }
        }
    }
}

int get_single_possible_digit(int cell_possible_digits[9])
{
    int single_possible_digit = 0;
    for (int p = 0; p < 9; p++)
    {
        if (cell_possible_digits[p] == 0)
            continue;

        if (single_possible_digit != 0)
            return 0; // There is more than one possible digit

        single_possible_digit = p + 1;
    }

    return single_possible_digit;
}

void print_grid(int grid[9][9], int cross_index)
{
    // // -------------------------
    // // | 1 2 3 | 4 5 6 | 7 8 9 |
    // // | 1 2 3 | 4 5 6 | 7 8 9 |
    // // | 1 2 3 | 4 5 6 | 7 8 9 |
    // // -------------------------
    // // | 1 2 3 | 4 5 6 | 7 8 9 |
    // // | 1 2 3 | 4 5 6 | 7 8 9 |
    // // | 1 2 3 | 4 5 6 | 7 8 9 |
    // // -------------------------
    // // | 1 2 3 | 4 5 6 | 7 8 9 |
    // // | 1 2 3 | 4 5 6 | 7 8 9 |
    // // | 1 2 3 | 4 5 6 | 7 8 9 |
    // // -------------------------

    char str[500];
    char digit_to_str[3]; // {'5',' ', '\0'}

    for (int y = 0; y < 9; y++)
    {
        if (y % 3 == 0)
            strcat(str, "\n-------------------------");

        for (int x = 0; x < 9; x++)
        {
            if (x == 0)
                strcat(str, "\n| ");
            else if (x % 3 == 0)
                strcat(str, "| ");

            // Print X or number or blank
            int flat_index = 9 * y + x;
            if (cross_index == flat_index)
                strcat(str, "X ");
            else if (grid[y][x] == 0)
                strcat(str, "  ");
            else
            {
                sprintf(digit_to_str, "%i ", grid[y][x]);
                strcat(str, digit_to_str);
            }

            if (x == 8)
                strcat(str, "|");
        }

        if (y == 8)
            strcat(str, "\n-------------------------\n");
    }

    // Print all at once
    printf(str);
}