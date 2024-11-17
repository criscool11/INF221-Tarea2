#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Tablas de costos globales
vector<int> cost_insert(26);// Costos de inserción(1 caracter)
vector<int> cost_delete(26);// Costos de eliminación(1 caracter)
vector<vector<int>> cost_replace(26, vector<int>(26)); // Costos de reemplazo (26x26) (2 caracteres)
vector<vector<int>> cost_transpose(26, vector<int>(26)); // Costos de transposición (26x26) (2 caracteres)

// Función para cargar un archivo de costo en un vector o matriz
void cargarCostos(const string &filename, vector<int> &cost_table) {
    ifstream file(filename);
    if (!file) throw runtime_error("Error al abrir " + filename);
    for (int &cost : cost_table) file >> cost;
}

void cargarCostos(const string &filename, vector<vector<int>> &cost_table) {
    ifstream file(filename);
    if (!file) throw runtime_error("Error al abrir " + filename);
    for (auto &row : cost_table)
        for (int &cost : row) file >> cost;
}

// Inicializar las tablas de costo desde archivos
void inicializarCostos() {
    cargarCostos("cost_insert.txt", cost_insert);
    cargarCostos("cost_delete.txt", cost_delete);
    cargarCostos("cost_replace.txt", cost_replace);
    cargarCostos("cost_transpose.txt", cost_transpose);
}

// Funciones de costo
int costo_ins(char b) {
    return cost_insert[b - 'a'];
}

int costo_del(char a) {
    return cost_delete[a - 'a'];
}

int costo_sub(char a, char b) {
    return cost_replace[a - 'a'][b - 'a'];
}

int costo_trans(char a, char b) {
    return cost_transpose[a - 'a'][b - 'a'];
}

// Función para reconstruir la secuencia de operaciones
void reconstruirSecuencia(int i, int j, const vector<vector<int>> &d, const string &s1, const string &s2) {
    vector<string> operaciones;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && s1[i - 1] == s2[j - 1]) {
            // No se realiza ninguna operación
            i--;
            j--;
        } else if (i > 0 && d[i][j] == d[i - 1][j] + costo_del(s1[i - 1])) {
            operaciones.push_back("Eliminar '" + string(1, s1[i - 1]) + "'");
            i--;
        } else if (j > 0 && d[i][j] == d[i][j - 1] + costo_ins(s2[j - 1])) {
            operaciones.push_back("Insertar '" + string(1, s2[j - 1]) + "'");
            j--;
        } else if (i > 0 && j > 0 && d[i][j] == d[i - 1][j - 1] + costo_sub(s1[i - 1], s2[j - 1])) {
            operaciones.push_back("Sustituir '" + string(1, s1[i - 1]) + "' por '" + string(1, s2[j - 1]) + "'");
            i--;
            j--;
        } else if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1] && 
                   d[i][j] == d[i - 2][j - 2] + costo_trans(s1[i - 2], s1[i - 1])) {
            operaciones.push_back("Transponer '" + string(1, s1[i - 2]) + "' y '" + string(1, s1[i - 1]) + "'");
            i -= 2;
            j -= 2;
        }
    }

    // Imprimir las operaciones en orden inverso
    cout << "Secuencia de operaciones:\n";
    for (auto it = operaciones.rbegin(); it != operaciones.rend(); ++it) {
        cout << *it << endl;
    }
}

int damerauLevenshtein_PD(const string& s1, const string& s2) {
    /*
    **
    Parametros: 
    s1 = String para transformar
    s2 = String al que se debe ser transformado
    **
    return d[len1][len2] = retorna el valor que esta en la esquina inferior derecha de la matriz generada
    por utilizar levenshtein, este valor representa la distancia de edición minima
    **
    La función utiliza el enfoque de Programación Dinamica para calcular la distancia de Damerau-Levenshtein
    entre dos strings
    */
    int len1 = s1.size();
    int len2 = s2.size();

    vector<vector<int>> d(len1 + 1, vector<int>(len2 + 1));

    for (int i = 0; i <= len1; i++) {
        d[i][0] = i;
    }
    for (int j = 0; j <= len2; j++) {
        d[0][j] = j;
    }

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = costo_sub(s1[i - 1], s2[j - 1]);
            d[i][j] = min({d[i - 1][j] + costo_del(s1[i - 1]),// Eliminación
                           d[i][j - 1] + costo_ins(s2[j - 1]),// Inserción
                           d[i - 1][j - 1] + cost}); // Sustitución

            // Comprobar la transposición
            if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
                d[i][j] = min(d[i][j], d[i - 2][j - 2] + costo_trans(s1[i - 2], s1[i - 1])); // Transposición
            }
        }
    }
    // Llamar a la función para reconstruir la secuencia de operaciones
    reconstruirSecuencia(len1, len2, d, s1, s2);

    return d[len1][len2];
}

void procesarDataset(const string &filename, vector<double> &tiemposTotales) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return;
    }

    string line;
    cout << "Procesando archivo: " << filename << endl;

    while (getline(file, line)) {
        // Saltar líneas vacías, si las hubiera
        if (line.empty()) {
            continue;
        }

        vector<string> pair;
        stringstream ss(line);
        string word;
        while (ss >> word) {
            pair.push_back(word);
        }

        if (pair.size() == 2) {
            string str1 = pair[0];
            string str2 = pair[1];

            // Calcular el tiempo de la función
            auto start = high_resolution_clock::now();
            int distance = damerauLevenshtein_PD(str1, str2);
            auto stop = high_resolution_clock::now();
            double tiempo = duration<double>(stop - start).count();
            cout << "Distancia Minima de Edicion entre '" << str1 << "' y '" << str2 << "': " << distance << endl;
            cout << "Tiempo: " <<fixed << setprecision(6)<< tiempo << " segundos" << endl << endl;

            // Agregar el tiempo al vector de tiempos totales
            tiemposTotales.push_back(tiempo);
        }
    }

    file.close();
}
int main() {
    inicializarCostos(); // Inicializa los costos necesarios

    //vector<string> filenames = {"caso1PD.txt", "caso2PD.txt", "caso3PD.txt", "caso4PD.txt", "caso5PD.txt"};
    vector<string> filenames = {"caso5PD.txt"};
    vector<double> tiemposTotales;

    for (const string &filename : filenames) {
        procesarDataset(filename, tiemposTotales);
    }

    // Imprimir todos los tiempos totales de forma vertical
    cout << "\nTiempos Totales de Todos los Bloques:" << endl;
    for (size_t i = 0; i < tiemposTotales.size(); ++i) {
        cout <<fixed << setprecision(6) <<tiemposTotales[i] << endl;
    }

    return 0;
}
