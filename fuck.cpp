#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include <deque>
using namespace std;

/*--------------*/

set<string> nodeset(vector<vector<string>> graph){
    set<string> count_only;
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < 2; j++){
            count_only.insert(graph[i][j]);
        }
    }
    return count_only;
}

vector<vector<string>> edgelist_fr(string filename) {  //       * COMMAND 1 (done!)

    /* DESC: gets input graph .txt file and uses vectors to convert to an edge list format */
    string s; ifstream t(filename); vector<string> edge_list; set<string> count_only;
    vector<vector<string>> edgelist_fr;

    while (getline(t, s)){ // read file, put in temp edge list vector
        edge_list.push_back(s);
    }

    for (int i = 0; i < edge_list.size(); i++) {
        vector<string> each_edge; stringstream str(edge_list[i]); string s;
        while (getline(str, s, ' ')) { // separate each node and cost
            each_edge.push_back(s); // put them in another vector holding each edge (format: {src, dest, cost})
        }
        edgelist_fr.push_back(each_edge); // all edges are here!
    }

    for (int i = 0; i < edgelist_fr.size(); i++) { // put srcs and dests in a set to count unique nodes
        for (int j = 0; j < 2; j++){
            count_only.insert(edgelist_fr[i][j]);
        }
    }

    cout << "Map added successfully with " << count_only.size() << " nodes and " << edgelist_fr.size() << " edges!\n\n";
    return edgelist_fr;
}

vector<vector<int>> ts_pmo(vector<vector<string>> graph) {

    // do not fucking piss me off. can't I do this algo with the edge list
    set<string> count_only;
    count_only = nodeset(graph);
    int ver = count_only.size();

    vector<vector<int>> matrix(ver, vector<int>(ver, 0));
    vector<string> nodes(count_only.begin(), count_only.end()); 
    int src = 0; int dest = 0;

    for (int i = 0; i < graph.size(); i++){
        auto i_s = find(nodes.begin(), nodes.end(), graph[i][0]);
        auto i_d = find(nodes.begin(), nodes.end(), graph[i][1]);
        if (i_s != nodes.end()) { src = i_s - nodes.begin(); }
        if (i_d != nodes.end()) { dest = i_d - nodes.begin(); }
        int w = stoi(graph[i][2]);

        matrix[src][dest] = w;
        matrix[dest][src] = w;
    }
    return matrix;
}

/* vector<string> commands(string filename) { //                   * COMMAND 0

    vector<string> command_list; string s; ifstream t(filename); 

    while (getline(t, s)){
        command_list.push_back(s);
    }

    string filename_2;
    for (int i = 0; i < command_list.size(); i++){
        if (command_list[i][1] == "1") { edgelist_fr(filename_2); }

        // other commands here!
    }

} */

void shortest_path(vector<vector<string>> graph, string src_, string dest_) { //           * COMMAND 2 (done!)

    set<string> count_only;
    count_only = nodeset(graph);
    int ver = count_only.size();

    vector<string> nodes(count_only.begin(), count_only.end()); 
    int src = 0; int dest = 0;

    auto i_s = find(nodes.begin(), nodes.end(), src_);
    auto i_d = find(nodes.begin(), nodes.end(), dest_);
    if (i_s != nodes.end()) { src = i_s - nodes.begin(); }
    if (i_d != nodes.end()) { dest = i_d - nodes.begin(); }

    int max_int = 100000;
    vector<int> dist_map(ver, max_int);
    vector<int> path(ver, -1);
    dist_map[src] = 0;

    // regular edge relaxation stuff
    for (int i = 0; i < ver - 1; i++) {
        for (int j = 0; j < graph.size(); j++) {
            auto i_x = find(nodes.begin(), nodes.end(), graph[j][0]);
            auto i_y = find(nodes.begin(), nodes.end(), graph[j][1]);

            int x = i_x - nodes.begin(); int y = i_y - nodes.begin();
            string w_ = graph[j][2]; int w = stoi(w_);

            if (dist_map[x] != max_int && dist_map[y] > dist_map[x] + w) {
                dist_map[y] = dist_map[x] + w;
                path[y] = x;
            }
        }
    }

    // just the algo doing its thing for negative cycles (courtesy of the lecture slides)
    for (int i = 0; i < graph.size(); i++) {
        auto i_x = find(nodes.begin(), nodes.end(), graph[i][0]);
        auto i_y = find(nodes.begin(), nodes.end(), graph[i][1]);

        int x = i_x - nodes.begin(); int y = i_y - nodes.begin();
        string w_ = graph[i][2]; int w = stoi(w_);

        if (dist_map[x] != max_int && dist_map[y] > dist_map[x] + w) {
            cout << "There is a negative cycle.";
            return;
        }
    }

    // printing time
    vector<int> route;
    int current = dest;

    while (current != -1){
        route.push_back(current);
        current = path[current];
        }

    reverse(route.begin(), route.end());

    cout << "The shortest path from " << nodes[route[0]] << " to " << nodes[route.back()] << " is ";
    for (int i = 0; i < route.size(); i++){
        cout << nodes[route[i]];
        if (i != route.size() - 1) { cout << " --> "; }
    }
}

void prim(vector<vector<string>> graph) {                        // * COMMAND 3

    // ! here, assume that src = index 1 in nodes

    int max_int = 999999; 
    set<string> count_only;

    count_only = nodeset(graph);
    int ver = count_only.size();

    vector<string> nodes(count_only.begin(), count_only.end());
    vector<int> path(ver, -1); vector<int> cost(ver, max_int); vector<bool> mst(ver, false);
    
    deque<pair<int, int>> pq;
    cost[0] = 0;
    path[0] = -1;
    pq.push_back({0,0});

    while (!pq.empty()) {

        int node = pq.front().second;
        pq.pop_front();

        if (mst[node]) {
            continue;
        }

        mst[node] = true;

        for (int i = 0; i < graph.size(); i++) {
            auto x_ = find(nodes.begin(), nodes.end(), graph[i][0]);
            auto y_ = find(nodes.begin(), nodes.end(), graph[i][1]);

            int x = x_ - nodes.begin();
            int y = y_ - nodes.begin();
            int w = stoi(graph[i][2]);

            if (x == node && !mst[y] && w < cost[y]) {
                pq.push_back({w, y});
                cost[y] = w;
                path[y] = node;
                sort(pq.begin(), pq.end(), [](std::pair<int, int> const &a, std::pair<int, int> const &b) {
                    return a.first < b.first; } );
            }
        }
    }

    int tot = 0;
    for (int i = 0; i < cost.size(); i++){
        if (cost[i] != max_int){tot += cost[i];}
    }

    cout << "The most cost-efficient network with a total cost of " << tot << " is given by the following edges:\n";
    for (int i = 1; i < ver; i++){
        if (path[i] != -1){
            cout << nodes[path[i]] << " -> " << nodes[i] << "\n";
        }
    }
}

void wide(vector<vector<string>> graph, string src_, string dest_) { //   * COMMAND 4

    int min_int = -999999; 
    set<string> count_only;
    count_only = nodeset(graph);
    int ver = count_only.size();

    vector<string> nodes(count_only.begin(), count_only.end()); 
    int src = 0; int dest = 0;

    auto i_s = find(nodes.begin(), nodes.end(), src_);
    auto i_d = find(nodes.begin(), nodes.end(), dest_);

    if (i_s != nodes.end()) { src = i_s - nodes.begin(); }
    if (i_d != nodes.end()) { dest = i_d - nodes.begin(); }

    vector<int> wide_map(ver, min_int);
    vector<int> path(ver, -1);
    deque<pair<int, int>> pq;

    wide_map[src] = 999999; path[src] = -1;
    pq.push_back({src, wide_map[src]});

    while (!pq.empty()) {

        int current = pq.front().first;
        pq.pop_front();

        for (int i = 0; i < graph.size(); i++) {
            auto i_x = find(nodes.begin(), nodes.end(), graph[i][0]);
            auto i_y = find(nodes.begin(), nodes.end(), graph[i][1]);

            int x = i_x - nodes.begin(); int y = i_y - nodes.begin();
            string w_ = graph[i][2]; int w = stoi(w_);
            if (x == current) {
                int distance = max(min(wide_map[x], w), wide_map[y]);
                if (distance > wide_map[y]) {
                    wide_map[y] = distance;
                    path[y] = current;
                    pq.push_back({y, distance});
                    sort(pq.begin(), pq.end(), [](std::pair<int, int> const &a, std::pair<int, int> const &b) {
                    return a.second < b.second; } );
                }
            }
        }
    }

    vector<int> route;
    int current = dest;

    while (current != -1){
        route.push_back(current);
        current = path[current];
        }

    reverse(route.begin(), route.end());

    cout << "The highest-bandwidth path from " << nodes[route[0]] << " to " << nodes[route.back()] << " is ";
    for (int i = 0; i < route.size(); i++){
        cout << nodes[route[i]];
        if (i != route.size() - 1) { cout << " --> "; }
    }
    cout << "\nBottleneck cost:" << wide_map[dest];
}

void held_karp(vector<vector<int>> matrix, vector<vector<string>> graph) {

    // ! src is also index 1 in the nodes set

    set<string> node_set;
    node_set = nodeset(graph);

    vector<string> nodes(node_set.begin(), node_set.end()); 

    int n = node_set.size();
    int n_2 = 1 << n;
    int max_int = 999999;

    vector<vector<long long>> dp(n_2, vector<long long>(n, max_int));
    vector<vector<int>> path(n_2, vector<int>(n, -1));

    dp[1][0] = 0;

    for (int i = 1; i < n_2; i++){
        if ((i & 1) == 0) {
            continue;
        }
        for (int j = 1; j < n; j++){
            if ((i & (1 << j)) == 0) {
                continue;
            }
            int prev_i = i ^ (1 << j);
            for (int k = 0; k < n; k++){
                if (prev_i & (1 << k)){
                    long long cost = dp[prev_i][k] + matrix[k][j];
                    if (cost < dp[i][j]){
                        dp[i][j] = cost;
                        path[i][j] = k;
                    }
                }
            }
        }
    }

    int to_return = (1 << n) - 1;
    long long min_cost = max_int;
    int last = -1;

    for (int j = 1; j < n; j++){
        long long cost = dp[to_return][j] + matrix[j][0];
        if (cost < min_cost) {
            min_cost = cost;
            last = j;
        }
    }

    vector<int> path_print;
    long long fin_return = to_return;
    int current = last;

    while (current != -1) {
        path_print.push_back(current);
        long long prev = path[fin_return][current];
        fin_return = fin_return ^ (1 << current);
        current = prev;
    }

    path_print.push_back(0);
    reverse(path_print.begin(), path_print.end());
    path_print.push_back(0);

    cout << "path is ";
    for (int i = 0; i < path_print.size(); i++){
        cout << nodes[path_print[i]] << " --> ";
    }
}


int main () {

    /*---- MAIN PRINTING STUFF ----*/
    cout << "===========================================\n\n";

    cout << "Software Project 2: CLI-Based Map Application\n" << "Submitted by: Kythe Ashley Desear (202502563)\n\n";
    
    cout << "Available Commands:\n" << "0. Load commands\n" << "1. Load a map\n" << "2. Find the shortest path\n" << "3. Find the most cost-efficient network\n"
            << "4. Find the most efficient maintenance route\n" << "5. Find the highest bandwidth path\n" << "6. Find all pairs shortest path\n\n";
    
    cout << "===========================================\n\n";

    /*---- COMMANDS PROPER ----*/
    // TODO: add error handling after ensuring that every command works


    int choice; string filename = "ouh.txt"; cin >> choice; 
    vector<vector<string>> graph; vector<vector<int>> matrix;
    graph = edgelist_fr(filename); matrix = ts_pmo(graph);
    
    if (choice == 1) { 
        edgelist_fr(filename); 
    }
    else if (choice == 2) {
        string src_; string dest_;
        cout << "Enter source node: "; cin >> src_;
        cout << "Enter destination node: "; cin >> dest_;
        shortest_path(graph, src_, dest_); 
    }
    else if (choice == 3) { prim(graph); }
    else if (choice == 4) { held_karp(matrix, graph); }
    else if (choice == 5) { 
        string src_; string dest_;
        cout << "Enter source node: "; cin >> src_;
        cout << "Enter destination node: "; cin >> dest_;
        wide(graph, src_, dest_); 
    }
    
    /*
    
    if (choice == 0) { commands(filename); } // this filename can be any filename (will implement some cin for this)
    
    
    else if (choice == 6) { not_dih(graph); }

    g++ -o fuck fuck.cpp && ./fuck
    */

}