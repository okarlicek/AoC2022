#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class TreeGrid {
public:
	size_t ncols = 0;
	size_t nrows = 0;
	std::vector<int> grid;

	void insert_row(std::string& line) {
		ncols = line.size();
		int value;
		for (size_t i = 0; i < ncols; ++i) {
			grid.push_back(line[i] - '0');
		}
		++nrows;
	}

	int get(size_t row, size_t col) {
		return grid[nrows * row + col];
	}

	int check_visibility(size_t row, size_t col) {
		int height = get(row, col);

		if (row == 0 || col == 0 || row == nrows - 1 || col == ncols - 1) {
			return 1;
		}

		// up
		bool visible = true;
		for (size_t i = 1; i <= row; ++i) {
			if (height <= get(row - i, col)) { visible = false; break; }
		}
		if (visible) { return 1; }
		// down
		visible = true;
		for (size_t i = 1; i < nrows - row; ++i) {
			if (height <= get(row + i, col)) { visible = false; break; }
		}
		if (visible) { return 1; }
		// left
		visible = true;
		for (size_t i = 1; i <= col; ++i) {
			if (height <= get(row, col - i)) { visible = false; break; }
		}
		if (visible) { return 1; }
		// right
		visible = true;
		for (size_t i = 1; i < ncols - col; ++i) {
			if (height <= get(row, col + i)) { visible = false; break; }
		}
		if (visible) { return 1; }
		return 0;
	}

	int calc_scenic_score(size_t row, size_t col) {
		int up = 0, down = 0, right = 0, left = 0;
		int height = get(row, col);

		// up
		for (size_t i = 1; i <= row; ++i) {
			++up;
			if (height <= get(row - i, col)) { break; }
		}
		// down
		for (size_t i = 1; i < nrows - row; ++i) {
			++down;
			if (height <= get(row + i, col)) { break; }
		}
		// left
		for (size_t i = 1; i <= col; ++i) {
			++left;
			if (height <= get(row, col - i)) { break; }
		}
		// right
		for (size_t i = 1; i < ncols - col; ++i) {
			++right;
			if (height <= get(row, col + i)) { break; }
		}
		return up * down * left * right;;
	}
};


int main(int argc, char** argv) {
	std::string filename = "input.txt";
	TreeGrid grid;

	std::fstream fs;
	fs.open(filename);
	if (!fs.good()) { return 1; }

	std::string line;
	while (std::getline(fs, line)) {
		grid.insert_row(line);
	}
	fs.close();
	
	int visible = 0;
	int scenic_score = 0;
	for (size_t row = 0; row < grid.nrows; ++row) {
		for (size_t col = 0; col < grid.ncols; ++col) {
			visible += grid.check_visibility(row, col);
			
			int temp_scenic_score = grid.calc_scenic_score(row, col);
			if (temp_scenic_score > scenic_score) { scenic_score = temp_scenic_score; }
		}
	}

	std::cout << "Task one: " << visible << std::endl;
	std::cout << "Task two: " << scenic_score << std::endl;

}
