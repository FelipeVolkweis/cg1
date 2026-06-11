# Computação Gráfica

Motor de renderização 3D desenvolvido em C++ e OpenGL, com cenas e materiais configuráveis por arquivos YAML.

## Demonstração em Vídeo
[Link pro Youtube](https://youtu.be/ecQ0ASkDJmQ)

## Estrutura do Projeto

- `src/`: Código-fonte (renderer, scene, shapes, transformations).
- `data/`: Definições de cenas em arquivos **YAML**
- `3rdparty/`: Dependências externas (GLAD e stb_image).

## Funcionalidades

- **Renderização de Primitivas:** Suporte para Cubos, Cones, Cilindros, Esferas, Toros e Semiesferas, além de modelos 3D (`.obj`).
- **Iluminação:** Modelo Blinn-Phong com luzes direcionais, pontuais e spots.
- **Sombras:** Cascaded Shadow Maps para luz direcional e shadow mapping para luzes pontuais e spots.
- **Materiais:** Texturas difusas e especulares, cores ambiente/difusa/especular, brilho e transparência, com sobrescrita por YAML.
- **Sistema de Cenas:** Carregamento de cenas, modelos, luzes, materiais e skybox via YAML (`data/scenes/main.yaml`).
- **Transformações Hierárquicas:** Suporte a Translação, Rotação, Escala e cálculo dinâmico de posições globais de cena (nós parent-child).
- **Engine de Física:** Integração com Bullet Physics para Corpos Rígidos e Veículos, com suporte a colisões.
- **Câmeras:** Câmeras em primeira pessoa e orbital, com suavização e alternância durante a execução.
- **Skybox:** Renderização de ambiente por cubemap.
- **Modo Wireframe:** Alternância entre renderização sólida e em linhas.

## Arquitetura do Projeto e Funcionamento Geral

Estrutura geral do projeto:

- **Engine de Física:** A engine de física (Bullet Physics) é utilizada para o cálculo de colisões e aplicação de forças. O fluxo de dados ocorre da seguinte forma: o `InputHandler` captura comandos do teclado, que são convertidos em forças aplicadas aos objetos. A física calcula a "posição alvo", que é então consumida pelo `Renderer`.
- **Transformações e Renderização:** A física calcula a dinâmica, enquanto transformações, iluminação, sombras, materiais e renderização são processados pelo pipeline OpenGL.
- **Estrutura de Grafo:** O projeto utiliza um grafo de cena (`Node` e `Scene`) com componentes para malhas, câmeras, luzes, física e controles.

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

### Via AppImage

Para gerar o AppImage (o AppImage já está pré gerado em bin/):

```bash
make appimage
```

O arquivo será criado em `bin/cg.AppImage`. Para executá-lo sem instalar dependências. Recomenda-se usar esse método para rodar sem necessidade de compilar o programa e instalar todas as dependências.

```bash
make run-appimage
```

## Controles

- **W, A, S, D:** Movimenta o veículo.
- **I, J, K, L:** Rotaciona a camera.
- **Espaço:** Freia o veículo.
- **C:** Troca de camera entre primeira pessoa e orbital.
- **P:** Alterna o modo Wireframe.
- **0–9:** Seleciona uma luz configurada na cena.
- **U:** Liga ou desliga a luz selecionada.
- **R/F, T/G, Y/H:** Aumenta ou reduz, respectivamente, as componentes ambiente, difusa e especular da luz.
