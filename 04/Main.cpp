#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

bool process_file(std::string filename, int& score_contains, int& score_overlaps);
int check_intervals_contains(int start1, int last1, int start2, int last2);
int check_intervals_overlapped(int start1, int last1, int start2, int last2);


int main(int charc, char** charv) {
	std::string filename = "input.txt";
	int score_contains = 0;
	int score_overlaps = 0;
	process_file(filename, score_contains, score_overlaps);

	std::cout << "Part one score is: " << score_contains << std::endl;
	std::cout << "Part two score is: " << score_overlaps << std::endl;
}

bool process_file(std::string filename, int& score_contains, int& score_overlaps) {
	std::ifstream fs;
	fs.open(filename);

	int start1, last1, start2, last2;
	std::string line, value, pair;
	while (std::getline(fs, line)) {
		std::istringstream stream_line(line);

		std::getline(stream_line, pair, ',');
		std::istringstream stream_pair(pair);
		
		std::getline(stream_pair, value, '-');
		std::istringstream(value) >> start1;
		std::getline(stream_pair, value, '-');
		std::istringstream(value) >> last1;

		std::getline(stream_line, pair, ',');
		std::istringstream stream_pair2(pair);

		std::getline(stream_pair2, value, '-');
		std::istringstream(value) >> start2;
		std::getline(stream_pair2, value, '-');
		std::istringstream(value) >> last2;
		score_contains += check_intervals_contains(start1, last1, start2, last2);
		score_overlaps += check_intervals_overlapped(start1, last1, start2, last2);
	}
	return true;
}

int check_intervals_contains(int start1, int last1, int start2, int last2) {
	if (start1 <= start2 && last1 >= last2) { return 1; }
	if (start1 >= start2 && last1 <= last2) { return 1; }
	return 0;
}

int check_intervals_overlapped(int start1, int last1, int start2, int last2) {
	if (last1 < start2 || last2 < start1) { return 0; }
	return 1;
}

