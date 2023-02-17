#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>


class Grid {
private:
	std::vector<char> grid_;
	std::vector<bool> visited_;
	size_t nrows_ = 0;
	size_t ncols_ = 0;
	size_t start_index_ = 0;
public:
	Grid(std::string& filename);
	const size_t nrows() const { return nrows_; }
	const size_t ncols() const { return ncols_; }
	const size_t start_row() const { return start_index_ / ncols_; }
	const size_t start_col() const { return start_index_ % ncols_; }
	const char get(size_t row, size_t col) const { return grid_[ncols_ * row + col]; }
	bool get_visited(size_t row, size_t col) { return visited_[ncols_ * row + col]; }
	void set_visited(size_t row, size_t col, bool vis) { visited_[ncols_ * row + col] = vis; }
	void reset_visited() {
		for (auto it = visited_.begin(); it != visited_.end(); ++it) {
			*it = false;
		}
	}
};

Grid::Grid(std::string& filename) {
	std::fstream fs;
	fs.open(filename);

	std::string line;
	while (std::getline(fs, line)) {
		ncols_ = line.size();
		nrows_ += 1;

		for (size_t i = 0; i < line.size(); ++i) {
			if (line[i] == 'S') {
				start_index_ = grid_.size();
				grid_.push_back('a');
			}
			else { grid_.push_back(line[i]); }
			visited_.push_back(false);
		}
	}
	fs.close();
}

struct VisitedPoint {
public:
	size_t row;
	size_t col;
	int step;
	char elevation;
	VisitedPoint(size_t row, size_t col, int step, char elevation) : row(row), col(col), step(step), elevation(elevation) {};
};

void clear(std::queue<VisitedPoint>& q)
{
	std::queue<VisitedPoint> empty;
	std::swap(q, empty);
}

int find_shortest_path(Grid& grid, std::queue<VisitedPoint>& points);


int main() {
	std::string filename = "input.txt";

	Grid grid(filename);
	std::queue<VisitedPoint> points;


	// Task one
	size_t start_row = grid.start_row();
	size_t start_col = grid.start_col();
	points.push(VisitedPoint(start_row, start_col, 0, 'a'));

	int task_one = find_shortest_path(grid, points);
	
	// Task two
	int task_two = INT_MAX;
	for (size_t row = 0; row < grid.nrows(); ++row) {
		for (size_t col = 0; col < grid.ncols(); ++col) {
			if (grid.get(row, col) != 'a') { continue; }

			clear(points);
			grid.reset_visited();

			points.push(VisitedPoint(row, col, 0, 'a'));

			int temp = find_shortest_path(grid, points);
			if (temp < task_two && temp != -1) {
				task_two = temp;
			}
		}
	}

	std::cout << "Task one: " << task_one << std::endl;
	std::cout << "Task two: " << task_two << std::endl;
	
}


int find_shortest_path(Grid& grid, std::queue<VisitedPoint>& points) {
	while (points.size() > 0) {
		VisitedPoint p = points.front();
		points.pop();

		if (grid.get_visited(p.row, p.col)) {
			continue; // already visited point
		}

		grid.set_visited(p.row, p.col, true);

		// step up
		if (p.row > 0) {
			if (!grid.get_visited(p.row - 1, p.col)) {
				if (grid.get(p.row - 1, p.col) == 'E') {
					if (int('z') - int(p.elevation) <= 1) {
						return p.step + 1;
					}
				}
				else if (int(grid.get(p.row - 1, p.col)) - int(p.elevation) <= 1) {
					points.push(VisitedPoint(p.row - 1, p.col, p.step + 1, grid.get(p.row - 1, p.col)));
				}
			}
		}
		// step up
		if (p.row < grid.nrows() - 1) {
			if (!grid.get_visited(p.row + 1, p.col)) {
				if (grid.get(p.row + 1, p.col) == 'E') {
					if (int('z') - int(p.elevation) <= 1) {
						return p.step + 1;
					}
				}
				else if (int(grid.get(p.row + 1, p.col)) - int(p.elevation) <= 1) {
					points.push(VisitedPoint(p.row + 1, p.col, p.step + 1, grid.get(p.row + 1, p.col)));
				}
			}
		}
		// step left
		if (p.col > 0) {
			if (!grid.get_visited(p.row, p.col - 1)) {
				if (grid.get(p.row, p.col - 1) == 'E') {
					if (int('z') - int(p.elevation) <= 1) {
						return p.step + 1;
					}
				}
				else if (int(grid.get(p.row, p.col - 1)) - int(p.elevation) <= 1) {
					points.push(VisitedPoint(p.row, p.col - 1, p.step + 1, grid.get(p.row, p.col - 1)));
				}
			}
		}
		// step right
		if (p.col < grid.ncols() - 1) {
			if (!grid.get_visited(p.row, p.col + 1)) {
				if (grid.get(p.row, p.col + 1) == 'E') {
					if (int('z') - int(p.elevation) <= 1) {
						return p.step + 1;
					}
				}
				else if (int(grid.get(p.row, p.col + 1)) - int(p.elevation) <= 1) {
					points.push(VisitedPoint(p.row, p.col + 1, p.step + 1, grid.get(p.row, p.col + 1)));
				}
			}
		}

	}
	return -1;
}
