#include<iostream>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;

typedef long long int ll;

ll part_one() {
	string line;
	vector<int> numbers;
	vector<char> operators;
	while (cin >> line) {
		if (line[0] < '0' || line[0] > '9') {
			operators.push_back(line[0]);
		} else {
			int num = 0;
			for (int i = 0; i < line.size(); ++i) {
				num = num * 10 + (line[i] - '0');
			}
			numbers.push_back(num);
		}
	}

	ll result = 0;
	for (int i = 0; i < operators.size(); ++i) {
		char oper = operators[i];
		if (oper == '*') {
			ll calculation = 1;
			for (int j = i; j < numbers.size(); j += operators.size()) {
				calculation *= numbers[j];
			}
			result += calculation;
		} else if (oper == '+') {
			ll calculation = 0;
			for (int j = i; j < numbers.size(); j += operators.size()) {
				calculation += numbers[j];
			}
			result += calculation;
		}
	}
	return result;
}

ll part_two() {
	string line;
	vector<string> lines;
	while (getline(cin, line)) {
		lines.push_back(line);
	}

	vector<char> operators;
	vector<int> column_starts;
	string last_line = lines[lines.size() - 1];
	for (int i = 0; i < last_line.size(); ++i) {
		char c = last_line[i];
		if (c == '*' || c == '+') {
			operators.push_back(c);
			column_starts.push_back(i);
		}
	}
	column_starts.push_back(lines[0].size() + 1);

	vector<vector<ll>> multiplications;
	vector<vector<ll>> additions;
	for (int i = 1; i < column_starts.size(); ++i) {
		int i_prev = column_starts[i - 1];
		int i_next = column_starts[i];
		int num_str_length = i_next - 1 - i_prev;
		vector<string> num_strs(lines.size() - 1);
		for (int j = 0; j < lines.size() - 1; ++j) {
			num_strs[j] = lines[j].substr(i_prev, num_str_length);
		}
		vector<ll> numbers;
		for (int j = 0; j < num_str_length; ++j) {
			ll num = 0;
			for (int k = 0; k < lines.size() - 1; ++k) {
				if (num_strs[k][j] >= '0' && num_strs[k][j] <= '9') {
					num = num * 10 + (num_strs[k][j] - '0');
				}
			}
			numbers.push_back(num);
		}
		if (operators[i - 1] == '*') {
			multiplications.push_back(numbers);
		} else if (operators[i - 1] == '+') {
			additions.push_back(numbers);
		}
	}

	ll result = 0;
	for (int i = 0; i < multiplications.size(); ++i) {
		ll mult = 1;
		for (int j = 0; j < multiplications[i].size(); ++j) {
			mult *= multiplications[i][j];
		}
		result += mult;
	}

	for (int i = 0; i < additions.size(); ++i) {
		ll add = 0;
		for (int j = 0; j < additions[i].size(); ++j) {
			add += additions[i][j];
		}
		result += add;
	}

	return result;
}

int main(void) {
	//ll result1 = part_one();
	//cout << "Part one result: " << result1 << endl;

	ll result2 = part_two();
	cout << "Part two result: " << result2 << endl;
}