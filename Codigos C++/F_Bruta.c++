#include <bits/stdc++.h>
#include <chrono>
using namespace std::chrono;
using namespace std;

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

// Estructura para almacenar el resultado con el costo y las operaciones
struct Result {
    int costo;
    vector<string> operaciones;
};

// Función recursiva para generar todas las operaciones posibles y calcular los costos
Result calcularCosto(string s1, string s2, int index1, int index2) {
    // Caso base: Si ambos strings están vacíos, el costo es 0
    if (index1 == s1.size() && index2 == s2.size()) {
        return {0, {}};
    }
    
    // Si solo queda un string
    if (index1 == s1.size()) {
        Result insert_result = calcularCosto(s1, s2, index1, index2 + 1);  // Insertar
        insert_result.costo += costo_ins(s2[index2]);
        insert_result.operaciones.push_back("Insertar '" + string(1, s2[index2]) + "'");
        return insert_result;
    }
    if (index2 == s2.size()) {
        Result delete_result = calcularCosto(s1, s2, index1 + 1, index2);  // Eliminar
        delete_result.costo += costo_del(s1[index1]);
        delete_result.operaciones.push_back("Eliminar '" + string(1, s1[index1]) + "'");
        return delete_result;
    }
    
    // Si los caracteres son iguales, simplemente continuar sin costo
    if (s1[index1] == s2[index2]) {
        return calcularCosto(s1, s2, index1 + 1, index2 + 1);
    }

    // Si los caracteres son diferentes, considerar todas las opciones
    Result min_result = {INT_MAX, {}};

    // Caso de sustitución
    Result sub_result = calcularCosto(s1, s2, index1 + 1, index2 + 1);
    sub_result.costo += costo_sub(s1[index1], s2[index2]);
    sub_result.operaciones.push_back("Sustituir '" + string(1, s1[index1]) + "' por '" + string(1, s2[index2]) + "'");
    if (sub_result.costo < min_result.costo) {
        min_result = sub_result;
    }

    // Caso de inserción
    Result ins_result = calcularCosto(s1, s2, index1, index2 + 1);
    ins_result.costo += costo_ins(s2[index2]);
    ins_result.operaciones.push_back("Insertar '" + string(1, s2[index2]) + "'");
    if (ins_result.costo < min_result.costo) {
        min_result = ins_result;
    }

    // Caso de eliminación
    Result del_result = calcularCosto(s1, s2, index1 + 1, index2);
    del_result.costo += costo_del(s1[index1]);
    del_result.operaciones.push_back("Eliminar '" + string(1, s1[index1]) + "'");
    if (del_result.costo < min_result.costo) {
        min_result = del_result;
    }

    // Caso de transposición (si es posible)
    if (index1 + 1 < s1.size() && index2 + 1 < s2.size() && s1[index1] == s2[index2 + 1] && s1[index1 + 1] == s2[index2]) {
        Result trans_result = calcularCosto(s1, s2, index1 + 2, index2 + 2);
        trans_result.costo += costo_trans(s1[index1], s1[index1 + 1]);
        trans_result.operaciones.push_back("Transponer '" + string(1, s1[index1]) + "' y '" + string(1, s1[index1 + 1]) + "'");
        if (trans_result.costo < min_result.costo) {
            min_result = trans_result;
        }
    }

    return min_result;
}

// Función de fuerza bruta para calcular la distancia de Damerau-Levenshtein
int damerauLevenshtein_FuerzaBruta(const string& s1, const string& s2, int i, int j) {
    /*
    **
    Parametros: 
    s1 = String para transformar
    s2 = String al que se debe ser transformado
    i = indice que indica la posición del string s1
    j = indice que indica la posición del string s2
    **
    return costoMin: Despues de probar todas las soluciones, retorna la mejor solución posible
    **
    La función utiliza el enfoque de Fuyerza Bruta para calcular la distancia de Damerau-Levenshtein
    entre dos strings, este enfoque consiste en generar todas las posibilidades mediante recursion
    */
    // Caso base: si ambas cadenas han sido procesadas completamente
    if (i == s1.size() && j == s2.size()) return 0;
    
    // Caso base: si se alcanza el final de s1, insertar el resto de s2
    if (i == s1.size()) return costo_ins(s2[j]) + damerauLevenshtein_FuerzaBruta(s1, s2, i, j + 1);

    // Caso base: si se alcanza el final de s2, eliminar el resto de s1
    if (j == s2.size()) return costo_del(s1[i]) + damerauLevenshtein_FuerzaBruta(s1, s2, i + 1, j);

    // Operaciones posibles
    int costoMin = INT_MAX;

    // 1. Substitución
    if (i < s1.size() && j < s2.size()) {
        int costo = costo_sub(s1[i], s2[j]) + damerauLevenshtein_FuerzaBruta(s1, s2, i + 1, j + 1);
        costoMin = min(costoMin, costo);
    }

    // 2. Inserción
    if (j < s2.size()) {
        int costo = costo_ins(s2[j]) + damerauLevenshtein_FuerzaBruta(s1, s2, i, j + 1);
        costoMin = min(costoMin, costo);
    }

    // 3. Eliminación
    if (i < s1.size()) {
        int costo = costo_del(s1[i]) + damerauLevenshtein_FuerzaBruta(s1, s2, i + 1, j);
        costoMin = min(costoMin, costo);
    }

    // 4. Transposición
    if (i < s1.size() - 1 && j < s2.size() - 1 && s1[i] == s2[j + 1] && s1[i + 1] == s2[j]) {
        int costo = costo_trans(s1[i], s1[i + 1]) + damerauLevenshtein_FuerzaBruta(s1, s2, i + 2, j + 2);
        costoMin = min(costoMin, costo);
    }

    return costoMin;
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
            int distance = damerauLevenshtein_FuerzaBruta(str1, str2, 0, 0);
            auto stop = high_resolution_clock::now();
            double tiempo = duration<double>(stop - start).count();

            Result result = calcularCosto(str1, str2, 0, 0);
            cout << "Secuencia de operaciones: " << endl;
            for (const string &op : result.operaciones) {
                cout << op << endl;
            }
            cout << "Distancia Minima de Edicion entre '" << str1 << "' y '" << str2 << "': " << distance << endl;
            cout << "Tiempo: " << tiempo << " segundos" << endl << endl;

            // Agregar el tiempo al vector de tiempos totales
            tiemposTotales.push_back(tiempo);
        }
    }

    file.close();
}

int main() {
    inicializarCostos(); // Inicializa los costos necesarios

    //vector<string> filenames = {"caso1FB.txt", "caso2FB.txt", "caso3FB.txt", "caso4FB.txt", "caso5FB.txt"};
    vector<string> filenames = {"caso5FB.txt"};
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