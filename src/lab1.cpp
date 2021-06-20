#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <utility>
#include <cstring>
#include <float.h>
#include <algorithm> 

#define INF DBL_MAX

using namespace std;

struct Node {
    vector<int> path;
    vector<vector<double>> reduced_matrix;
    double cost;
    int vertex;
    int level;
};



struct cmp {
    bool operator() (const Node *lhs, const Node *rhs) const {
        return lhs->cost > rhs->cost;
    }
};

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

bool is_delivery(int vertex, vector<int> delivery) {
    if (find(delivery.begin(), delivery.end(), vertex) != delivery.end() && vertex != 1) {
        return true;
    } else {
        return false;
    }
}

bool already_pickup(int vertex, vector<int> path) {
    return find(path.begin(), path.end(), vertex - 1) != path.end();
}

double solve_w_stack(vector<vector<double>> &adj_matrix, int source, int target, vector<int> &pickup, vector<int> &delivery, vector<int> &solution, double UB, double LB, int time_limit) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();

    priority_queue<Node *, vector<Node*>, cmp> p_queue;
    stack<Node *, vector<Node*>> q;
    vector<int> v;
    int size = adj_matrix.size();

    source = source - 1;
    target = target - 1;

    Node *root = new_node(adj_matrix, v, 0, -1, source, size);
    root->cost = calc_cost(root->reduced_matrix, size);

    // p_queue.push(root);
    q.push(root);
    duration<double, std::milli> delta = high_resolution_clock::now() - t1;
        while (!q.empty() && delta.count()/1000 <= (double)time_limit) {
        // because of cmp, the top node in the queue has the min cost;
        Node *min = q.top();
        // remove it from the live node list
        q.pop();
        cout << "size: " << q.size() << " UB: " << UB << " curr_cost: " << min->cost << " curr_level: " << min->level << endl;

        int i = min->vertex;

        // if all locations are already visited except the target
        if (min->level == size - 1) {
            if (min->cost - adj_matrix[i][0] < LB) {
                solution = min->path;
                LB = min->cost - adj_matrix[i][0];
                UB = min->cost - adj_matrix[i][0];
            }
        }
        if (min->level == size - 2) {
            // add the target to the current path
            Node *child = new_node(min->reduced_matrix, min->path, min->level + 1, i, target, size);
            child->cost = min->cost + min->reduced_matrix[i][target] + calc_cost(child->reduced_matrix, size);
            if (child->cost - adj_matrix[target][0] < UB) {
                q.push(child);
            }
        }

        for (int j = 0; j < size; j++) {
            if (min->reduced_matrix[i][j] != INF && j != target) {
                if (is_delivery(j, delivery)) {
                    if (already_pickup(j, min->path)) {
                        Node *child = new_node(min->reduced_matrix, min->path, min->level + 1, i, j, size);
                        child->cost = min->cost + min->reduced_matrix[i][j] + calc_cost(child->reduced_matrix, size);
                        if (child->cost - adj_matrix[j][0] < UB) {
                            q.push(child);
                        }
                    } else {
                        continue;
                    }
                } else {
                    Node *child = new_node(min->reduced_matrix, min->path, min->level + 1, i, j, size);
                    child->cost = min->cost + min->reduced_matrix[i][j] + calc_cost(child->reduced_matrix, size);
                    if (child->cost - adj_matrix[j][0] < UB) {
                        q.push(child);
                    }
                }
            }
        }
        delete min;
        delta = high_resolution_clock::now() - t1;
    }
    return LB;
}

double solve_w_pq(vector<vector<double>> &adj_matrix, int source, int target, vector<int> &pickup, vector<int> &delivery, vector<int> &solution, double UB, double LB, int time_limit) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();
    unsigned long int n_nodes = 0, aux = 0;


    priority_queue<Node *, vector<Node*>, cmp> p_queue;
    vector<int> v;
    int size = adj_matrix.size();

    source = source - 1;
    target = target - 1;

    Node *root = new_node(adj_matrix, v, 0, -1, source, size);
    root->cost = calc_cost(root->reduced_matrix, size);

    p_queue.push(root);
    duration<double, std::milli> delta = high_resolution_clock::now() - t1;
    while (!p_queue.empty() && delta.count()/1000 <= (double)time_limit) {
        aux = p_queue.size();
        if (aux > n_nodes) {
            n_nodes = aux;
        }
        // because of cmp, the top node in the queue has the min cost;
        Node *min = p_queue.top();
        // remove it from the live node list
        p_queue.pop();
        // cout << "size: " << p_queue.size() << " UB: " << UB << " curr_cost: " << min->cost << " curr_level: " << min->level << endl;
        // cout << " UB: " << UB << " curr_cost: " << min->cost << " curr_level: " << min->level << endl ;
        int i = min->vertex;

        if (min->level == size - 1) {
            LB = min->cost - adj_matrix[target][source];
            UB = min->cost - adj_matrix[target][source];
            solution = min->path;
            cout << "n_nodes: " << n_nodes << endl;
            return min->cost - adj_matrix[target][source];
        }

        // if all locations are already visited except the target
        if (min->level == size - 2) {
            // calc cost and add the target to the current path
            Node *child = new_node(min->reduced_matrix, min->path, min->level + 1, i, target, size);
            child->cost = min->cost + min->reduced_matrix[i][target] + calc_cost(child->reduced_matrix, size);
            solution = child->path;
            // cout << " UB: " << UB << " curr_cost: " << child->cost << " curr_level: " << " cost: " << adj_matrix[target][source] << child->level << endl ;
            if (child->cost - adj_matrix[target][source] < UB) {
                cout << "min: " << min->vertex << endl;
                p_queue.push(child);
                LB = child->cost - adj_matrix[target][source];
                UB = child->cost - adj_matrix[target][source];
            }
            delete min;
            continue;    
        }

        for (int j = 0; j < size; j++) {
            if (min->reduced_matrix[i][j] != INF && j != target) {
                if (is_delivery(j, delivery)) {
                    if (already_pickup(j, min->path)) {
                        Node *child = new_node(min->reduced_matrix, min->path, min->level + 1, i, j, size);
                        child->cost = min->cost + min->reduced_matrix[i][j] + calc_cost(child->reduced_matrix, size);
                        if (child->cost - adj_matrix[j][source] < UB) {
                            p_queue.push(child);
                        }
                    } else {
                        continue;
                    }
                } else {
                    Node *child = new_node(min->reduced_matrix, min->path, min->level + 1, i, j, size);
                    child->cost = min->cost + min->reduced_matrix[i][j] + calc_cost(child->reduced_matrix, size);
                    if (child->cost - adj_matrix[j][source] < UB) {
                        p_queue.push(child);
                    }
                }
            }
        }
        delete min;
        delta = high_resolution_clock::now() - t1;
    }
    cout << "2 n_nodes: " << n_nodes << endl;
    return LB;
}