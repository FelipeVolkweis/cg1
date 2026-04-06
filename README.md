# Computação Gráfica

Este projeto é um motor de renderização 3D simples desenvolvido em C++ utilizando OpenGL. Ele permite o carregamento de cenas a partir de arquivos de texto, suporta algumas primitivas geométricas e transformações espaciais básicas.

## Funcionalidades

- **Renderização de Primitivas:** Suporte para Cubos, Cones, Cilindros, Esferas, Toros e Semiesferas.
- **Sistema de Cenas:** Carregamento dinâmico de cenas (`data/scenes/main.txt`) e subcenas (`data/subscenes/*.txt`).
- **Transformações:** Suporte a Translação, Rotação e Escala.
- **Interatividade:** Controle em tempo real de objetos marcados como `translatable`, `rotatable` ou `scalable`.
- **Modo Wireframe:** Alternância entre renderização sólida e em linhas.

## Documentação
A documentação do projeto é interna por meio de comentários em estilo `Doxygen` nas header files.

## Pré-requisitos

Para compilar o projeto, você precisará de:

- **CMake**
- **OpenGL**
- **GLFW3**
- **Eigen3**

As bibliotecas **GLAD** já estão incluídas no diretório `3rdparty`.

## Como Compilar

Você pode compilar utilizando o `Makefile` fornecido ou diretamente via `cmake`.

### Via Makefile:
```bash
make
```

### Via CMake:
```bash
mkdir build
cd build
cmake ..
make
```

## Como Executar

Após a compilação, execute o binário gerado:

```bash
./build/cg
```
Ou via Makefile:
```bash
make run
```

## Controles

- **W, A, S, D:** Move objetos marcados como `translatable`.
- **Q, E:** Altera a escala de objetos marcados como `scalable`.
- **R, T:** Rotaciona no eixo Y objetos marcados como `rotatable`.
- **P:** Alterna o modo Wireframe.

## Estrutura do Projeto

- `src/`: Código-fonte (renderer, scene, shapes, transformations).
- `data/`: Definições de cenas e subcenas em arquivos `.txt`.
- `3rdparty/`: Dependências externas (GLAD).
