#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <array>


class CPU {
public:
	size_t counter = 0;
	int x = 1;
	std::vector<int> signal_strength;
	std::array<std::array<bool, 40>, 6>  crt;

	void check_counter() {
		if (counter == 20 || (counter > 20 && ((counter - 20) % 40 == 0))) {
			signal_strength.push_back(x * (int)counter);
		}
	}

	void draw_crt() {
		size_t row = counter / 40;
		size_t col = counter % 40;

		if (col == x || col == x - 1 || col == x + 1) {
			crt[row][col] = true;
		}
		else { crt[row][col] = false; }
	}

	void noop() {
		draw_crt();
		++counter;
		check_counter();
	}

	void addx(int n) {
		draw_crt();
		++counter;
		check_counter();
		draw_crt();
		++counter;
		check_counter();
		x += n;
	}
};


int main(int argc, char** argv) {
	std::string filename = "input.txt";
	CPU cpu;

	std::ifstream fs;
	fs.open(filename);
	if (!fs.good()) { return 1; }
	
	std::string line;
	while (std::getline(fs, line)) {
		if (line.substr(0, 4) == "noop") { cpu.noop(); }
		else if (line.substr(0, 4) == "addx") {
			int n = std::stoi(line.substr(5));
			cpu.addx(n);
		}
	}
	fs.close();

	int sum = 0;
	for (size_t i = 0; i < 6; ++i) {
		sum += cpu.signal_strength[i];
	}
	std::cout << "Task one: " << sum << std::endl;

	std::cout << "Task two: " << std::endl;
	for (size_t row = 0; row < 6; ++row) {
		for (size_t col = 0; col < 40; ++col) {
			if (cpu.crt[row][col]) { std::cout << '#'; }
			else { std::cout << '.'; }
		}
		std::cout << std::endl;
	}
}
