#include "a_star_search.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>

bool is_within_boundaries_of_grid(int col, int row, int col_count, int row_count) {
    return row >= 0 && row < row_count && col >= 0 && col < col_count;
}

bool is_unblocked(const std::vector<std::vector<int>> &grid, int col, int row) { return grid[row][col] == 1; }

bool is_destination(int col, int row, const std::pair<int, int> &dest) {
    return col == dest.first && row == dest.second;
}

double calculate_h_value(int col, int row, const std::pair<int, int> &dest) {
    return std::sqrt((col - dest.first) * (col - dest.first) + (row - dest.second) * (row - dest.second));
}

Path trace_path(const std::vector<std::vector<Cell>> &cell_details, std::pair<int, int> dest) {
    Path path;
    int col = dest.first;
    int row = dest.second;

    while (!(cell_details[row][col].parent_i == row && cell_details[row][col].parent_j == col)) {
        path.emplace_back(col, row);
        int temp_row = cell_details[row][col].parent_i;
        int temp_col = cell_details[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    path.emplace_back(col, row);
    std::reverse(path.begin(), path.end());
    return path;
}

std::optional<std::vector<std::pair<int, int>>> a_star_search(const std::vector<std::vector<int>> &grid,
                                                              std::pair<int, int> src, std::pair<int, int> dest) {

    const int num_rows = static_cast<int>(grid.size());
    // precondition, grid must have at least one row.
    const int num_cols = static_cast<int>(grid[0].size());

    if (!is_within_boundaries_of_grid(src.first, src.second, num_cols, num_rows) ||
        !is_within_boundaries_of_grid(dest.first, dest.second, num_cols, num_rows)) {
        std::cout << "Source or destination is invalid\n";
        return std::nullopt;
    }

    if (!is_unblocked(grid, src.first, src.second) || !is_unblocked(grid, dest.first, dest.second)) {
        std::cout << "Source or destination is blocked\n";
        return std::nullopt;
    }

    if (is_destination(src.first, src.second, dest)) {
        std::cout << "We are already at the destination\n";
        return Path{src};
    }

    std::vector<std::vector<bool>> closed_list(num_rows, std::vector<bool>(num_cols, false));

    std::vector<std::vector<Cell>> cell_details(num_rows, std::vector<Cell>(num_cols));

    int j = src.first;
    int i = src.second;

    cell_details[i][j].f = 0.0;
    cell_details[i][j].g = 0.0;
    cell_details[i][j].h = 0.0;
    cell_details[i][j].parent_i = i;
    cell_details[i][j].parent_j = j;

    using Node = std::tuple<double, int, int>;
    std::priority_queue<Node, std::vector<Node>, std::greater<>> open_list;

    open_list.emplace(0.0, i, j);

    const std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0},  {-1, 0},
                                                         {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    while (!open_list.empty()) {
        auto [f, i, j] = open_list.top();
        open_list.pop();

        closed_list[i][j] = true;

        for (auto [di, dj] : directions) {
            int new_i = i + di;
            int new_j = j + dj;

            if (is_within_boundaries_of_grid(new_j, new_i, num_cols, num_rows) && is_unblocked(grid, new_j, new_i) &&
                !closed_list[new_i][new_j]) {

                if (is_destination(new_j, new_i, dest)) {
                    cell_details[new_i][new_j].parent_i = i;
                    cell_details[new_i][new_j].parent_j = j;
                    return trace_path(cell_details, dest);
                }

                double g_new = cell_details[i][j].g + 1.0;
                double h_new = calculate_h_value(new_j, new_i, dest);
                double f_new = g_new + h_new;

                if (cell_details[new_i][new_j].f == std::numeric_limits<double>::infinity() ||
                    cell_details[new_i][new_j].f > f_new) {

                    open_list.emplace(f_new, new_i, new_j);
                    cell_details[new_i][new_j].f = f_new;
                    cell_details[new_i][new_j].g = g_new;
                    cell_details[new_i][new_j].h = h_new;
                    cell_details[new_i][new_j].parent_i = i;
                    cell_details[new_i][new_j].parent_j = j;
                }
            }
        }
    }

    std::cout << "Failed to find the destination cell.\n";
    return std::nullopt;
}
