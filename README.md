Para executar o algoritmo aqui desenvolvido, existem 2 formas.
## Com Cmake
```bash
cmake -B build
cd build
make e/d input_path.pgm output_path.pgm
```

onde o parâmetro `e` é usado para encode e o `d` para decode.

## Modo raiz
```bash
g++ main.cpp encode.cpp decode.cpp compressor.cpp -o main
./main e/d input_path.pgm output_path.pgm
```

onde o parâmetro `e` é usado para encode e o `d` para decode.
