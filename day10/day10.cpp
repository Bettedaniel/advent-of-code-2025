#include<iostream>
#include<vector>
#include<string>

using namespace std;

typedef long long int ll;
typedef unsigned int uint;

pair<string, int> read_lights(string line) {
	for (int i = 0; i < line.size(); ++i) {
		if (line[i] == ']') {
			return make_pair(line.substr(1, i - 1), i + 2);
		} else if (line[i] == '[') {
			continue;
		}
	}
	// This shouldn't happen. It would mean we only have lights and nothing else.
	return make_pair(line.substr(1, line.size() - 2), line.size());
}

pair<vector<vector<int>>, int> read_buttons(string line, int start) {
	vector<vector<int>> buttons;
	int i = start;
	while (i < line.size()) {
		if (line[i] == '{') {
			return make_pair(buttons, i);
		}
		if (line[i] == '(') {
			vector<int> button;
			int num = 0;
			++i;
			while (line[i] != ')') {
				if (line[i] == ',') {
					button.push_back(num);
					num = 0;
				} else {
					num = num * 10 + (line[i] - '0');
				}
				++i;
			}
			button.push_back(num);
			buttons.push_back(button);
		}
		++i;
	}
	// This shouldn't happen. It would mean we have no joltages.
	return make_pair(buttons, line.size());
}

vector<int> read_joltages(string line, int start) {
	vector<int> joltages;
	int num = 0;
	for (int i = start; i < line.size(); ++i) {
		if (line[i] == '}') {
			joltages.push_back(num);
			break;
		} else if (line[i] == '{') {
			continue;
		} else if (line[i] == ',') {
			joltages.push_back(num);
			num = 0;
		} else {
			num = num * 10 + (line[i] - '0');
		}
	}
	return joltages;
}

int part_one_solve(string lights, vector<vector<int>> buttons) {
	uint lights_int = 0;
	for (int i = lights.size() - 1; i >= 0; --i) {
		lights_int = (lights_int << 1) | (lights[i] == '#');
	}

	vector<uint> button_ints(buttons.size());
	for (int i = 0; i < buttons.size(); ++i) {
		uint button_int = 0;
		for (int j = 0; j < buttons[i].size(); ++j) {
			button_int = button_int | (1 << buttons[i][j]);
		}
		button_ints[i] = button_int;
	}

	uint max_val = (1 << lights.size());
	uint INF = 1 << 30;
	vector<uint> dp(max_val + 1, INF);
	dp[0] = 0;
	for (int i = 0; i < button_ints.size(); ++i) {
		for (int j = 0; j <= max_val; ++j) {
			if (dp[j] < INF) {
				dp[j ^ button_ints[i]] = min(dp[j ^ button_ints[i]], dp[j] + 1);
			}
		}
	}
	return dp[lights_int];
}

int part_one(vector<string> lights, vector<vector<vector<int>>> buttons) {
	int result = 0;
	for (int t = 0; t < lights.size(); ++t) {
		result += part_one_solve(lights[t], buttons[t]);
	}
	return result;
}

int main(void) {
	vector<string> lights;
	vector<vector<vector<int>>> buttons;
	vector<vector<int>> joltages;

	string line;
	while (getline(cin, line)) {
		//cout << line << endl;
		pair<string, int> l = read_lights(line);
		pair<vector<vector<int>>, int> b = read_buttons(line, l.second);
		vector<int> j = read_joltages(line, b.second);

		lights.push_back(l.first);
		buttons.push_back(b.first);
		joltages.push_back(j);
	}

	int result1 = part_one(lights, buttons);
	cout << "Part one result: " << result1 << endl;
}