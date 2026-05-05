# Computação Gráfica

Este projeto é um motor de renderização 3D simples desenvolvido em C++ utilizando OpenGL. Ele permite o carregamento de cenas a partir de arquivos YAML.

## Demonstração em Vídeo
<iframe width="560" height="315" src="https://youtu.be/ecQ0ASkDJmQ" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

[Link pro Youtube](https://youtu.be/ecQ0ASkDJmQ)

## Estrutura do Projeto

- `src/`: Código-fonte (renderer, scene, shapes, transformations).
- `data/`: Definições de cenas em arquivos **YAML**
- `3rdparty/`: Dependências externas (GLAD e stb_image).

## Funcionalidades

- **Renderização de Primitivas:** Suporte para Cubos, Cones, Cilindros, Esferas, Toros e Semiesferas, além de modelos 3D (`.obj`).
- **Sistema de Cenas:** Carregamento dinâmico de cenas via YAML (`data/scenes/main.yaml`).
- **Transformações Hierárquicas:** Suporte a Translação, Rotação, Escala e cálculo dinâmico de posições globais de cena (nós parent-child).
- **Engine de Física:** Integração com Bullet Physics para Corpos Rígidos e Veículos, com suporte a colisões.
- **Modo Wireframe:** Alternância entre renderização sólida e em linhas.

## Arquitetura do Projeto e Funcionamento Geral

Estrutura geral do projeto:

- **Engine de Física:** A engine de física (Bullet Physics) é utilizada para o cálculo de colisões e aplicação de forças. O fluxo de dados ocorre da seguinte forma: o `InputHandler` captura comandos do teclado, que são convertidos em forças aplicadas aos objetos. A física calcula a "posição alvo", que é então consumida pelo `Renderer`.
- **Transformações e Renderização:** Apesar do uso da engine de física para os cálculos de dinâmica, todas as transformações sobre os modelos e a renderização final são feitas manualmente via OpenGL. O `Renderer` apenas utiliza os dados da física como destino para as matrizes de transformação calculadas no código.
- **Estrutura de Grafo:** O projeto utiliza uma cena em grafo (`Node` e `Scene`) para gerenciar a hierarquia. O ciclo de execução é dividido em `Update` (cada nós possui N componentes que executam um bloco de código nesta etapa) e `Render` (onde as matrizes são montadas e enviadas para a GPU).

## Documentação
A documentação do projeto é interna por meio de comentários em estilo `Doxygen` nas header files.

## Pré-requisitos

Para compilar o projeto, você precisará de:

- **CMake**
- **OpenGL**
- **GLFW3**
- **Eigen3**
- **Bullet Physics**
- **yaml-cpp**
- **tinyobjloader**

As bibliotecas **GLAD** e **stb_image** já estão incluídas no diretório `3rdparty`.

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

- **W, A, S, D:** Movimenta o veículo.
- **I, J, K, L:** Rotaciona a camera.
- **Espaço:** Freia o veículo.
- **C:** Troca de camera entre primeira pessoa e orbital.
- **P:** Alterna o modo Wireframe.