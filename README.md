# Airport Graph and Sparse Matrix (Grafo de Aeroportos e Matriz Esparsa)

Este projeto é uma implementação em C de um sistema de gerenciamento de malha aérea, utilizando conceitos de Teoria de Grafos e Estruturas de Dados Avançadas. O objetivo é modelar aeroportos como vértices e voos como arestas, permitindo a representação eficiente de conexões aéreas e a busca de trajetos.

## 🚀 Funcionalidades

- **Cadastro de Aeroportos**: Adição de novos aeroportos à rede (Vértices do Grafo).
- **Cadastro de Voos**: Criação de conexões entre aeroportos (Arestas do Grafo), associadas a um número de voo.
- **Remoção de Voos**: Possibilidade de remover voos específicos através do seu número de identificação.
- **Listagem de Voos**: Consulta de todos os voos que partem de um aeroporto específico.
- **Busca de Trajetos**: Implementação do algoritmo de **Busca em Profundidade (DFS - Depth-First Search)** para encontrar possíveis caminhos entre dois aeroportos.

## 🛠️ Estruturas de Dados Utilizadas

Para garantir a eficiência no armazenamento e na manipulação dos dados, o projeto utiliza:

1. **Matriz Esparsa**: 
   - Utilizada para representar a matriz de adjacência do grafo.
   - Em vez de uma matriz bidimensional convencional (que consumiria muita memória para redes com muitos aeroportos e poucos voos), a matriz esparsa armazena apenas os elementos não nulos através de listas encadeadas por linha.
2. **Vetor Dinâmico**: 
   - Utilizado para armazenar a lista de aeroportos cadastrados, permitindo que a malha cresça dinamicamente conforme novos aeroportos são adicionados.
3. **Percurso**: 
   - Estrutura auxiliar para rastrear e imprimir a sequência de aeroportos e voos durante a busca de trajetos.

## 📂 Estrutura do Projeto

- `aeroporto.h`: Definição da estrutura `Aeroporto`.
- `voo.h`: Definição da estrutura `Voo`.
- `vetor_dinamico.h`: Implementação de um vetor genérico dinâmico.
- `matriz_esparsa.h`: Implementação da matriz esparsa para representação do grafo.
- `percurso.h`: Estrutura para representação de caminhos entre aeroportos.
- `malha.h` / `malha.c`: Lógica principal de gerenciamento da malha aérea e algoritmos de busca.

## 💻 Como Compilar e Executar

Para compilar o projeto, utilize o GCC (ou outro compilador C):

```bash
gcc -o aeroporto main.c malha.c matriz_esparsa.c vetor_dinamico.c aeroporto.c voo.c percurso.c
```
*(Nota: Certifique-se de incluir todos os arquivos `.c` correspondentes aos headers no comando de compilação).*

Para executar:
```bash
./aeroporto
```

## 📝 Exemplo de Fluxo

A função `inicializar_malha_padrao` fornece um exemplo de configuração com aeroportos como BSB (Brasília), GRU (São Paulo), GIG (Rio de Janeiro), CNF (Belo Horizonte) e SSA (Salvador), permitindo testar a busca de trajetos entre eles.
