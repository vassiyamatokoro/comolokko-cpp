#include "Grid.hpp"

#include <spdlog/spdlog.h>

Grid::Grid(const GridConfig& config)
    : config_(config),
      cells_(static_cast<std::size_t>(config.rows),
             std::vector<CellState>(static_cast<std::size_t>(config.cols), CellState::Empty)) {
    spdlog::info("Izgara olusturuldu: {}x{}", config_.rows, config_.cols);
}

void Grid::reset() {
    for (auto& row : cells_) {
        for (auto& cell : row) {
            cell = CellState::Empty;
        }
    }
    spdlog::info("Izgara sifirlandi.");
}

bool Grid::isInside(int row, int col) const noexcept {
    return row >= 0 && row < config_.rows && col >= 0 && col < config_.cols;
}

CellState Grid::getCell(int row, int col) const {
    if (!isInside(row, col)) {
        return CellState::Empty;
    }
    return cells_[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
}

bool Grid::setCell(int row, int col, CellState state) {
    if (!isInside(row, col)) {
        spdlog::warn("Gecersiz hucre koordinati: ({}, {})", row, col);
        return false;
    }
    cells_[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)] = state;
    return true;
}

void Grid::draw() const {
    for (int r = 0; r < config_.rows; ++r) {
        for (int c = 0; c < config_.cols; ++c) {
            const int x = config_.offsetX + c * (config_.cellSize + config_.padding);
            const int y = config_.offsetY + r * (config_.cellSize + config_.padding);

            Color fill = config_.cellColor;
            const CellState state = cells_[static_cast<std::size_t>(r)][static_cast<std::size_t>(c)];
            if (state == CellState::Blocked) {
                fill = LIGHTGRAY;
            } else if (state == CellState::Active) {
                fill = SKYBLUE;
            }

            DrawRectangle(x, y, config_.cellSize, config_.cellSize, fill);
            DrawRectangleLines(x, y, config_.cellSize, config_.cellSize, config_.borderColor);
        }
    }
}
