#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>


class Folder {
public:
	Folder* parent;
	std::map<std::string, std::unique_ptr<Folder>> children;
	int direct_size = 0;
	Folder(Folder* parent) : parent(parent) {};
	
	void insert_children(std::string folder) {
		children[folder] = std::make_unique<Folder>(this);
	};

	int evaluate(int& sum_size) {
		int size = direct_size;

		for (auto it = children.begin(); it != children.end(); ++it) {
			size += it->second->evaluate(sum_size);
		}
		if (size <= 100000) {
			sum_size += size;
		}

		return size;
	}

	int free_memory(int need_to_free, int& min_folder_to_free) {
		int size = direct_size;

		for (auto it = children.begin(); it != children.end(); ++it) {
			size += it->second->free_memory(need_to_free, min_folder_to_free);
		}

		if (size >= need_to_free && size < min_folder_to_free) {
			min_folder_to_free = size;
		}
		return size;
	}
};


Folder* process_command(std::string line, Folder* current_folder_ptr);
void process_output(std::string line, Folder* current_folder_ptr);


int main(int argc, char** argv) {
	Folder root(nullptr);
	Folder* current_folder_ptr = &root;

	std::string filename = "input.txt";
	std::ifstream fs;
	fs.open(filename);
	if (!fs.good()) { return 1; }

	std::string line;
	std::getline(fs, line); // skipping first line `cd /`

	while (std::getline(fs, line)) {
		if (line[0] == '$') {
			current_folder_ptr = process_command(line, current_folder_ptr);
		}
		else {
			process_output(line, current_folder_ptr);
		}
	}
	fs.close();

	int sum_size = 0;
	int total_size = root.evaluate(sum_size);

	std::cout << "Task one: " << sum_size << std::endl;
	
	int need_to_free = 30000000 - (70000000 - total_size);
	int min_folder_to_free = 70000000;
	root.free_memory(need_to_free, min_folder_to_free);
	
	std::cout << "Task two: " << min_folder_to_free;
}


Folder* process_command(std::string line, Folder* current_folder_ptr) {
	if (line.substr(2, 2) == "cd") {
		if (line.substr(5) == "..") {
			current_folder_ptr = current_folder_ptr->parent;
		}
		else {
			current_folder_ptr = &*current_folder_ptr->children[line.substr(5)]; // transforming unique ptr to normal ptr
		}
	}
	return current_folder_ptr;
}


void process_output(std::string line, Folder* current_folder_ptr) {
	if (line[0] == 'd') {
		current_folder_ptr->insert_children(line.substr(4));
	}
	else {
		int value = std::stoi(line);
		current_folder_ptr->direct_size += value;
	}
}
