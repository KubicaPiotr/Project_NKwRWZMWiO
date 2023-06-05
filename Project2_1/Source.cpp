#include <iostream>
#include <vector>
#include <queue>
//#include <algorithm>
#include <fstream>
using namespace std;
#define INF 1e9

vector<pair<int, int>> graph[100]; // graf reprezentowany przez listę sąsiedztwa
int dist[100]; // tablica przechowująca odległości od wierzchołka startowego

// Funkcja obliczająca najkrótszą drogę z każdego miasta do stolicy (int start = 1)
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
            dijkstra(1, n); // oblicz odległości z wierzchołka 1
            for (int i = 1; i <= n; i++) {
                //if (i != u) {
                    temp_sum += dist[i];
                //}
            }
            if (temp_sum < min_sum) {
                min_sum = temp_sum;
                min_connection = u * (n+1) + w; // zapisz numer połączenia
            }

            // przywróć połączenie
            graph[u].push_back(make_pair(w, weight));
            graph[w].push_back(make_pair(u, weight));
        }
    }
    return min_connection;
}

int main() {

    cout << "Lotnisko w San Escobar pęka w szwach!" << endl;
    cout << "To oczywiscie efekt ogromnego ruchu lotniczego z Polski, jaki nastapil po nawiazaniu stosunkow dyplomatycznych miedzy oboma krajami" << endl;
    cout << "Jako minister transportu republiki postanowilas wybudowac nowe lotnisko dla naszych narodowych linii lotniczych El Niño i to natychmiast." << endl;
    cout << "W tym celu na pas startowy zostanie przerobiony jeden z odcinków sieci autostrad w naszym kraju." << endl;
    cout << "Siec autostrad na planach ma kształt grafu, a wierzcholkami sa poszczegolne miasta." << endl;
    cout << "Algorytm ustali, ktory odcinek sieci autostrad nalezy poswiecic pod pas startowy," << endl;
    cout << "tak aby suma odleglosci ze stolicy kraju, Santo Subito (miasto numer 1), do pozostalych miast pozostala jak najmniejsza." << endl << endl;

    ifstream input_file("graf.txt");

    // zczytaj ilość miast
    int n;
    input_file >> n;

    // zczytaj ilość odcinków autostrad
    int num_edges;
    input_file >> num_edges;

    // zczytaj krawędzie wraz z wagami
    for (int i = 0; i < num_edges; i++) {
        int u, v, weight;
        input_file >> u >> v >> weight;
        graph[u].push_back(make_pair(v, weight));
        graph[v].push_back(make_pair(u, weight));
    }
    /* WPISYWANIE RĘCZNE
    int n, m;
    cout << "Podaj liczbe miast:";
    cin >> n;
    cout << "Podaj liczbe odcinkow autostrad: ";
    cin >> m;

    // wczytaj krawędzie grafu
    cout << endl << "WCZYTAJ ODNCICKI AUTOSTRAD: " << endl;
    for (int i = 0; i < m; i++) {
        cout << endl << "ODCINEK " << i + 1 << ": " << endl;
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
    */
    // znajdź połączenie, które powinno zostać usunięte
    int connection_to_remove = find_connection_to_remove(n);

    // wyznacz numery miast, między którymi znajduje się autostrada do usunięcia
    int u = connection_to_remove / (n+1);
    int w = connection_to_remove % (n+1);

    if (connection_to_remove == -1)
    {
        cout << endl << "Nie mozna usunac zadnego odcinka autostrady" << endl;
    }
    else
        cout << endl << "Odcinek autostrady, ktory powinien zostac usuniety znajduje sie pomiedzy miastami: " << u << " - " << w << endl;

    system("pause");
    return 0;
}