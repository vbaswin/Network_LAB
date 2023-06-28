#include <stdio.h>

typedef struct SHORTEST_PATH_TREE {
	int node;
	int cost;
	int prev_node;
} shortest_path_tree;

typedef struct ROUTING_TABLE {
	int destination;
	int cost;
	int next_hop;
} routing_table;

void initialize_adj_matrix(int n, int adj_matrix[][n]) {
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			adj_matrix[i][j] = -1;
}

void initialize_spt(int n, int source, shortest_path_tree spt[n]) {
	for (int i = 0; i < n; ++i) {
		if (i == source)
			spt[i].cost = 0;
		else
			spt[i].cost = -1;

		spt[i].node = i;
		spt[i].prev_node = -1;
	}
}

void initialize_routing_table(int n, routing_table rt[]) {
	for (int i = 0; i < n; ++i) {
		rt[i].destination = i;
		rt[i].next_hop = -1;
		rt[i].cost = -1;
	}
}

void display_adj_matrix(int n, int adj_matrix[][n]) {
	printf("\n\n");
	int val;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			val = adj_matrix[i][j];
			if (val == -1)
				printf("-\t");
			else
				printf("%d\t", val);
		}
		printf("\n\n");
	}
}

void dispaly_spt(int n, shortest_path_tree spt[]) {
	printf("\n\n");
	for (int i = 0; i < n; ++i)
		printf("%d\t%d\t%d\n\n", spt[i].node, spt[i].cost, spt[i].prev_node);
}

void dispaly_rt(int n, routing_table rt[]) {
	printf("\n\n");
	for (int i = 0; i < n; ++i)
		printf("%d\t%d\t%d\n\n", rt[i].destination, rt[i].cost, rt[i].next_hop);
}

void add_edge_adj_matrix(int n, int adj_matrix[][n]) {
	int x, y, cost, entries;
	printf("Enter no of entries to add: ");
	scanf("%d", &entries);
	for (int i = 0; i < entries; ++i) {
		printf("Enter row, col and cost: ");
		scanf("%d %d %d", &x, &y, &cost);
		adj_matrix[x][y] = cost;
	}
}

void dijstra(int n, int adj_matrix[][n], int source, shortest_path_tree spt[]) {
	int covered_vertices[n];
	for (int i = 0; i < n; ++i)
		covered_vertices[i] = 0;

	int all_covered = 0, smallest, smallest_node = -1, new_cost;

	while (!all_covered) {
		smallest = 1000000;
		for (int i = 0; i < n; ++i) {
			if (spt[i].cost != -1 && spt[i].cost < smallest && !covered_vertices[i]) {
				smallest = spt[i].cost;
				smallest_node = spt[i].node;
			}
		}
		for (int j = 0; j < n; ++j) {
			if (adj_matrix[smallest_node][j] != -1 && !covered_vertices[j]) {
				new_cost = smallest + adj_matrix[smallest_node][j];
				if (spt[j].cost == -1 || new_cost < spt[j].cost) {
					spt[j].cost = new_cost;
					spt[j].prev_node = smallest_node;
				}
			}
		}
		covered_vertices[smallest_node] = 1;
		// checking whether all the vertices are covered or not
		int p;
		for (p = 0; p < n; ++p) {
			if (!covered_vertices[p])
				break;
		}
		if (p == n)
			all_covered = 1;
	}
}

void create_routing_table(int n, int source, routing_table rt[], shortest_path_tree spt[]) {
	int prev = -1, cur = -1;
	for (int i = 0; i < n; ++i) {
		cur = i;
		prev = i;
		while (cur != source) {
			prev = cur;
			cur = spt[cur].prev_node;
		}
		rt[i].next_hop = prev;
		rt[i].cost = spt[i].cost;
	}
}

int main() {
	freopen("c.in", "r", stdin);
	int n;
	printf("Enter no of nodes: ");
	scanf("%d", &n);
	int adj_matrix[n][n];
	initialize_adj_matrix(n, adj_matrix);
	add_edge_adj_matrix(n, adj_matrix);
	display_adj_matrix(n, adj_matrix);

	shortest_path_tree spt[n];	  // output of dijstras algorithm
	initialize_spt(n, 0, spt);
	// dispaly_spt(n, spt);
	dijstra(n, adj_matrix, 0, spt);
	dispaly_spt(n, spt);

	routing_table rt[n];
	initialize_routing_table(n, rt);
	// dispaly_rt(n, rt);
	create_routing_table(n, 0, rt, spt);
	dispaly_rt(n, rt);

	return 0;
}
