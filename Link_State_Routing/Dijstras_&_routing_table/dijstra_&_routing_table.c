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

void initialize_spt(int n, shortest_path_tree spt[][n]) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (j == i)
				spt[i][j].cost = 0;
			else
				spt[i][j].cost = -1;

			spt[i][j].node = j;
			spt[i][j].prev_node = -1;
		}
	}
}

void initialize_routing_table(int n, routing_table rt[][n]) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			rt[i][j].destination = j;
			rt[i][j].next_hop = -1;
			rt[i][j].cost = -1;
		}
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

void dispaly_spt(int n, shortest_path_tree spt[][n]) {
	printf("\n\n");
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			printf("%d\t%d\t%d\n\n", spt[i][j].node, spt[i][j].cost, spt[i][j].prev_node);
		printf("\n\n");
	}
}

void dispaly_rt(int n, routing_table rt[][n]) {
	printf("\n\n");
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			printf("%d\t%d\t%d\n\n", rt[i][j].destination, rt[i][j].cost, rt[i][j].next_hop);
		printf("\n\n");
	}
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

void dijstra_all_sources(int n, int adj_matrix[][n], shortest_path_tree spt[][n]) {
	for (int i = 0; i < n; ++i)
		dijstra(n, adj_matrix, i, spt[i]);
}

void create_routing_table_all_sources(int n, routing_table rt[][n], shortest_path_tree spt[][n]) {
	for (int i = 0; i < n; ++i)
		create_routing_table(n, i, rt[i], spt[i]);
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

	shortest_path_tree spt[n][n];	 // output of dijstras algorithm
	initialize_spt(n, spt);
	// dispaly_spt(n, spt);
	dijstra_all_sources(n, adj_matrix, spt);
	printf("\nShortest Path Trees\n\n");
	dispaly_spt(n, spt);

	routing_table rt[n][n];
	initialize_routing_table(n, rt);
	// dispaly_rt(n, rt);
	create_routing_table_all_sources(n, rt, spt);
	printf("\nRouting Tables\n\n");
	dispaly_rt(n, rt);

	return 0;
}
