#pragma once
#include <array>
#include <string>
#include <vector>
#include <cstddef>

// Representa un código postal con 6 posiciones: 4 dígitos y 2 letras (ej: 2553FI)
class Poscode {
public:
    Poscode() : data_{} {}
    explicit Poscode(const std::string& s) { setFromString(s); }

    // Devuelve el código completo como string
    std::string getData() const;

    // p en [1..6]; 1 es el primer dígito, 6 es la segunda letra
    // Devuelve el valor crudo almacenado (carácter)
    char getChar(int p) const;

    // Mapea p a un valor entero para CountingSort:
    //  - p=1..4 -> 0..9
    //  - p=5..6 -> 0..25  (A..Z)
    int getValue(int p) const;

    // Set desde string (espera longitud >=6; toma los primeros 6 símbolos alfanuméricos)
    void setFromString(const std::string& s);

    // Comparador lexicográfico por las 6 posiciones
    static bool lessLex(const Poscode& a, const Poscode& b);

private:
    // Guarda exactamente 6 chars (4 dígitos + 2 letras en mayúscula)
    std::array<char, 6> data_;
};

// Carga n códigos desde el archivo 'path'.
// Devuelve un puntero a un arreglo de tamaño n (Propiedad del llamador, liberar con delete[]).
Poscode* readCodes(const std::string& path, std::size_t n);
