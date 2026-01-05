#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<queue>
#include<algorithm>

using namespace std;

typedef long long int ll;

vector<bool> find_reachable(int start, vector<vector<int>> graph) {
	int node_count = graph.size();
	vector<bool> reachable(node_count);
	queue<int> q;
	q.push(start);
	reachable[start] = true;

	while (!q.empty()) {
		int u = q.front();
		q.pop();

		for (int v : graph[u]) {
			if (!reachable[v]) {
				reachable[v] = true;
				q.push(v);
			}
		}
	}
	return reachable;
}

int part_one(int start, int end, vector<vector<int>> graph) {
	int INF = (1 << 30);
	int node_count = graph.size();
	vector<int> indegree(node_count, INF);
	vector<bool> reachable = find_reachable(start, graph);

	if (!reachable[end]) return 0;

	for (int i = 0; i < node_count; ++i) {
		if (reachable[i]) indegree[i] = 0;
	}

	for (int i = 0; i < node_count; ++i) {
		if (reachable[i]) {
			for (int child : graph[i]) {
				indegree[child] += 1;
			}
		}
	}

	if (indegree[start] > 0) {
		cout << "It seems the start node (" << start << ") may be inside a cycle?" << endl;
		indegree[start] = 0;
	}
	vector<bool> visited(node_count, false);
	vector<int> result(node_count, 0);
	queue<int> q;
	q.push(start);
	result[start] = 1;
	visited[start] = true;
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		for (int v : graph[u]) {
			indegree[v]--;
			result[v] += result[u];
			if (indegree[v] == 0) {
				q.push(v);
			}
		}
	}
	return result[end];
}

vector<bool> find_reachable_via(int start, int via1, int via2, vector<vector<int>> graph) {
	int INF = (1 << 30);
	int node_count = graph.size();
	vector<int> indegree(node_count, INF);
	vector<bool> reachable = find_reachable(start, graph);

	for (int i = 0; i < node_count; ++i) {
		if (reachable[i]) indegree[i] = 0;
	}

	for (int i = 0; i < node_count; ++i) {
		if (reachable[i]) {
			for (int child : graph[i]) {
				indegree[child] += 1;
			}
		}
	}

	if (indegree[start] > 0) {
		cout << "It seems the start node may be inside a cycle?" << endl;
		indegree[start] = 0;
	}
	vector<bool> result(node_count, false);
	vector<bool> reachable_via1(node_count, false);
	vector<bool> reachable_via2(node_count, false);
	queue<int> q;
	q.push(start);
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		result[u] = reachable_via1[u] && reachable_via2[u];
		for (int v : graph[u]) {
			indegree[v]--;
			if (!reachable_via1[v]) {
				reachable_via1[v] = v == via1 || reachable_via1[u];
			}
			if (!reachable_via2[v]) {
				reachable_via2[v] = v == via2 || reachable_via2[u];
			}
			if (indegree[v] == 0) {
				q.push(v);
			}
		}
	}
	return result;
}

// Find between any ordering
vector<bool> find_between(int start, int end, vector<vector<int>> graph, vector<vector<int>> reverse_graph) {
	vector<bool> reachable_from_start = find_reachable(start, graph);
	vector<bool> reverse_reachable_from_start = find_reachable(start, reverse_graph);
	vector<bool> reachable_from_end = find_reachable(end, graph);
	vector<bool> reverse_reachable_from_end = find_reachable(end, reverse_graph);

	vector<bool> between(graph.size(), false);
	for (int i = 0; i < graph.size(); ++i) {
		bool start_before_end = (reachable_from_start[i] && reverse_reachable_from_end[i]);
		bool end_before_start = (reachable_from_end[i] && reverse_reachable_from_start[i]);
		between[i] = start_before_end || end_before_start;
	}
	return between;
}

ll part_two(int start, int via1, int via2, int end, vector<vector<int>> graph) {
	int node_count = graph.size();

	vector<vector<int>> reverse_graph(node_count, vector<int>());
	for (int u = 0; u < node_count; ++u) {
		for (int v : graph[u]) reverse_graph[v].push_back(u);
	}

	// Nodes reachable from start via the two nodes.
	vector<bool> reachable_via = find_reachable_via(start, via1, via2, graph);
	// Nodes reachable from the end via the two nodes in the reverse graph.
	vector<bool> reverse_reachable_via = find_reachable_via(end, via1, via2, reverse_graph);
	// Nodes placed between the two via nodes.
	vector<bool> between = find_between(via1, via2, graph, reverse_graph);

	vector<bool> keep(node_count, false);
	for (int i = 0; i < node_count; ++i) {
		keep[i] = reachable_via[i] || reverse_reachable_via[i] || between[i];
	}

	// Create graph only consisting of nodes that will be part of a path from start to end.
	vector<vector<int>> pruned_graph(node_count);
	for (int u = 0; u < node_count; ++u) {
		if (keep[u]) {
			vector<int> children;
			for (int v : graph[u]) {
				if (keep[v]) {
					children.push_back(v);
				}
			}
			pruned_graph[u] = children;
		}
	}

	int INF = (1 << 30);
	vector<int> indegree(node_count, INF);

	for (int i = 0; i < node_count; ++i) {
		if (keep[i]) indegree[i] = 0;
	}

	for (int i = 0; i < node_count; ++i) {
		if (keep[i]) {
			for (int child : pruned_graph[i]) {
				indegree[child] += 1;
			}
		}
	}

	if (indegree[start] > 0) {
		cout << "It seems the start node (" << start << ") may be inside a cycle ? " << endl;
		indegree[start] = 0;
	}
	vector<ll> result(node_count, 0);
	queue<int> q;
	q.push(start);
	result[start] = 1;
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		for (int v : pruned_graph[u]) {
			indegree[v]--;
			if (keep[v]) {
				result[v] += result[u];
			}
			if (indegree[v] == 0) {
				q.push(v);
			}
		}
	}

	return result[end];
}

int main(void) {
	string line;
	unordered_map<string, int> name_mapping;
	unordered_map<string, vector<string>> input;
	int next = 0;
	name_mapping["you"] = next++;
	name_mapping["out"] = next++;
	name_mapping["svr"] = next++;
	name_mapping["dac"] = next++;
	name_mapping["fft"] = next++;
	while (getline(cin, line)) {
		int idx = 0;
		string parent;
		vector<string> children;
		for (int i = idx; i < line.size(); ++i) {
			if (i < idx) continue;
			if (line[i] == ':') {
				parent = line.substr(idx, i);
				auto parent_search = name_mapping.find(parent);
				if (parent_search == name_mapping.end()) {
					name_mapping[parent] = next++;
				}
				idx = i + 2; // Skip the whitespace
			}

			if (i == line.size() - 1 || line[i] == ' ') {
				int l = i - idx;
				if (i == line.size() - 1) l = i - idx + 1;
				string child = line.substr(idx, l);
				children.push_back(child);
				auto child_search = name_mapping.find(child);
				if (child_search == name_mapping.end()) {
					name_mapping[child] = next++;
				}
				idx = i + 1;
			}
		}
		input[parent] = children;
	}

	vector<vector<int>> graph(next, vector<int>());
	for (auto& [parent, children] : input) {
		int parent_index = name_mapping.find(parent)->second;
		vector<int> child_indices;
		for (string child : children) {
			child_indices.push_back(name_mapping.find(child)->second);
		}
		graph[parent_index] = child_indices;
	}

	int result1 = part_one(0, 1, graph);
	cout << "Part one result: " << result1 << endl;
	ll result2 = part_two(2, 3, 4, 1, graph);
	cout << "Part two result " << result2 << endl;
}