#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Cada item da tabela guarda o bloco e quantas vezes apareceu
struct Item {
    string bloco;
    int frequencia;
    bool ocupado;

    Item() {
        bloco = "";
        frequencia = 0;
        ocupado = false;
    }
};

// Tabela com double hashing
class HashTable {
private:
    Item* tabela;
    int tamanho;

    // Primeira função de hash (base 256)
    int hash1(string s) {
        unsigned h = 0;
        for (int i = 0; s[i] != '\0'; i++)
            h = (h * 256 + s[i]) % tamanho;
        return h;
    }

    // Segunda função de hash (passo de salto)
    int hash2(string s) {
        unsigned h = 0;
        for (int i = 0; s[i] != '\0'; i++)
            h = (h * 33 + s[i]) % (tamanho - 1);
        return (h == 0 ? 1 : h); // evita passo 0
    }

public:
    HashTable(int tamanho) {
        this->tamanho = tamanho;
        tabela = new Item[tamanho];
    }

    ~HashTable() {
        delete[] tabela;
    }

    // Insere ou incrementa a frequência de um bloco
    void inserir(string bloco) {
        int h1_val = hash1(bloco);
        int h2_val = hash2(bloco);

        int i = 0, pos;
        do {
            pos = (h1_val + i * h2_val) % tamanho;
            if (!tabela[pos].ocupado) {
                tabela[pos].bloco = bloco;
                tabela[pos].frequencia = 1;
                tabela[pos].ocupado = true;
                return;
            } else if (tabela[pos].bloco == bloco) {
                tabela[pos].frequencia++;
                return;
            }
            i++;
        } while (i < tamanho); //  pra não travar
    }

    // Salva o resultado no arquivo final
    void salvarEmArquivo(const string& nomeArquivo) {
        ofstream out(nomeArquivo);
        for (int i = 0; i < tamanho; i++) {
            if (tabela[i].ocupado) {
                out << tabela[i].bloco << " " << tabela[i].frequencia << endl;
            }
        }
        out.close();
    }
};

// Ignora linhas muito curtas
bool linhaValida(const string& linha) {
    return linha.size() >= 60;
}

int main() {
    ifstream in("sequences.fasta.txt"); //mudar para teste.txt caso queira conferir o outro arquivo
    if (!in.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return 1;
    }

    HashTable tabela(20011); // tamanho primo p/ espalhar melhor

    string linha;
    while (getline(in, linha)) {
        if (!linhaValida(linha)) continue;

        for (int i = 0; i < 60; i += 6) {
            string bloco = linha.substr(i, 6);
            tabela.inserir(bloco);
        }
    }

    in.close();
    tabela.salvarEmArquivo("saida.txt");

    cout << "Pronto! Resultado salvo em 'saida.txt'" << endl;
    return 0;
}
