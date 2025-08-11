#include <bits/stdc++.h>
using namespace std;

struct Estado {
    int b5, b3;
};

const int CAP5 = 5, CAP3 = 3;

void imprimirEstado(const Estado &e, int nivel, const string &acao) {
    for (int i = 0; i < nivel; i++) cout << "   ";
    cout << "(" << e.b5 << "," << e.b3 << ")";
    if (!acao.empty()) cout << " -- " << acao;
    if (e.b5 + e.b3 == 7) cout << "  <- Objetivo!";
    cout << "\n";
}

void gerarArvore(const Estado &e, int nivel, set<pair<int,int>> visitados) {
    visitados.insert({e.b5, e.b3});

    if (e.b5 + e.b3 == 7) {
        // Imprime o estado objetivo e NÃO gera filhos (para este ramo)
        imprimirEstado(e, nivel, "Estado final - Objetivo");
        return;
    }

    vector<pair<Estado,string>> proximos;

    proximos.push_back({{CAP5, e.b3}, "Encher balde de 5"});
    proximos.push_back({{e.b5, CAP3}, "Encher balde de 3"});
    proximos.push_back({{0, e.b3}, "Esvaziar balde de 5"});
    proximos.push_back({{e.b5, 0}, "Esvaziar balde de 3"});

    int transf = min(e.b5, CAP3 - e.b3);
    proximos.push_back({{e.b5 - transf, e.b3 + transf}, "Transferir de 5 para 3"});

    transf = min(e.b3, CAP5 - e.b5);
    proximos.push_back({{e.b5 + transf, e.b3 - transf}, "Transferir de 3 para 5"});

    for (auto &prox : proximos) {
        if (!visitados.count({prox.first.b5, prox.first.b3})) {
            imprimirEstado(prox.first, nivel + 1, prox.second);
            gerarArvore(prox.first, nivel + 1, visitados);
        }
    }
}

int main() {
    Estado inicial = {0, 0};
    imprimirEstado(inicial, 0, "Início");
    gerarArvore(inicial, 0, {});
    return 0;
}