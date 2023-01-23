#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class Game {
private:
	char opponent_move_;
	char my_move_;
	int choose_move() const;
public:
	Game(char oponent_move, char my_move) : opponent_move_(oponent_move), my_move_(my_move) {};
	int evaluate_part_one() const;
	int evaluate_part_two() const;
};

int Game::evaluate_part_one() const {
	int score = 0;
	switch (my_move_) {
	case 'X':
		score += 1;
		break;
	case 'Y':
		score += 2;
		break;
	case 'Z':
		score += 3;
		break;
	}
	if (my_move_ == opponent_move_) {
		score += 3;
	}
	else if ((my_move_ == 'X' && opponent_move_ == 'Z') ||
		(my_move_ == 'Z' && opponent_move_ == 'Y') ||
		(my_move_ == 'Y' && opponent_move_ == 'X')) {
		score += 6;
	}
	return score;
}

int Game::choose_move() const {
	char my_move;
	if (my_move_ == 'Y') {
		my_move = opponent_move_;
	}
	else if (my_move_ == 'X') {
		if (opponent_move_ == 'X') { my_move = 'Z'; }
		else if (opponent_move_ == 'Y') { my_move = 'X'; }
		else if (opponent_move_ == 'Z') { my_move = 'Y'; }
	}
	else if (my_move_ == 'Z') {
		if (opponent_move_ == 'X') { my_move = 'Y'; }
		else if (opponent_move_ == 'Y') { my_move = 'Z'; }
		else if (opponent_move_ == 'Z') { my_move = 'X'; }
	}

	switch (my_move) {
	case 'X':
		return 1;
	case 'Y':
		return 2;
	case 'Z':
		return 3;
	default:
		return 0;
	}
}

int Game::evaluate_part_two() const {
	int score = 0;
	switch (my_move_) {
	case 'X':
		score += choose_move();
		break;
	case 'Y':
		score += 3 + choose_move();
		break;
	case 'Z':
		score += 6 + choose_move();
		break;
	}
	return score;
}

bool load_data(const std::string& filename, std::vector<Game>& games);
bool load_data(std::ifstream& is, std::vector<Game>& games);

int main(int argc, char** argv) {
	std::string filename = "input.txt";
	std::vector<Game> games;
	load_data(filename, games);

	int total_score_1 = 0;
	int total_score_2 = 0;

	for (auto it = games.begin(); it != games.end(); ++it) {
		total_score_1 += it->evaluate_part_one();
		total_score_2 += it->evaluate_part_two();
	}
	std::cout << "Total score of part one is: " << total_score_1 << std::endl;
	std::cout << "Total score of part two is: " << total_score_2 << std::endl;

}

bool load_data(const std::string& filename, std::vector<Game>& games) {
	std::ifstream fs;
	fs.open(filename);
	if (!fs.good()) {
		return false;
	}
	bool success = load_data(fs, games);
	fs.close();
	return success;
}

bool load_data(std::ifstream& is, std::vector<Game>& games) {
	std::string line;
	while (std::getline(is, line)) {
		char oponent_move = line[0];
		switch (oponent_move) {
		case 'A':
			oponent_move = 'X';
			break;
		case 'B':
			oponent_move = 'Y';
			break;
		case 'C':
			oponent_move = 'Z';
			break;
		}
		char my_move = line[2];
		games.push_back(std::move(Game(oponent_move, my_move)));
	}
	return true;
}

