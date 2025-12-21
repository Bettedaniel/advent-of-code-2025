#include<iostream>
#include<vector>
#include<queue>

using namespace std;

typedef long long int ll;

int ys[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int xs[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
int down = 6; // "Down" is at this index in the above arrays.
int up = 1; // "Up" is at this index in the above arrays.
int sides[2] = { 3, 4 }; // "Sides" is at these indices in the above arrays.

pair<int, int> find_s(vector<string> input) {
	for (int i = 0; i < input.size(); ++i) {
		for (int j = 0; j < input[i].size(); ++j) {
			if (input[i][j] == 'S') {
				return make_pair(i, j);
			}
		}
	}
	return make_pair(-1, -1);
}

int part_one(vector<string> input) {
	vector<vector<bool>> visited(input.size(), vector<bool>(input[0].size()));
	queue<pair<int, int>> queue;

	int y_max = input.size();

	queue.push(find_s(input));

	int result = 0;
	while (!queue.empty()) {
		pair<int, int> node = queue.front();
		queue.pop();
		int node_y = node.first;
		int node_x = node.second;
		if (input[node_y][node_x] == '^') {
			result++;
			for (int i = 0; i < 2; ++i) {
				int y = node_y + ys[sides[i]];
				int x = node_x + xs[sides[i]];
				if (y >= 0 && y < y_max && x >= 0 && x < input[y].size() && !visited[y][x]) {
					queue.push(make_pair(y, x));
					visited[y][x] = true;
				}
			}
		} else {
			int y = node_y + ys[down];
			int x = node_x + xs[down];
			if (y >= 0 && y < y_max && x >= 0 && x < input[y].size() && !visited[y][x]) {
				queue.push(make_pair(y, x));
				visited[y][x] = true;
			}
		}
	}

	return result;
}

ll part_two(vector<string> input) {
	vector<vector<ll>> timelines(input.size(), vector<ll>(input[0].size(), 0));

	pair<int, int> s_yx = find_s(input);

	int y_max = input.size();
	for (int i = 0; i < input[y_max - 1].size(); ++i) {
		timelines[y_max - 1][i] = 1;
	}

	for (int i = y_max - 2; i >= 0; --i) {
		int x_max = input[i].size();
		for (int j = 0; j < x_max; ++j) {
			if (input[i][j] != '^') {
				int y = i + ys[down];
				int x = j + xs[down];
				if (y >= 0 && y < y_max && x >= 0 && x < x_max) {
					timelines[i][j] += timelines[y][x];
				}
			}
		}
		for (int j = 0; j < input[i].size(); ++j) {
			if (input[i][j] == '^') {
				for (int k = 0; k < 2; ++k) {
					int y = i + ys[sides[k]];
					int x = j + xs[sides[k]];
					if (y >= 0 && y < y_max && x >= 0 && x < x_max) {
						timelines[i][j] += timelines[y][x];
					}
				}
			}
		}
	}

	return timelines[s_yx.first][s_yx.second];
}

int main(void) {
	string line;
	vector<string> input;
	while (cin >> line) {
		input.push_back(line);
	}

	int result1 = part_one(input);
	cout << "Part one result: " << result1 << endl;

	ll result2 = part_two(input);
	cout << "Part two result: " << result2 << endl;
}