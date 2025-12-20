#include<iostream>
#include<vector>
#include<queue>

using namespace std;

int ys[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int xs[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

bool check(int y, int y_max, int x, int x_max) {
	return y >= 0 && y < y_max && x >= 0 && x < x_max;
}

int count_adjacent_rolls(vector<string> diagram, int i, int j, int y_max, int x_max) {
	int count = 0;
	for (int k = 0; k < 8; k++) {
		int y = i + ys[k];
		int x = j + xs[k];
		if (check(y, y_max, x, x_max)) {
			count += diagram[y][x] == '@';
		}
	}
	return count;
}

class PartOne {
	public:
		int solve(vector<string> diagram) {
			int y_max = diagram.size();
			int x_max = diagram[0].size();
			int result = 0;
			for (int i = 0; i < y_max; ++i) {
				for (int j = 0; j < x_max; ++j) {
					if (diagram[i][j] == '@') {
						if (count_adjacent_rolls(diagram, i, j, y_max, x_max) < 4) {
							result++;
						}
					}
				}
			}
			return result;
		}
};

class PartTwo {
	public:
		int solve(vector<string> diagram) {
			int y_max = diagram.size();
			int x_max = diagram[0].size();
			vector<vector<int>> indegrees(y_max, vector<int>(x_max, 0));
			vector<vector<bool>> visited(y_max, vector<bool>(x_max, 0));
			queue<pair<int, int>> queue;
			for (int i = 0; i < y_max; ++i) {
				for (int j = 0; j < x_max; ++j) {
					if (diagram[i][j] == '@') {
						int count = count_adjacent_rolls(diagram, i, j, y_max, x_max);
						indegrees[i][j] = count;
						if (count < 4) {
							visited[i][j] = true;
							queue.push(make_pair(i, j));
						}
					}
				}
			}

			int result = 0;
			while (!queue.empty()) {
				pair<int, int> next = queue.front();
				queue.pop();
				result++;
				int i = next.first;
				int j = next.second;

				for (int k = 0; k < 8; ++k) {
					int y = i + ys[k];
					int x = j + xs[k];
					if (check(y, y_max, x, x_max) && diagram[y][x] == '@') {
						indegrees[y][x]--;
						if (indegrees[y][x] < 4 && !visited[y][x]) {
							visited[y][x] = true;
							queue.push(make_pair(y, x));
						}
					}
				}
			}
			return result;
		}
};

int main(void) {
	string input;
	vector<string> inputs;
	while (cin >> input) {
		inputs.push_back(input);
	}

	PartOne partOne;
	int result1 = partOne.solve(inputs);
	cout << "Part one result: " << result1 << endl;

	PartTwo partTwo;
	int result2 = partTwo.solve(inputs);
	cout << "part two result: " << result2 << endl;

}