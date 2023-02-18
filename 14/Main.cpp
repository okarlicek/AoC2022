#include <iostream>
#include <string>
#include <fstream>
#include <array>


constexpr size_t Y_MAX = 200;
constexpr size_t X_MIN = 300;
constexpr size_t X_MAX = 700;

class Cave {
public:
	std::array<bool, (X_MAX - X_MIN)* Y_MAX> data; // x = [X_MIN, X_MAX), y = [0, Y_MAX]
	Cave() { data.fill(false); }
	bool& at(size_t x, size_t y) { return data[(x - X_MIN) + y * (X_MAX - X_MIN)]; }
};

bool process_data(std::string& filename, Cave& cave, int* max_y);
void process_line(std::string& line, Cave& cave, int* max_y);
void process_coord(int x1, int y1, int x2, int y2, Cave& cave);
void add_floor(int y, Cave& cave);
bool sand_drop(Cave& cave);

int main() {
	std::string filename = "input.txt";
	Cave* cave = new Cave;
	int max_y = 0;

	// task one
	if (!process_data(filename, *cave, &max_y)) { return -1; }
	
	bool drop_sand = true;
	int task_one = 0;

	while (drop_sand) {
		drop_sand = sand_drop(*cave);
		if (drop_sand) { task_one += 1; }
	}
	
	// task two
	add_floor(max_y + 2, *cave);
	
	drop_sand = true;
	int task_two = 0;

	while (drop_sand) {
		drop_sand = sand_drop(*cave);
		task_two += 1;
	}

	std::cout << "Task one: " << task_one << std::endl;
	std::cout << "Task two: " << task_two + task_one << std::endl;

	delete cave;
}

bool sand_drop(Cave& cave) {
	size_t sand_x = 500;
	size_t sand_y = 0;
	
	bool rest = false;
	while (!rest) {
		if (sand_y == Y_MAX - 1 || sand_x == X_MIN || sand_x == X_MAX - 1) {
			return false;
		}
		else if (!cave.at(sand_x, sand_y + 1)) {
			sand_y += 1;
		}
		else if (!cave.at(sand_x - 1, sand_y + 1)) {
			sand_y += 1;
			sand_x -= 1;
		}
		else if (!cave.at(sand_x + 1, sand_y + 1)) {
			sand_y += 1;
			sand_x += 1;
		}
		else {
			rest = true;
		}
	}
	if (sand_x == 500 && sand_y == 0) {
		return false;
	}
	cave.at(sand_x, sand_y) = true;
	return true;
}

void add_floor(int y, Cave& cave) {
	for (size_t i = X_MIN; i < X_MAX; ++i) {
		cave.at(i, y) = true;
	}
}

bool process_data(std::string& filename, Cave& cave, int* max_y) {
	std::fstream fs;
	fs.open(filename);

	if (!fs.good()) { return false; }

	std::string line;
	while (std::getline(fs, line)) {
		process_line(line, cave, max_y);
	}
	
	return true;
}

void process_line(std::string& line, Cave& cave, int* max_y) {
	size_t pos;

	int first_x, first_y, second_x, second_y;

	first_x = std::stoi(line, &pos);
	line = line.substr(pos + 1);
	first_y = std::stoi(line, &pos);
	line = line.substr(pos + 4);
	if (*max_y < first_y) { *max_y = first_y; }

	while (line.size() > 0) {
		second_x = std::stoi(line, &pos);
		line = line.substr(pos + 1);
		second_y = std::stoi(line, &pos);
		if (line.size() > pos + 4) {
			line = line.substr(pos + 4);
		}
		else {
			line = line.substr(pos);
		}
		process_coord(first_x, first_y, second_x, second_y, cave);

		first_x = second_x;
		first_y = second_y;
		if (*max_y < first_y) { *max_y = first_y; }
	}
}

void process_coord(int x1, int y1, int x2, int y2, Cave& cave) {
	if (x1 < x2) {
		for (size_t i = 0; i <= x2 - x1; ++i) {
			cave.at(x1 + i, y1) = true;
		}
	}
	else if (x1 > x2) {
		for (size_t i = 0; i <= x1 - x2; ++i) {
			cave.at(x2 + i, y2) = true;
		}
	}
	if (y1 < y2) {
		for (size_t i = 0; i <= y2 - y1; ++i) {
			cave.at(x1, y1 + i) = true;
		}
	}
	else if (y1 > y2) {
		for (size_t i = 0; i <= y1 - y2; ++i) {
			cave.at(x2, y2 + i) = true;
		}
	}
}

