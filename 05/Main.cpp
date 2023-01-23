#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <array>


class Crates {
private:
	std::array<std::deque<char>, 9> data_;
public:
	void insert_data(char ch, size_t position);
	void move_crate_9000(size_t n_times, size_t from, size_t to);
	void move_crate_9001(size_t n_times, size_t from, size_t to);
	void print_top_items();
};

void Crates::insert_data(char ch, size_t position) {
	data_[position].push_front(ch);
}

void Crates::move_crate_9000(size_t n_times, size_t from, size_t to) {
	for (size_t i = 0; i < n_times; ++i) {
		data_[to].push_back(data_[from].back());
		data_[from].pop_back();
	}
}

void Crates::move_crate_9001(size_t n_times, size_t from, size_t to) {
	size_t size = data_[from].size();

	for (size_t i = n_times; i > 0; --i) {
		data_[to].push_back(data_[from][size - i]);
	}
	for (size_t i = 0; i < n_times; ++i) {
		data_[from].pop_back();
	}
}

void Crates::print_top_items() {
	for (size_t i = 0; i < data_.size(); ++i) {
		std::cout << data_[i].back();
	}
	std::cout << std::endl;
}


void load_data(std::string& filename, Crates& data, int task);
void load_data(std::ifstream& fs, Crates& data, int task);

int main(int argc, char** argv) {
	std::string filename = "input.txt";
	Crates data;
	Crates data2;

	load_data(filename, data, 1);
	std::cout << "Task one: ";
	data.print_top_items();

	load_data(filename, data2, 2);
	std::cout << "Task two: ";
	data2.print_top_items();
}

void load_data(std::string& filename, Crates& data, int task) {
	std::ifstream fs;
	fs.open(filename);
	if (!fs.good()) {
		return;
	}
	load_data(fs, data, task);
	fs.close();
}

void load_data(std::ifstream& fs, Crates& data, int task) {
	std::string line;

	while (std::getline(fs, line)) {
		if (line.size() == 0) {
			break;
		}

		// inserting data to Crates
		for (size_t i = 0; i < line.size(); ++i) {
			if (isdigit(line[i])) {
				// end of the crates
				break;
			}
			if (isalpha(line[i])) {
				data.insert_data(line[i], (i - 1) / 4);
			}
		}
	}
	
	std::string value;
	std::array<size_t, 3> values;
	size_t index_values = 0;
	while (std::getline(fs, line)) {
		std::istringstream line_stream(line);
		while (std::getline(line_stream, value, ' ')) {
			if (isdigit(value[0])) {
				std::istringstream(value) >> values[index_values];
				++index_values;
			}
		}
		index_values = 0;
		if (task == 1) {
			data.move_crate_9000(values[0], values[1] - 1, values[2] - 1);
		}
		else if (task == 2) {
			data.move_crate_9001(values[0], values[1] - 1, values[2] - 1);
		}
	}
}
