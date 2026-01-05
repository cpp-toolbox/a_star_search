#ifndef A_STAR_SEARCH_HPP
#define A_STAR_SEARCH_HPP

#include <optional>
#include <vector>
#include <numeric>

using Grid = std::vector<std::vector<int>>;
using Path = std::vector<std::pair<int, int>>;

struct Cell {
    int parent_i = 0;
    int parent_j = 0;

    double f = std::numeric_limits<double>::infinity();
    double g = std::numeric_limits<double>::infinity();
    double h = 0.0;
};

// returns in [col, row] tuples that map into the grid
// in the grid 0 means blocked (wall) and 1 means unblocked.
// we are returning a path of points whih is the path from the src to dest if it exists.
std::optional<std::vector<std::pair<int, int>>> a_star_search(const std::vector<std::vector<int>> &grid,
                                                              std::pair<int, int> src, std::pair<int, int> dest);

#endif
