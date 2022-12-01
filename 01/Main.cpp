#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>

constexpr int N_MAXS = 3; // n highest values to consider

bool load_data(const std::string& filename, std::vector<int>& elves);
bool load_data(std::ifstream& is, std::vector<int>& elves);
void print_maximum(const std::vector<int>& elves);
void print_maximum_n(const std::vector<int>& elves);


int main(int argc, char** argv) {
	std::string filename = "input.txt"; // filename with inputs
	std::vector<int> elves; // vector of elves
	if (!load_data(filename, elves)) { return 1; }

	print_maximum(elves);
	print_maximum_n(elves);
	return 0;
}

void print_maximum_n(const std::vector<int>& elves) {
	// will print `n` highest number of calories
	std::array<int, N_MAXS> maxs;
	maxs.fill(0);

	for (auto&& elf_cals : elves) {
		for (size_t i = 0; i < N_MAXS; ++i) {
			if (maxs[i] < elf_cals) {
				// if the elf has more calories than is in the array we put that value into array
				// we need to shift the values so we get rid of the smallest value

				for (size_t j = maxs.size()-1; j > i; --j) {
					maxs[j] = maxs[j - 1];
				}
				maxs[i] = elf_cals;
				break;
			}
		}
	}
	int total = 0;
	for (size_t i = 0; i < N_MAXS; ++i) {
		total += maxs[i];
	}
	std::cout << N_MAXS << " highest total: " << total <<std::endl;
}

void print_maximum(const std::vector<int>& elves) {
	// will find elf with highest number of calories
	int maximum = -1;
	for (auto&& elf_cals : elves) {
		if (elf_cals > maximum) {
			maximum = elf_cals;
		}
	}
	std::cout << "Maximum: " << maximum << std::endl;
}

bool load_data(const std::string& filename, std::vector<int>& elves) {
	std::ifstream fs;
	fs.open(filename); // opening file
	if (!fs.good()) {
		return false;
	}
	return load_data(fs, elves); // processing ifstream
}


bool load_data(std::ifstream& is, std::vector<int>& elves) {
	std::string line;
	int sum_calories = 0;

	while (std::getline(is, line)) {
		if (line.empty()) { 
			// when line empty we push sum_calories to elves vector and reset the value
			elves.push_back(sum_calories);
			sum_calories = 0;
			continue;
		}
		int calorie = std::stoi(line); // parsing number
		sum_calories += calorie;
	}
	return true;
}
