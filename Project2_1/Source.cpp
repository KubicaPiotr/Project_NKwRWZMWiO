#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
#define INF 1e9

vector<pair<int, int>> graph[10001]; // graf reprezentowany przez listę sąsiedztwa
int dist[10001]; // tablica przechowująca odległości od wierzchołka startowego

// Funkcja obliczająca odległości z wierzchołka startowego do każdego innego wierzchołka w grafie
void dijkstra(int start, int n) {
    for (int i = 1; i <= n; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, start));

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (auto v : graph[u]) {
            int w = v.first;
            int weight = v.second;

            if (dist[u] + weight < dist[w]) {
                dist[w] = dist[u] + weight;
                pq.push(make_pair(dist[w], w));
            }
        }
    }
}

// Funkcja znajdująca połączenie, które powinno zostać usunięte, aby suma odległości pozostała jak najmniejsza
int find_connection_to_remove(int n) {
    int min_sum = INF, min_connection = -1;
    dijkstra(1, n); // oblicz odległości z wierzchołka startowego (1)
    for (int u = 1; u <= n; u++) {
        for (auto v : graph[u]) {
            int w = v.first;
            int weight = v.second;

            // usuń połączenie
            graph[u].erase(remove_if(graph[u].begin(), graph[u].end(), [&](pair<int, int> p) {
                return p.first == w;
                }), graph[u].end());

            graph[w].erase(remove_if(graph[w].begin(), graph[w].end(), [&](pair<int, int> p) {
                return p.first == u;
                }), graph[w].end());

            int temp_sum = 0;
            dijkstra(u, n); // oblicz odległości z wierzchołka u
            for (int i = 1; i <= n; i++) {
                if (i != u) {
                    temp_sum += dist[i];
                }
            }
            if (temp_sum < min_sum) {
                min_sum = temp_sum;
                min_connection = u * n + w; // zapisz numer połączenia
            }

            // przywróć połączenie
            graph[u].push_back(make_pair(w, weight));
            graph[w].push_back(make_pair(u, weight));
        }
    }
    return min_connection;
}

int main() {
    int n, m;
    cout << "Podaj liczbe miast:";
    cin >> n;
    cout << "Podaj liczbe odcinkow autostrad: ";
    cin >> m;

    // wczytaj krawędzie grafu
    cout << endl<< "WCZYTAJ ODNCICKI AUTOSTRAD: "<<endl;
    for (int i = 0; i < m; i++) {
        cout << endl<<"ODCINEK "<<i+1<< ": "<<endl;
        int u, v, weight;
        cout << "Podaj numer pierwszego miasta na tym odcinku autostrady: ";
        cin >> u;
        cout << "Podaj numer drugiego miasta na tym odcinku autostrady: ";
        cin >> v;
        cout << "Podaj odleglosc pomiedzy miastami: ";
        cin >> weight;
        graph[u].push_back(make_pair(v, weight));
        graph[v].push_back(make_pair(u, weight));
    }

    // znajdź połączenie, które powinno zostać usunięte
    int connection_to_remove = find_connection_to_remove(n);

    // wyznacz numery wierzchołków, między którymi znajduje się połączenie do usunięcia
    int u = connection_to_remove / n;
    int w = connection_to_remove % n;

    cout << "Odcinek autostrady, ktory powinien zostac usuniety znajduje sie pomiedzy miastami: " << u << " - " << w << endl;
    return 0;
}