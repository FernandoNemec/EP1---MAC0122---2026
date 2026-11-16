## Para compilar:
```text
gcc src/html_generator.c -o html_generator
```
## Para gerar usando todos os txt de input:
```text
./html_generator $(ls txt_inputs/)
```

## Para gerar escolhendo os txt de input:
```text
./html_generator txt_inputs/txt_escolhido_1.txt txt_inputs/txt_escolhido_2.txt
```

## Os arquivos html gerados ficam na pasta build, organizados nas pastas html_outputs/ .cat/ .idx/ da build com base na função de cada html

## O index.html, que é o arquivo principal, fica na build
