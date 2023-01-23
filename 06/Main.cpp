#include <string>
#include <iostream>
#include <fstream>


size_t find_marker(std::string& data, size_t n_chars) {
	bool same = false;
	for (size_t index = n_chars - 1; index < data.size(); ++index) {

		for (size_t i_shift = 0; i_shift < n_chars; ++i_shift) {
			for (size_t j_shift = i_shift + 1; j_shift < n_chars; ++j_shift) {
				if (data[index - i_shift] == data[index - j_shift]) {
					same = true;
					break;
				}
				if (same) { break; }
			}
		}
		if (same) {
			same = false;
			continue;
		}
		return index + 1;
	}
}

int main(int argc, char** argv) {
	std::string data;

	std::fstream fs;
	fs.open("input.txt");
	if (!fs.good()) { return 1; }

	std::getline(fs, data);
	fs.close();

	std::cout << "Task 1: " << find_marker(data, 4) << std::endl;
	std::cout << "Task 1: " << find_marker(data, 14) << std::endl;
}
