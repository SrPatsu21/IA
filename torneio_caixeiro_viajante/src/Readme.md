# Desafios

1. Aumentar o número de cidades

    Desafio: Testar o algoritmo com 10 ou mais cidades.
    Objetivo: Observar como o tempo de execução e a dificuldade de encontrar a melhor solução aumentam.

    R= Aumentou e a distancia percorida tambem.

2. Modificar a função de fitness

    Desafio: Criar uma função de fitness que penalize caminhos que cruzem muito ou caminhos que ultrapassem um limite de distância.

    R= fiz um sistema que se a distancia for maior que 30 o sistema vai reduzir o valor do fitness

3. Testar diferentes estratégias de seleção

    Exemplos:
        Roleta (proporcional à fitness)
        Seleção por ranking

    Objetivo: Comparar o desempenho e explicar como a seleção influencia a evolução da população.

    R= O torneio e o ranking tem um desempenho bem parecido tendo como moda 176 e nao se distanciando muito. A roleta teve um desempenho abaixo sendo a melhor distancia 179  as vezes resultando em 190 até.
    tournamentSelection = 176 normalmente
    rouletteSelection = 179 raramente
    rankingSelection = 176 normalmente

4. Comparar métodos de crossover

    Desafio: Implementar outros tipos de crossover:
        Order Crossover (OX)
        Partially Mapped Crossover (PMX)

    Objetivo: Analisar qual crossover produz melhores resultados para rotas.
    R= O PMX aparentemente é melhor, nos meus testes ele resultou nos melhores valores e mais constantes que o Order Crossover.

5. Explorar taxa de mutação

    Desafio: Testar diferentes valores de taxa_mutacao (ex.: 0.05, 0.1, 0.2).
    Objetivo: Entender o trade-off entre exploração e estabilidade da população.
    R= As mutacoes mais altas tem um resultado menos, com uma populacao reduzida, porem pode nao alterar boas solucoes

6. Visualizar a evolução

    Desafio: Criar gráficos mostrando:
        Melhor fitness por geração
        Distância média da população por geração

    Objetivo: Ajudar a compreender como a população evolui ao longo do tempo.
