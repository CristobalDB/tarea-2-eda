#include "poscode.hpp"
#include <fstream>
#include <stdexcept>
#include <cctype>

std::string Poscode::getData() const {
    return std::string(data_.begin(), data_.end());
}

char Poscode::getChar(int p) const {
    if (p < 1 || p > 6) throw std::out_of_range("p fuera de rango [1..6]");
    return data_[p-1];
}

int Poscode::getValue(int p) const {
    char c = getChar(p);
    if (p >= 1 && p <= 4) { // dígitos
        if (!std::isdigit(static_cast<unsigned char>(c))) return 0;
        return c - '0';
    } else { // letras A..Z
        char u = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        if (u < 'A' || u > 'Z') return 0;
        return u - 'A';
    }
}

void Poscode::setFromString(const std::string& s) {
    // Toma los primeros 6 caracteres alfanuméricos encontrados
    std::array<char,6> tmp{};
    int k = 0;
    for (char ch : s) {
        if (std::isalnum(static_cast<unsigned char>(ch))) {
            tmp[k++] = ch;
            if (k == 6) break;
        }
    }
    if (k < 6) {
        throw std::runtime_error("Código inválido: se esperaban 6 símbolos alfanuméricos");
    }
    // Normaliza letras a mayúscula en posiciones 5 y 6
    for (int i = 0; i < 6; ++i) {
        if (i >= 4 && std::isalpha(static_cast<unsigned char>(tmp[i]))) {
            tmp[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(tmp[i])));
        }
    }
    data_ = tmp;
}

bool Poscode::lessLex(const Poscode& a, const Poscode& b) {
    for (int p = 1; p <= 6; ++p) {
        char ca = a.getChar(p);
        char cb = b.getChar(p);
        if (ca < cb) return true;
        if (ca > cb) return false;
    }
    return false;
}

Poscode* readCodes(const std::string& path, std::size_t n) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("No se pudo abrir el archivo: " + path);
    }
    Poscode* arr = new Poscode[n];
    std::string line;
    std::size_t i = 0;
    while (i < n && std::getline(in, line)) {
        if (line.empty()) continue;
        // Para datasets con posible columna adicional, toma la primera palabra
        std::string tok = line;
        // si hay espacios, corta
        auto pos = line.find_first_of(" \t");
        if (pos != std::string::npos) tok = line.substr(0, pos);
        arr[i++] = Poscode(tok);
    }
    if (i < n) {
        // Si el archivo tiene menos de n, redondea; en práctica, para la tarea usar n correcto
        // Aquí podríamos realloc, pero mantenemos n y llenamos con el último valor válido si existiera.
        for (; i < n; ++i) {
            arr[i] = Poscode("0000AA");
        }
    }
    return arr;
}
