#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>

enum TypeSignal {Int, List};

class SignalBase {
public:
	SignalBase() {};
	virtual ~SignalBase() {};
	virtual TypeSignal type() = 0;
};

class SignalInt final : public SignalBase {
public:
	int data = 0;
	virtual TypeSignal type() { return TypeSignal::Int; };
};

class SignalList final : public SignalBase {
public:
	std::queue<std::unique_ptr<SignalBase>> data;
	SignalList* father = nullptr;
	virtual TypeSignal type() { return TypeSignal::List; };
};



int compare(SignalBase* left, SignalBase* right);
int compare(SignalInt* left, SignalInt* right);
int compare(SignalList* left, SignalList* right);
int compare(SignalInt* left, SignalList* right);
int compare(SignalList* left, SignalInt* right);
bool process_file(std::string& filename, std::vector<std::unique_ptr<SignalList>>& left, std::vector<std::unique_ptr<SignalList>>& right);
void process_line(std::string& line, SignalList& root);
void create_divider(SignalList& root, bool first);


int main() {
	std::string filename = "input.txt";

	std::vector<std::unique_ptr<SignalList>> left;
	std::vector<std::unique_ptr<SignalList>> right;
	
	if (!process_file(filename, left, right)) { return -1; }

	// task one
	int task_one = 0;
	for (size_t i = 0; i < left.size(); ++i) {
		if (compare(&*left[i], &*right[i]) == -1) {
			task_one += i + 1;
		}
	}
	
	// task two

	// This part is very ugly as my compare function is poping the elements from the list
	// I need to reload data for every divider, was kind of lazy to change how the compare work
	std::vector<std::unique_ptr<SignalList>> signals;
	if (!process_file(filename, signals, signals)) { return -1; }

	int first = 1;
	int second = 2;
	// first divider
	for (size_t i = 0; i < signals.size(); ++i) {
		SignalList div;
		create_divider(div, true);
		if (compare(&div, &*signals[i]) > 0) {
			first += 1;
		}
	}
	// second divider
	signals.clear();
	if (!process_file(filename, signals, signals)) { return -1; }
	for (size_t i = 0; i < signals.size(); ++i) {
		SignalList div;
		create_divider(div, false);
		if (compare(&div, &*signals[i]) > 0) {
			second += 1;
		}
	}

	std::cout << "TASK ONE: " << task_one << std::endl;
	std::cout << "TASK TWO: " << first * second << std::endl;
}

int compare(SignalBase* left, SignalBase* right) {
	if (left->type() == TypeSignal::Int && right->type() == TypeSignal::Int) {
		return compare((SignalInt*)left, (SignalInt*)right);
	}
	else if (left->type() == TypeSignal::List && right->type() == TypeSignal::Int) {
		return compare((SignalList*)left, (SignalInt*)right);
	}
	else if (left->type() == TypeSignal::Int && right->type() == TypeSignal::List) {
		return compare((SignalInt*)left, (SignalList*)right);
	}
	return compare((SignalList*)left, (SignalList*)right);
}

int compare(SignalInt* left, SignalInt* right) {
	if (left->data < right->data) { return -1; }
	else if (left->data > right->data) { return 1; }
	return 0;
}

int compare(SignalList* left, SignalList* right) {
	// both list empty
	if (left->data.size() == 0 && right->data.size() == 0) { return 0; }
	else if (left->data.size() == 0) { return -1; }
	else if (right->data.size() == 0) { return 1; }
	int comparison = compare(&*left->data.front(), &*right->data.front());
	left->data.pop();
	right->data.pop();
	if (comparison == 0) { return compare(left, right); }
	return comparison;
}

int compare(SignalInt* left, SignalList* right) {
	SignalList left_list;
	SignalInt left_value;
	left_value.data = left->data;
	left_list.data.push(std::make_unique<SignalInt>(std::move(left_value)));
	return compare(&left_list, right);
}

int compare(SignalList* left, SignalInt* right) {
	return compare(right, left) * -1;
}

void create_divider(SignalList& root, bool first) {
	std::string line;
	if (first) { line = "[[2]]"; }
	else { line = "[[6]]"; }

	process_line(line, root);
}

bool process_file(std::string& filename, std::vector<std::unique_ptr<SignalList>>& left, std::vector<std::unique_ptr<SignalList>>& right) {
	std::fstream fs;
	fs.open(filename);

	if (!fs.good()) { return false; }

	std::string line;
	while(std::getline(fs, line)) {
		if (line.size() == 0) { continue; }

		std::unique_ptr<SignalList> l = std::make_unique<SignalList>();
		std::unique_ptr<SignalList> r = std::make_unique<SignalList>();

		process_line(line, *l);
		std::getline(fs, line);
		process_line(line, *r);

		left.push_back(std::move(l));
		right.push_back(std::move(r));
	}
	return true;

}

void process_line(std::string& line, SignalList& root) {
	SignalList* current_list = &root;
	for (size_t i = 0; i < line.size(); ++i) {
		if (line[i] == '[') {
			SignalList new_list;
			new_list.father = current_list;
			current_list->data.push(std::make_unique<SignalList>(std::move(new_list)));
			current_list = (SignalList*)& *current_list->data.back();
		}
		else if (line[i] == ']') {
			current_list = current_list->father;
		}
		else if (line[i] == ',') {
			continue;
		}
		else {
			size_t pos;
			SignalInt value;
			value.data = std::stoi(line.substr(i), &pos);
			current_list->data.push(std::make_unique<SignalInt>(std::move(value)));
		}
	}
}
