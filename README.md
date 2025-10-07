# Para poder correr el codigo hay que estar en la raiz del proyecto y correr los siguientes comandos en la terminal
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/test
