#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>

using namespace std;

// неориентированный взвешенный

class Graph {
private:
    // Список смежности: хеш-таблица (вершина - соседи с весами)
    unordered_map<int, unordered_map<int, int>> adjacency_list;
    
public:
    void addEdge(int node1, int node2, int weight) {
        adjacency_list[node1][node2] = weight;
        adjacency_list[node2][node1] = weight;
    }
    
    const unordered_map<int, int>& getNeighbors(int node) const {
        static const unordered_map<int, int> empty; // Для случая, если вершины нет
        auto it = adjacency_list.find(node);
        return (it != adjacency_list.end()) ? it->second : empty;
    }
    
    void dijkstra(int start_node) const {
        // Приоритетная очередь для выбора вершины с минимальным расстоянием
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        unordered_map<int, int> distances;
        for (const auto& pair : adjacency_list) {
            distances[pair.first] = numeric_limits<int>::max();
        }
        
        unordered_map<int, int> previous;
        
        distances[start_node] = 0;
        pq.push({0, start_node});
        
        cout << "Начинаем алгоритм Дейкстры с вершины " << start_node << endl;
        
        while (!pq.empty()) {
            int current_node = pq.top().second;
            int current_distance = pq.top().first;
            pq.pop();
            
            // Пропускаем, если нашли более короткий путь до этой вершины
            if (current_distance > distances[current_node]) {
                cout << "  Пропускаем вершину " << current_node 
                    << ", так как уже нашли более короткий путь (" 
                    << distances[current_node] << ")" << endl;
                continue;
            }
            
            cout << "\nОбрабатываем вершину " << current_node 
                << " (расстояние = " << current_distance << ")" << endl;
            
            // Проверяем всех соседей текущей вершины
            for (const auto& neighbor : getNeighbors(current_node)) {
                int neighbor_node = neighbor.first;
                int edge_weight = neighbor.second;
                int distance = current_distance + edge_weight;
                
                cout << "  Проверяем соседа: " << neighbor_node 
                    << " (вес ребра: " << edge_weight 
                    << ", новое расстояние: " << distance 
                    << ", старое расстояние: " << distances[neighbor_node] << ")";
                
                // Если нашли более короткий путь до соседа
                if (distance < distances[neighbor_node]) {
                    distances[neighbor_node] = distance;
                    previous[neighbor_node] = current_node;
                    pq.push({distance, neighbor_node});
                    cout << " -> Обновляем расстояние" << endl;
                } else {
                    cout << " -> Оставляем старое расстояние" << endl;
                }
            }
        }
        
        // Выводим результаты
        cout << "\nРезультаты работы алгоритма Дейкстры:" << endl;
        for (const auto& pair : distances) {
            int node = pair.first;
            int distance = pair.second;
            
            cout << "Вершина " << node << ": расстояние = ";
            if (distance == numeric_limits<int>::max()) {
                cout << "∞ (недостижима)";
            } else {
                cout << distance;
                
                // Восстанавливаем путь
                vector<int> path;
                for (int at = node; at != start_node; at = previous[at]) {
                    path.push_back(at);
                }
                path.push_back(start_node);
                reverse(path.begin(), path.end());
                
                cout << ", путь: ";
                for (size_t i = 0; i < path.size(); ++i) {
                    if (i != 0) cout << " -> ";
                    cout << path[i];
                }
            }
            cout << endl;
        }
    }
    
    // Вывод графа
    void printGraph() const {
        cout << "Структура графа (список смежности):" << endl;
        for (const auto& pair : adjacency_list) {
            int node = pair.first;
            const auto& neighbors = pair.second;
            
            cout << node << " -> ";
            bool first = true;
            for (const auto& neighbor : neighbors) {
                if (!first) cout << ", ";
                cout << neighbor.first << "(" << neighbor.second << ")";
                first = false;
            }
            cout << endl;
        }
    }
};

int main() {
    Graph g;
    
    // Добавляем ребра в граф
    g.addEdge(1, 2, 7);
    g.addEdge(1, 3, 9);
    g.addEdge(1, 6, 14);
    g.addEdge(2, 3, 10);
    g.addEdge(2, 4, 15);
    g.addEdge(3, 4, 11);
    g.addEdge(3, 6, 2);
    g.addEdge(4, 5, 6);
    g.addEdge(5, 6, 9);
    
    // Выводим структуру графа
    g.printGraph();
    cout << endl;
    
    // Запускаем алгоритм Дейкстры с вершины 1
    g.dijkstra(1);
    
    return 0;
}