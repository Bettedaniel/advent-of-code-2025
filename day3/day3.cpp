#include<iostream>
#include<vector>

using namespace std;

typedef long long int ll;

ll solve(string battery, int turns) {
	if (turns == 0) return 0;
	vector<ll> previous(battery.size());
	vector<ll> next(battery.size());
	previous[0] = battery[0] - '0';
	for (int i = 1; i < battery.size(); ++i) {
		ll joltage = battery[i] - '0';
		previous[i] = max(previous[i-1], joltage);
	}

	if (turns == 1) return previous[battery.size() - 1];
	for (int i = 2; i <= turns; ++i) {
		ll joltage = previous[i - 2] * 10 + (battery[i - 1] - '0');
		next[i - 1] = joltage;
		for (int j = i; j < battery.size(); ++j) {
			ll joltage = previous[j - 1] * 10 + (battery[j] - '0');
			next[j] = max(next[j - 1], joltage);
		}

		for (int i = 0; i < battery.size(); ++i) {
			previous[i] = next[i];
			next[i] = 0;
		}
	}
	return previous[battery.size() - 1];
}

ll solve(vector<string> batteries, int turns) {
	ll result = 0;
	for (int i = 0; i < batteries.size(); ++i) {
		ll result2 = solve(batteries[i], turns);
		result += result2;
	}
	return result;
}

int main(void) {
	string line;
	vector<string> batteries;
	while (cin >> line) {
		batteries.push_back(line);
	}

	ll result1 = solve(batteries, 2);
	cout << "Part 1 result: " << result1 << endl;
	ll result2 = solve(batteries, 12);
	cout << "Part 2 result: " << result2 << endl;
}