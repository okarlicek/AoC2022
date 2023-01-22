#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <string>


class Rucksack {
public:
	Rucksack(std::string& items);
	std::set<char> left_pocket;
	std::set<char> right_pocket;
	int value_of_char(char ch) const;
	int evaluate() const;
	bool is_in(char ch) const;
};

Rucksack::Rucksack(std::string& items) {
	size_t half = items.size() / 2;
	for (size_t i = 0; i < items.size(); ++i) {
		if (i < half) {
			left_pocket.insert(items[i]);
		}
		else {
			right_pocket.insert(items[i]);
		}
	}
}

int Rucksack::value_of_char(char ch) const {
	if (isupper(ch)) {
		return int(ch) - 38;
	}
	return int(ch) - 96;
}

int Rucksack::evaluate() const {
	int score = 0;
	for (auto it = left_pocket.begin(); it != left_pocket.end(); ++it) {
		char ch = *it;
		if (right_pocket.count(ch)) {
			score += value_of_char(ch);
		}
	}
	return score;
}

bool Rucksack::is_in(char ch) const {
	if (left_pocket.count(ch)) {
		return true;
	}
	else if (right_pocket.count(ch)) {
		return true;
	}
	return false;
}


bool load_data(std::string& filename, std::vector<Rucksack>& data);
bool load_data(std::ifstream& file, std::vector<Rucksack>& data);
int task_two_score(std::vector<Rucksack>& data);
int find_common(Rucksack& elf1, Rucksack& elf2, Rucksack& elf3);

int main(int charc, char** argv) {
	std::string filename = "input.txt";
	std::vector<Rucksack> data;
	load_data(filename, data);

	int total_score = 0;
	for (auto it = data.begin(); it != data.end(); ++it) {
		total_score += it->evaluate();
	}
	std::cout << "Score of part one is: " << total_score << std::endl;
	std::cout << "Score of part two is: " << task_two_score(data) << std::endl;
}

int task_two_score(std::vector<Rucksack>& data) {
	int total_score = 0;
	for (auto it = data.begin(); it < data.end(); ++it) {
		Rucksack& elf1 = *it;
		Rucksack& elf2 = *(++it);
		Rucksack& elf3 = *(++it);
		total_score += find_common(elf1, elf2, elf3);
	}
	return total_score;
}

int find_common(Rucksack& elf1, Rucksack& elf2, Rucksack& elf3) {
	for (auto it = elf1.left_pocket.begin(); it != elf1.left_pocket.end(); ++it) {
		if (elf2.is_in(*it) && elf3.is_in(*it)) {
			return elf1.value_of_char(*it);
		}
	}
	for (auto it = elf1.right_pocket.begin(); it != elf1.right_pocket.end(); ++it) {
		if (elf2.is_in(*it) && elf3.is_in(*it)) {
			return elf1.value_of_char(*it);
		}
	}
	return 0;
}

bool load_data(std::string& filename, std::vector<Rucksack>& data) {
	std::ifstream fs;
	fs.open(filename);
	if (!fs.good()) {
		return false;
	}
	return load_data(fs, data);
}

bool load_data(std::ifstream& file, std::vector<Rucksack>& data) {
	std::string line;
	while (std::getline(file, line)) {
		data.emplace_back(Rucksack(line));
	}
	return true;
}
