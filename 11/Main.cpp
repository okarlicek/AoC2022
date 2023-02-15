#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>


class Monkey {
public:
	std::vector<long long> items;
	int plus_operation = 0;
	int mult_operation = 1;
	int pow_operation = 1;
	int test_div = 1;
	size_t true_index = 0;
	size_t false_index = 0;
	int inspected_items = 0;
};

long long calculate_business(std::vector<Monkey> monkeys) {
	size_t first = 0;
	size_t second = 0;
	for (auto it = monkeys.begin(); it != monkeys.end(); ++it) {
		if (it->inspected_items > first) {
			second = first;
			first = it->inspected_items;
		}
		else if (it->inspected_items > second) {
			second = it->inspected_items;
		}
	}
	return first * second;
}

bool process_file(std::string& filename, std::vector<Monkey>& monkeys);
void process_items(std::ifstream& fs, Monkey& monkey);
void process_operation(std::ifstream& fs, Monkey& monkey);
void process_test(std::ifstream& fs, Monkey& monkey);


int main(int argc, char** argv) {
	std::string filename = "input.txt";
	std::vector<Monkey> monkeys;
	int number_iterations = 20;
	
	// part one
	if (!process_file(filename, monkeys)) { return 1; }
	for (int i = 0; i < number_iterations; ++i) {
		for (auto it = monkeys.begin(); it != monkeys.end(); ++it) {
			it->inspected_items += (int)it->items.size();
			for (auto it_item = it->items.begin(); it_item != it->items.end(); ++it_item) {
				long long item_worry = *it_item;

				item_worry = (pow(item_worry, it->pow_operation) * it->mult_operation) + it->plus_operation;
				item_worry = item_worry / 3;
				if (item_worry % it->test_div == 0) {
					monkeys[it->true_index].items.push_back(item_worry);
				}
				else {
					monkeys[it->false_index].items.push_back(item_worry);
				}
			}
			it->items.clear();
			
		}
	}
	std::cout << "Task one: " << calculate_business(monkeys) << std::endl;
	
	// part two
	monkeys.clear();
	if (!process_file(filename, monkeys)) { return 1; }
	
	long long modular = 1;
	for (auto it = monkeys.begin(); it != monkeys.end(); ++it) {
		modular *= it->test_div;
	}

	number_iterations = 10000;
	for (int i = 0; i < number_iterations; ++i) {
		for (auto it = monkeys.begin(); it != monkeys.end(); ++it) {
			it->inspected_items += (int)it->items.size();
			for (auto it_item = it->items.begin(); it_item != it->items.end(); ++it_item) {
				long long item_worry = *it_item;

				item_worry = (pow(item_worry, it->pow_operation) * it->mult_operation) + it->plus_operation;
				item_worry = item_worry % modular;
				if (item_worry % it->test_div == 0) {
					monkeys[it->true_index].items.push_back(item_worry);
				}
				else {
					monkeys[it->false_index].items.push_back(item_worry);
				}
			}
			it->items.clear();

		}
	}
	std::cout << "Task two: " << calculate_business(monkeys) << std::endl;
}

bool process_file(std::string& filename, std::vector<Monkey>& monkeys) {
	std::ifstream fs;
	fs.open(filename);
	if (!fs.good()) { return false; }

	std::string line;
	while (std::getline(fs, line)) {
		if (line.size() == 0) {
			continue;
		}
		Monkey monkey;
		process_items(fs, monkey);
		process_operation(fs, monkey);
		process_test(fs, monkey);
		monkeys.push_back(monkey);
	}

	fs.close();
	return true;
}

void process_items(std::ifstream& fs, Monkey& monkey) {
	std::string items;
	std::getline(fs, items);

	items = items.substr(18);
	std::istringstream items_stream(items);

	std::string item;
	while (std::getline(items_stream, item, ',')) {
		long long item_worry = std::stoll(item);
		monkey.items.push_back(item_worry);
	}
}

void process_operation(std::ifstream& fs, Monkey& monkey) {
	std::string operation;
	std::getline(fs, operation);
	operation = operation.substr(23);
	if (operation == "* old") {
		monkey.pow_operation = 2;
	}
	else if (operation[0] == '*') {
		monkey.mult_operation = std::stoi(operation.substr(1));
	}
	else if (operation[0] == '+') {
		monkey.plus_operation = std::stoi(operation.substr(1));
	}
}

void process_test(std::ifstream& fs, Monkey& monkey) {
	std::string test;
	// div test
	std::getline(fs, test);
	test = test.substr(21);
	monkey.test_div = std::stoi(test);
	// true event
	std::getline(fs, test);
	test = test.substr(29);
	monkey.true_index = std::stoi(test);
	// false event
	std::getline(fs, test);
	test = test.substr(30);
	monkey.false_index = std::stoi(test);
}
