#include <string>
#include <set>
#include <array>
#include <iostream>
#include <fstream>


struct Point {
public:
	int x = 0;
	int y = 0;
};

bool operator<(const Point& lhs, const Point& rhs) {
	if (lhs.x == rhs.x) { return lhs.y < rhs.y; }
	return lhs.x < rhs.x;
}


class Rope {
public:
	Point head;
	Point tail;
	std::array<Point, 9> knots;
	std::set<Point> tail_visited;
	std::set<Point> knot_visited;
	Rope() { tail_visited.insert(tail); knot_visited.insert(tail); }
	void move_head(char direction, size_t steps) {
		for (size_t i = 0; i < steps; ++i) {
			switch (direction) {
			case 'U':
				head.x += 1;
				break;
			case 'D':
				head.x -= 1;
				break;
			case 'L':
				head.y += 1;
				break;
			case 'R':
				head.y -= 1;
				break;
			}
			move_tail(head, tail, tail_visited);

			bool insert = false;
			move_tail(head, knots[0], knot_visited, insert);
			for (size_t i = 1; i < knots.size(); ++i) {
				if (i == 8) { insert = true; }
				move_tail(knots[i - 1], knots[i], knot_visited, insert);
			}
		}
	}

	void move_tail(Point& head_ref, Point& tail_ref, std::set<Point>& set_ref, bool insert = true) {
		if (abs(head_ref.x - tail_ref.x) > 1 || abs(head_ref.y - tail_ref.y) > 1) {
			int dir_x = head_ref.x - tail_ref.x;
			int dir_y = head_ref.y - tail_ref.y;
			if (dir_x) { tail_ref.x += dir_x / abs(dir_x); }
			if (dir_y) { tail_ref.y += dir_y / abs(dir_y); }
			
			if(insert) { set_ref.insert(tail_ref); }
		}
	}
};


int main(int argc, char** argv) {	
	std::string filename = "input.txt";
	Rope rope;

	std::ifstream fs;
	fs.open(filename);
	if (!fs.good()) { return 1; }

	std::string line;
	while (std::getline(fs, line)) {
		char direction = line[0];
		int steps = std::stoi(line.substr(2));

		rope.move_head(direction, steps);
	}
	fs.close();

	std::cout << "Task one: " << rope.tail_visited.size() << std::endl;
	std::cout << "Task two: " << rope.knot_visited.size() << std::endl;
}

