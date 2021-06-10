#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <cstring>
#include <float.h>
#include <unistd.h>

#define INF DBL_MAX

using namespace std;

struct Node {
    vector<int> path;
    vector<vector<double>> reduced_matrix;
    double cost;
    int vertex;
    int level;
    // stores 0 if there is no predecessor or the pred number
    int pred;
};

struct cmp {
    bool operator() (const Node *lhs, const Node *rhs) const {
        return lhs->cost > rhs->cost;
    }
};

void test(vector<vector<double>> &parent_matrix, int size) {
    Node *node = new Node;
    node->reduced_matrix = parent_matrix;
    // copy(parent_matrix.begin(), parent_matrix.end(), node->reduced_matrix.begin());

    cout << "Reduced copy" << endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << node->reduced_matrix[i][j] << " ";
        } 
        cout << endl;
    }
}

Node *new_node(vector<vector<double>> &parent_matrix, vector<int> const &path, int level, int i, int j, int size) {
    Node *node = new Node;
    node->path = path;
    
    node->path.push_back(j);

    node->reduced_matrix = parent_matrix;

    for (int k = 0; level != 0 && k < size; k++) {
        node->reduced_matrix[i][k] = INF;
        node->reduced_matrix[k][j] = INF;
    }

    node->reduced_matrix[j][0] = INF;
    node->level = level;
    node->vertex = j;

    return node;
}

void row_reduction(vector<vector<double>> &reduced_matrix, vector<double> &row, int size) {
    fill_n(row.begin(), size, INF);

    // find min of each row
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (reduced_matrix[i][j] < row[i]) {
                row[i] = reduced_matrix[i][j];
            }
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (reduced_matrix[i][j] != INF && row[i] != INF) {
                reduced_matrix[i][j] -= row[i];
            }
        }
    }
}

void col_reduction(vector<vector<double>> &reduced_matrix, vector<double> &col, int size) {
    fill_n(col.begin(), size, INF);

    // find min of each column
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (reduced_matrix[i][j] < col[j]) {
                col[j] = reduced_matrix[i][j];
            }
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (reduced_matrix[i][j] != INF && col[j] != INF) {
                reduced_matrix[i][j] -= col[j];
            }
        }
    }
}

double calc_cost(vector<vector<double>> &reduced_matrix, int size) {
    double cost = 0.0;
    vector<double> row(size);
    vector<double> col(size);

    row_reduction(reduced_matrix, row, size);
    col_reduction(reduced_matrix, col, size);

    for (int i = 0; i < size; i++) {
        if (row[i] != INF) {
            cost += row[i];
        } else {
            cost += 0;
        }
        if (col[i] != INF) {
            cost += col[i];
        } else {
            cost += 0;
        }
    }

    return cost;
}

void print_path(vector<int> const &list) {
    for (int i = 0; i < list.size() - 1; i++) {
        cout << list[i] + 1 << "—>";
    }
    cout << list[list.size() - 1] + 1 << endl;
}

bool is_delivery(int vertex, vector<int> &delivery) {
    return find(delivery.begin(), delivery.end(), vertex) != delivery.end();
}

bool already_pickup(int vertex, vector<int> path, vector<int> &pickup, vector<int> &delivery) {
    auto it = find(delivery.begin(), delivery.end(), vertex);
    int pickup_vertex = -1;

    if (it != delivery.end()) {
        int index = it - pickup.begin();
        // cout << "index: " << index;
        pickup_vertex = pickup[index] - 1;
    } else {
        cout << "pickup index not found in delivery vector!" << endl;
        return false;
    }
    // cout << " vertex: " << vertex << " pickup_vertex: " << pickup_vertex << " path: ";
    // print_path(path);
    return find(path.begin(), path.end(), pickup_vertex) != path.end();
}

double solve(vector<vector<double>> &adj_matrix, int source, int target, vector<int> &pickup, vector<int> &delivery) {
    
    priority_queue<Node *, vector<Node*>, cmp> p_queue;
    vector<int> v;
    int size = adj_matrix.size();

    source = source - 1;
    target = target - 1;

    Node *root = new_node(adj_matrix, v, 0, -1, source, size);
    root->cost = calc_cost(root->reduced_matrix, size);

    p_queue.push(root);

    while (!p_queue.empty()) {
        // because of cmp, the top node in the queue has the min cost;
        Node *min = p_queue.top();

        // remove it from the live node list
        p_queue.pop();

        int i = min->vertex;

        // if all locations are already visited except the target
        if (min->level == size - 1) {
            print_path(min->path);
            return min->cost;
        }
        if (min->level == size - 2) {
            // add the target to the current path
            Node *child = new_node(min->reduced_matrix, min->path, min->level + 1, i, target, size);
            child->cost = min->cost + min->reduced_matrix[i][target] + calc_cost(child->reduced_matrix, size);
            p_queue.push(child);
        }

        for (int j = 0; j < size; j++) {
            if (min->reduced_matrix[i][j] != INF && j != target) {
                if (is_delivery(j, delivery)) {
                    if (already_pickup(j, min->path, pickup, delivery)) {
                        // cout << "is_delivery: " << j << "curr_path: ";
                        Node *child = new_node(min->reduced_matrix, min->path, min->level + 1, i, j, size);
                        child->cost = min->cost + min->reduced_matrix[i][j] + calc_cost(child->reduced_matrix, size);
                        p_queue.push(child);
                    } else {
                        continue;
                    }
                } else {
                    Node *child = new_node(min->reduced_matrix, min->path, min->level + 1, i, j, size);
                    child->cost = min->cost + min->reduced_matrix[i][j] + calc_cost(child->reduced_matrix, size);
                    p_queue.push(child);
                }
            }
        }

        // print_path(min->path);
        delete min;  
    }
    Node *min = p_queue.top();
    print_path(min->path);
    return min->cost;
}