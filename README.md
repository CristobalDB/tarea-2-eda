# Para poder correr el codigo hayq ue estar en la riaz del proyecto y correr los siguientes comando en la terminal
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

cmake --build build -j

./build/test

