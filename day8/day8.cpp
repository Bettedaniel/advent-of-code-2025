#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>

using namespace std;

typedef long long int ll;
typedef pair<ll, pair<int, int>> pp;

ll distance(ll x1, ll y1, ll z1, ll x2, ll y2, ll z2) {
	return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1);
}

int find_set(int i, vector<int> parents) {
	if (i != parents[i]) {
		parents[i] = find_set(parents[i], parents);
	}
	return parents[i];
}

bool link(int i, int j, vector<int> &parents, vector<int> &ranks) {
	int i_set = find_set(i, parents);
	int j_set = find_set(j, parents);

	if (i_set == j_set) return false;
	if (ranks[i_set] > ranks[j_set]) {
		parents[j_set] = i_set;
		ranks[i_set] += ranks[j_set];
		ranks[j_set] = 0;
		return true;
	} else {
		parents[i_set] = j_set;
		ranks[j_set] += ranks[i_set];
		ranks[i_set] = 0;
		return true;
	}
}

int part_one(vector<int> coords[3], int limit) {
	int boxes = coords[0].size();
	vector<int> parents(boxes);
	vector<int> ranks(boxes);
	priority_queue<pp, vector<pp>, greater<pp>> pqueue;
	for (int i = 0; i < boxes; ++i) {
		parents[i] = i;
		ranks[i] = 1;
		for (int j = i + 1; j < boxes; ++j) {
			ll dist = distance(coords[0][i], coords[1][i], coords[2][i], coords[0][j], coords[1][j], coords[2][j]);
			pp entry = make_pair(dist, make_pair(i, j));
			pqueue.push(entry);
		}
	}

	int original_size = pqueue.size();
	while (!pqueue.empty() && original_size - pqueue.size() < limit) {
		pp closest = pqueue.top();
		pqueue.pop();

		int i = closest.second.first;
		int j = closest.second.second;

		link(i, j, parents, ranks);
	}

	sort(ranks.begin(), ranks.end(), greater<int>());

	return ranks[0] * ranks[1] * ranks[2];
}

ll part_two(vector<int> coords[3]) {
	int boxes = coords[0].size();
	vector<int> parents(boxes);
	vector<int> ranks(boxes);
	priority_queue<pp, vector<pp>, greater<pp>> pqueue;
	for (int i = 0; i < boxes; ++i) {
		parents[i] = i;
		ranks[i] = 1;
		for (int j = i + 1; j < boxes; ++j) {
			ll dist = distance(coords[0][i], coords[1][i], coords[2][i], coords[0][j], coords[1][j], coords[2][j]);
			pp entry = make_pair(dist, make_pair(i, j));
			pqueue.push(entry);
		}
	}

	int original_size = pqueue.size();

	pair<int, int> last_box;
	while (!pqueue.empty()) {
		pp closest = pqueue.top();
		pqueue.pop();

		int i = closest.second.first;
		int j = closest.second.second;

		if (link(i, j, parents, ranks)) {
			last_box = make_pair(i, j);
		}
	}

	return (ll) coords[0][last_box.first] * (ll) coords[0][last_box.second];
}

int main(void) {
	string line;
	vector<string> input;
	while (cin >> line) {
		input.push_back(line);
	}

	vector<int> coords[3] = { vector<int>(input.size()), vector<int>(input.size()), vector<int>(input.size()) };
	for (int i = 0; i < input.size(); ++i) {
		string line = input[i];
		int next_start = 0;
		for (int j = 0; j < 3; ++j) {
			int num = 0;
			for (int k = next_start; k < line.size(); ++k) {
				if (line[k] == ',') {
					next_start = k + 1;
					break;
				} else {
					num = num * 10 + (line[k] - '0');
				}
			}
			coords[j][i] = num;
		}
	}

	int result1 = part_one(coords, 1000);
	cout << "Part one result: " << result1 << endl;

	ll result2 = part_two(coords);
	cout << "Part two result: " << result2 << endl;
}