#include <iostream>
#include <vector>
#include <string>

using namespace std;

class PartOne {
	private:
		int turnRight(int start, int distance) {
			return (start + distance) % 100;
		}

		int turnLeft(int start, int distance) {
			int d = distance % 100;
			return turnRight(start, 100 - d);
		}

		int turn(int start, char direction, int distance) {
			if (direction == 'L') {
				return turnLeft(start, distance);
			} else {
				return turnRight(start, distance);
			}
		}

	public:
		int solve(vector<char> directions, vector<int> distances) {
			int result = 0;
			int position = 50;
			for (int i = 0; i < distances.size(); i++) {
				position = turn(position, directions[i], distances[i]);
				if (position == 0) {
					++result;
				}
			}
			return result;
		}
};

class PartTwo {
	private:
		pair<int, int> turnRight(int start, int distance) {
			int max = start + distance;
			int passZeroCount = max / 100;
			return make_pair(max % 100, passZeroCount);
		}

		pair<int, int> turnLeft(int start, int distance) {
			int fullRounds = distance / 100;
			int d = distance % 100;
			int extraZero = d >= start && start != 0 ? 1 : 0;
			pair<int, int> rightTurn = turnRight(start, 100 - d);
			return make_pair(rightTurn.first, fullRounds + extraZero);
		}

		pair<int, int> turn(int start, char direction, int distance) {
			if (direction == 'L') {
				return turnLeft(start, distance);
			} else {
				return turnRight(start, distance);
			}
		}


	public:
		int solve(vector<char> directions, vector<int> distances) {
			int result = 0;
			int position = 50;
			for (int i = 0; i < distances.size(); i++) {
				pair<int, int> t = turn(position, directions[i], distances[i]);
				position = t.first;
				result += t.second;
			}
			return result;
		}
};

int main(void) {
	string line;
	vector<char> directions;
	vector<int> distances;
	while (cin >> line) {
		directions.push_back(line[0]);
		int distance = line[1] - '0';
		for (int i = 2; i < line.size(); i++) {
			int c = line[i] - '0';
			distance = distance * 10 + c;
		}
		distances.push_back(distance);
	}

	PartOne program1;
	cout << "Result part one: " << program1.solve(directions, distances) << endl;

	PartTwo program2;
	cout << "Result part two: " << program2.solve(directions, distances) << endl;

}