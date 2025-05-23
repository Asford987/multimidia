# Implementação de algoritmo de compressão aritmético
Para executar o algoritmo aqui desenvolvido, existem 2 formas.
## Com Cmake
```bash
cmake -B build
cd build
make
arith_codec e/d input_path.pgm output_path.pgm
```

onde o parâmetro `e` é usado para encode e o `d` para decode.

## Modo raiz
```bash
g++ main.cpp encode.cpp decode.cpp compressor.cpp -o main
./main e/d input_path.pgm output_path.pgm
```

onde o parâmetro `e` é usado para encode e o `d` para decode.

## Exemplos
Após a compilação, podemos comprimir um arquivo com o seguinte comando
```bash
./main e baboon_ascii.pgm baboon_ascii_encoded
```

Para descomprimir, podemos usar o seguinte comando:
```bash
./main d baboon_ascii_encoded baboon_ascii_rec.pgm
```