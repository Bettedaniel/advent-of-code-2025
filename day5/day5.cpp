#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

typedef long long int ll;

int part_one(vector<ll> range_starts, vector<ll> range_ends, vector<ll> ingredients) {
	int result = 0;
	for (int i = 0; i < ingredients.size(); ++i) {
		ll ingredient = ingredients[i];
		for (int j = 0; j < range_starts.size(); ++j) {
			ll range_start = range_starts[j];
			ll range_end = range_ends[j];
			if (range_start <= ingredient && ingredient <= range_end) {
				result++;
				break;
			}
		}
	}
	return result;
}

ll part_two(vector<ll> range_starts, vector<ll> range_ends) {
	vector<pair<ll, ll>> ranges(range_starts.size());
	for (int i = 0; i < ranges.size(); ++i) {
		ranges[i] = make_pair(range_starts[i], range_ends[i]);
	}

	sort(ranges.begin(), ranges.end());

	vector<pair<ll, ll>> reduced_ranges;

	int i = 0;
	int j = 1;
	ll start = ranges[i].first;
	ll end = ranges[i].second;
	while (i < ranges.size()) {
		while (j < ranges.size()) {
			ll j_start = ranges[j].first;
			ll j_end = ranges[j].second;
			if (end >= j_start) {
				end = max(end, j_end);
				++j;
			} else {
				reduced_ranges.push_back(make_pair(start, end));
				start = j_start;
				end = j_end;
				i = j;
				++j;
			}
		}
		if (j >= ranges.size()) {
			reduced_ranges.push_back(make_pair(start, end));
			break;
		}
	}

	ll result = 0;
	for (int k = 0; k < reduced_ranges.size(); ++k) {
		ll addition = reduced_ranges[k].second - reduced_ranges[k].first + 1;
		result += addition;
	}

	return result;
}

int main(void) {
	vector<ll> range_starts;
	vector<ll> range_ends;
	vector<ll> ingredients;

	string input;
	while (cin >> input) {
		bool is_range = false;
		ll range_start = 0;
		ll num = 0;
		for (int i = 0; i < input.size(); ++i) {
			if (input[i] == '-') {
				range_start = num;
				num = 0;
				is_range = true;
			} else {
				num = num * 10 + (input[i] - '0');
			}
		}

		if (is_range) {
			range_starts.push_back(range_start);
			range_ends.push_back(num);
		} else {
			ingredients.push_back(num);
		}
	}

	int result1 = part_one(range_starts, range_ends, ingredients);
	cout << "Part one result: " << result1 << endl;

	ll result2 = part_two(range_starts, range_ends);
	cout << "Part two result: " << result2 << endl;
}