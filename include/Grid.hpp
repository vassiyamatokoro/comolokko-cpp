#pragma once

#include <cstddef>
#include <vector>
#include <raylib.h>

enum class CellState {
    Empty,
    Blocked,
    Active
};

struct GridConfig {
    int rows{6};
    int cols{6};
    int cellSize{72};
    int padding{6};
    int offsetX{270};
    int offsetY{140};
    Color cellColor{255, 252, 245, 255};
    Color borderColor{180, 165, 145, 255};
};

class Grid {
public:
    explicit Grid(const GridConfig& config);

    void reset();
    void draw() const;
    bool isInside(int row, int col) const noexcept;
    CellState getCell(int row, int col) const;
    bool setCell(int row, int col, CellState state);

    int rows() const noexcept { return config_.rows; }
    int cols() const noexcept { return config_.cols; }

private:
    GridConfig config_;
    std::vector<std::vector<CellState>> cells_;
};
