Node *new_node(vector<vector<double>> &parent_matrix, vector<int> const &path, int level, int i, int j, int size);
void row_reduction(vector<vector<double>> &reduced_matrix, vector<double> &row, int size);
void col_reduction(vector<vector<double>> &reduced_matrix, vector<double> &col, int size);
double calc_cost(vector<vector<double>> &reduced_matrix, int size);
double solve(vector<vector<double>> &adj_matrix, int source, int target, vector<int> &pickup, vector<int> &delivery, vector<int> &solution, double UB, double LB, int time_limit);
bool already_pickup(int vertex, vector<int> path);
bool is_delivery(int vertex);
void print_path(vector<int> const &list);
void test(vector<vector<double>> &parent_matrix, int size);