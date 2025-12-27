#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<cstdlib>

using namespace std;

typedef long long int ll;

class Edge {
	public:
		int y1;
		int x1;
		int y2;
		int x2;

		Edge(int y1, int x1, int y2, int x2) : y1(y1), x1(x1), y2(y2), x2(x2) {}

		bool is_horizontal() const {
			return y1 == y2;
		}

		bool operator<(const Edge& other) const {
			return min(y1, y2) < min(other.y1, other.y2);
		}

};

class Span {
	public:
		int a;
		int b;
		Span(int a, int b) : a(a), b(b) {}

		Span merge(const Span& other) const {
			if ((other.a <= b && other.a >= a) || (a <= other.b && a >= other.a)) {
				return Span(min(a, other.a), max(b, other.b));
			}
			return Span(-1, -1);
		}

		bool operator<(const Span& other) const {
			if (a == other.a) {
				b < other.b;
			}
			return a < other.a;
		}
};

class QEntry {
	public:
		int x_min;
		int y_min;
		int x_max;
		int y_max;
		ll area;
		QEntry(int x_min, int y_min, int x_max, int y_max) : x_min(x_min), y_min(y_min), x_max(x_max), y_max(y_max), area((ll)(y_max - y_min + 1)* (ll)(x_max - x_min + 1)) {}

		bool operator<(const QEntry& other) const {
			return area < other.area;
		}
};

ll part_one(vector<pair<int, int>> input) {
	ll mx = 0;
	for (int i = 0; i < input.size(); ++i) {
		for (int j = i + 1; j < input.size(); ++j) {
			mx = max(mx, abs( (ll) (input[i].first - input[j].first + 1) * (ll) (input[i].second - input[j].second + 1)));
		}
	}
	return mx;
}

/*
* This is used in the part_two solution, which is not very quick (Takes minutes to run).
* A possible improvement could be to improve this part and not run through all y between y_min and y_max.
* Instead another quicker look-up structure could be built (Perhaps an interval tree).
*/
bool is_valid(int y_min, int y_max, int x_min, int x_max, vector<vector<Span>>& indexed_spans) {
	for (int y = y_min; y <= y_max; ++y) {
		vector<Span> spans = indexed_spans[y];
		int span_counter = 0;
		for (int i = 0; i < spans.size(); ++i) {
			Span span = spans[i];
			if (x_min >= span.a && x_max <= span.b) {
				++span_counter;
			}
		}
		if (span_counter != 1) return false;
	}
	return true;
}

ll basic_find_result(vector<pair<int, int>> &input, vector<vector<Span>> &indexed_spans) {
	ll result = 0;
	for (int i = 0; i < input.size(); ++i) {
		for (int j = i + 1; j < input.size(); ++j) {
			int y_min = min(input[i].first, input[j].first);
			int y_max = max(input[i].first, input[j].first);
			int x_min = min(input[i].second, input[j].second);
			int x_max = max(input[i].second, input[j].second);

			ll area = (ll)(y_max - y_min + 1) * (ll)(x_max - x_min + 1);

			if (area > result) {
				if (is_valid(y_min, y_max, x_min, x_max, indexed_spans)) {
					result = area;
				}
			}
		}
	}
	return result;
}

ll pq_find_result(vector<pair<int, int>>& input, vector<vector<Span>>& indexed_spans) {
	priority_queue<QEntry> pq;

	for (int i = 0; i < input.size(); ++i) {
		for (int j = i + 1; j < input.size(); ++j) {
			int y_min = min(input[i].first, input[j].first);
			int y_max = max(input[i].first, input[j].first);
			int x_min = min(input[i].second, input[j].second);
			int x_max = max(input[i].second, input[j].second);

			pq.push(QEntry(x_min, y_min, x_max, y_max));
		}
	}

	while (!pq.empty()) {
		QEntry e = pq.top();
		pq.pop();
		if (is_valid(e.y_min, e.y_max, e.x_min, e.x_max, indexed_spans)) {
			return e.area;
		}
	}
	return 0;
}

ll part_two(vector<pair<int, int>> input) {
	int reds = input.size();
	vector<Edge> edges;
	int min_row = 0;
	int max_row = 2 << 30;
	for (int i = 1; i <= reds; ++i) {
		pair<int, int> before = input[i - 1];
		pair<int, int> next = input[i % reds];

		min_row = min(min_row, min(before.first, next.first));
		max_row = max(max_row, max(before.first, next.first));

		edges.push_back(Edge(before.first, before.second, next.first, next.second));
	}

	vector<vector<Edge>> indexed_horizontals(max_row + 1, vector<Edge>());
	vector<Edge> verticals;
	for (int i = 0; i < edges.size(); ++i) {
		Edge edge = edges[i];
		if (edge.is_horizontal()) {
			indexed_horizontals[edge.y1].push_back(edge);
		} else {
			verticals.push_back(edge);
		}
	}

	sort(verticals.begin(), verticals.end());

	vector<vector<Span>> indexed_spans(max_row + 1, vector<Span>());
	for (int y = min_row; y <= max_row; ++y) {
		vector<int> intersections;
		for (int i = 0; i < verticals.size(); ++i) {
			Edge v = verticals[i];
			if (y >= min(v.y1, v.y2) && y < max(v.y1, v.y2)) {
				intersections.push_back(v.x1);
			}
		}
		sort(intersections.begin(), intersections.end());
		vector<Span> spans;
		for (int i = 0; i < intersections.size(); i += 2) {
			int xL = intersections[i];
			int xR = intersections[i + 1];
			spans.push_back(Span(xL, xR));
		}

		vector<Edge> horizontals = indexed_horizontals[y];
		for (int i = 0; i < horizontals.size(); ++i) {
			Edge h = horizontals[i];
			spans.push_back(Span(min(h.x1, h.x2), max(h.x1, h.x2)));
		}

		sort(spans.begin(), spans.end());

		vector<Span> total_spans;
		if (spans.size() > 0) {
			int idx = 1;
			Span current = spans[0];
			while (idx < spans.size()) {
				Span merged = current.merge(spans[idx]);
				if (merged.a == -1) {
					total_spans.push_back(current);
					current = spans[idx];
				} else {
					current = merged;
				}
				++idx;
			}
			total_spans.push_back(current);
		}

		indexed_spans[y] = total_spans;
	}

	//return basic_find_result(input, indexed_spans);
	return pq_find_result(input, indexed_spans);
}

int main(void) {
	string line;
	vector<pair<int, int>> input;
	while (cin >> line) {
		pair<int, int> pos;
		int num = 0;
		for (int i = 0; i < line.size(); ++i) {
			if (line[i] == ',') {
				pos.second = num;
				num = 0;
			} else {
				num = num * 10 + (line[i] - '0');
			}
		}
		pos.first = num;
		input.push_back(pos);
	}

	ll result1 = part_one(input);
	cout << "Part one result: " << result1 << endl;

	ll result2 = part_two(input);
	cout << "Part two result: " << result2 << endl;
}