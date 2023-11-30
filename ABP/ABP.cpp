#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>

struct Palavra {
    int ocorrencias;
    std::string texto;
    Palavra* esq, * dir, * pai;
};
typedef struct Palavra Palavra;

struct Letra {
    int qtdPalavras;
    char letra;
    struct Letra* prox, * ant;
    struct Palavra* raiz;
};
typedef struct Letra Letra;

struct MaiorOcorrencia {
    Palavra* palavra;
    struct MaiorOcorrencia* prox, * ant;
};
typedef struct MaiorOcorrencia MaiorOcorrencia;


struct Header {
    struct Letra* inicio, * fim;
    struct MaiorOcorrencia* maiorOcorrencia;
    int numeroPalavras;
    int totalOcorrencias;
};
Header header;

int maior = 0;
void preOrdem(Palavra* a, bool ocorrencias, bool recalculoOcorrencias, bool checaMaiorOcorrencia) {
    if (a != nullptr) {
        if (ocorrencias) {
            if (a->ocorrencias == 1)
                std::cout << a->texto << std::endl;

            preOrdem(a->esq, true, false, false);
            preOrdem(a->dir, true, false, false);
        }
        else if (!ocorrencias && !recalculoOcorrencias && !checaMaiorOcorrencia) {
            std::cout << a->texto << std::endl;
            preOrdem(a->esq, false, false, false);
            preOrdem(a->dir, false, false, false);
        }
        else if (recalculoOcorrencias) {
            if (a->ocorrencias > maior) {
                maior = a->ocorrencias;
            }

            preOrdem(a->esq, false, true, false);
            preOrdem(a->dir, false, true, false);
        }
        else if (checaMaiorOcorrencia) {
            if (a->ocorrencias == maior) {
                MaiorOcorrencia* novoNodo = new MaiorOcorrencia;
                novoNodo->palavra = a;
                novoNodo->prox = nullptr;
                novoNodo->ant = nullptr;
                if (header.maiorOcorrencia == nullptr) {
                    header.maiorOcorrencia = novoNodo;
                }
                else {
                    MaiorOcorrencia* aux = header.maiorOcorrencia;

                    while (aux->prox != nullptr) {
                        aux = aux->prox;
                    }

                    novoNodo->ant = aux;
                    aux->prox = novoNodo;
                }
            }
        }
    }
}

void posOrdem(Palavra* a) {
    if (a != nullptr) {
        posOrdem(a->esq);
        posOrdem(a->dir);
        std::cout << a->texto << std::endl;
    }
}

Palavra* insere(Palavra* noAnterior, Palavra* no, const std::string& palavra, Letra* letra) {
    if (no == nullptr) {
        no = new Palavra;
        no->texto = palavra;
        no->esq = nullptr;
        no->dir = nullptr;
        no->pai = noAnterior;
        no->ocorrencias = 1;
        std::cout << "Palavra Inserida" << std::endl;
        header.numeroPalavras++;
        letra->qtdPalavras++;
        return no;
    }
    else {
        int res = palavra.compare(no->texto);

        if (res > 0)
            no->dir = insere(no, no->dir, palavra, letra);
        else if (res < 0)
            no->esq = insere(no, no->esq, palavra, letra);
        else if (res == 0) {
            no->ocorrencias++;
            MaiorOcorrencia* novoNodo = new MaiorOcorrencia;
            novoNodo->prox = nullptr;
            novoNodo->ant = nullptr;
            novoNodo->palavra = no;
            if (header.maiorOcorrencia != nullptr) {
                MaiorOcorrencia* aux = header.maiorOcorrencia;
                MaiorOcorrencia* aux2 = header.maiorOcorrencia;
                if ((no->ocorrencias) > ((aux->palavra)->ocorrencias)) {
                    while (aux != nullptr) {
                        aux2 = aux->prox;
                        delete aux;
                        aux = aux2;
                    }
                    header.maiorOcorrencia = novoNodo;
                }
                else if ((no->ocorrencias) == ((aux->palavra)->ocorrencias) && no != aux->palavra) {
                    while (aux->prox != nullptr) {
                        aux = aux->prox;
                    }
                    aux->prox = novoNodo;
                    novoNodo->ant = aux;
                }
            }
            else {
                header.maiorOcorrencia = novoNodo;
            }
            std::cout << "Adicionada mais uma ocorrencia na palavra" << std::endl;
        }
        return no;
    }

    header.totalOcorrencias++;
}

void InserirPalavra() {
    std::string palavra;
    std::cout << "Escreva a palavra que deseja inserir:\n";
    std::cin >> palavra;

    transform(palavra.begin(), palavra.end(), palavra.begin(), ::toupper);

    char letra = palavra[0];

    Letra* aux = header.inicio;
    int achouLetra = 0;

    while (aux != nullptr) {
        if (letra == aux->letra) {
            achouLetra = 1;
            if (aux->raiz != nullptr)
                insere(nullptr, aux->raiz, palavra, aux);
            break;
        }
        else
            aux = aux->prox;
    }

    if (achouLetra == 0) {
        Letra* novaLetra = new Letra;

        if (header.inicio != nullptr) {
            aux = header.inicio;
            while (aux != nullptr) {
                if (letra > aux->letra) {
                    if (aux->prox == nullptr) {
                        //insere no fim 
                        novaLetra->prox = nullptr;
                        novaLetra->ant = aux;
                        header.fim = novaLetra;
                        aux->prox = novaLetra;
                        break;
                    }
                    else
                        aux = aux->prox;
                }
                else {
                    //insere no meio
                    novaLetra->prox = aux;
                    novaLetra->ant = aux->ant;
                    if (aux->ant != nullptr)
                        (aux->ant)->prox = novaLetra;
                    else
                        header.inicio = novaLetra;
                    aux->ant = novaLetra;
                    break;
                }
            }
        }
        else {
            novaLetra->ant = nullptr;
            novaLetra->prox = nullptr;
            header.inicio = novaLetra;
            header.fim = novaLetra;
        }

        novaLetra->qtdPalavras = 0;
        novaLetra->letra = letra;
        novaLetra->raiz = insere(nullptr, nullptr, palavra, novaLetra);
    }
    header.totalOcorrencias++;
}


Palavra* busca(Palavra* no, const std::string& palavraDigitada) {
    Palavra* aux = no;
    while (aux != nullptr) {
        int res = palavraDigitada.compare(aux->texto);
        if (res == 0)
            return aux;
        else if (res > 0)
            aux = aux->dir;
        else
            aux = aux->esq;
    }
    return nullptr;
}

void ConsultarPalavra() {
    std::string palavra;
    std::cout << "Escreva a palavra que deseja consultar:\n";
    std::cin >> palavra;

    transform(palavra.begin(), palavra.end(), palavra.begin(), ::toupper);

    char letra = toupper(palavra[0]);

    if (header.inicio != nullptr) {
        Letra* procuraLetra = header.inicio;

        while (procuraLetra != nullptr) {
            if (toupper(procuraLetra->letra) == letra) {
                break;
            }
            else
                procuraLetra = procuraLetra->prox;
        }

        if (procuraLetra == nullptr)
            std::cout << "Nenhuma palavra inserida com a letra " << letra << std::endl;
        else {
            if (procuraLetra->raiz != nullptr) {
                Palavra* procuraPalavra = procuraLetra->raiz;
                procuraPalavra = busca(procuraPalavra, palavra);

                if (procuraPalavra != nullptr) {
                    std::cout << "Palavra encontrada" << std::endl;
                    std::cout << "Numero de ocorrencias: " << procuraPalavra->ocorrencias << std::endl;
                }
                else
                    std::cout << "Palavra nao encontrada!" << std::endl;
            }
        }
    }
    else
        std::cout << "Não existe nenhuma palavra inserida ainda" << std::endl;
}
void RemovePalavra() {
    if (header.inicio == nullptr)
        std::cout << "Nao tem nada para remover" << std::endl;
    else {
        std::string palavra;
        std::cout << "Digite a palavra que deseja remover: ";
        std::cin >> palavra;

        transform(palavra.begin(), palavra.end(), palavra.begin(), ::toupper);

        char letra = toupper(palavra[0]);


        Letra* procuraLetra = header.inicio;

        while (procuraLetra != nullptr) {
            if (procuraLetra->letra == letra)
                break;
            procuraLetra = procuraLetra->prox;
        }

        if (procuraLetra == nullptr)
            std::cout << "Nenhuma palavra inserida com a letra " << letra << std::endl;
        else {
            Palavra* palavraBuscada = busca(procuraLetra->raiz, palavra);
            if (palavraBuscada == nullptr)
                std::cout << "Palavra nao encontrada para remover" << std::endl;
            else {
                header.totalOcorrencias -= palavraBuscada->ocorrencias;
                if (header.maiorOcorrencia != nullptr) {
                    if ((header.maiorOcorrencia)->palavra == palavraBuscada) {
                        if (header.maiorOcorrencia->prox == nullptr) {
                            delete header.maiorOcorrencia;
                            header.maiorOcorrencia = nullptr;
                            maior = 0;
                        }
                        else {
                            MaiorOcorrencia* aux = header.maiorOcorrencia;
                            (aux->prox)->ant = nullptr;
                            header.maiorOcorrencia = aux->prox;
                            delete aux;
                        }
                    }
                    else {
                        MaiorOcorrencia* aux = header.maiorOcorrencia;
                        while (aux != nullptr) {
                            if (aux->palavra == palavraBuscada) {
                                if (aux->prox == nullptr) {
                                    (aux->ant)->prox = nullptr;
                                    delete aux;
                                }
                                else {
                                    (aux->ant)->prox = aux->prox;
                                    (aux->prox)->ant = aux->ant;
                                    aux->prox = nullptr;
                                    aux->ant = nullptr;
                                    delete aux;
                                }
                                break;
                            }
                            aux = aux->prox;
                        }
                    }
                }
                if (palavraBuscada->esq == nullptr && palavraBuscada->dir == nullptr) {
                    if (palavraBuscada->pai != nullptr) {
                        if ((palavraBuscada->pai)->esq == palavraBuscada)
                            (palavraBuscada->pai)->esq = nullptr;
                        else
                            (palavraBuscada->pai)->dir = nullptr;
                    }
                    else {
                        //caso seja a unica palavra da letra
                        if (procuraLetra->prox == nullptr && procuraLetra->ant == nullptr) {
                            header.inicio = nullptr;
                            header.fim = nullptr;
                            procuraLetra->raiz = nullptr;
                        }
                        else if (procuraLetra->prox != nullptr && procuraLetra->ant == nullptr) {
                            (procuraLetra->prox)->ant = nullptr;
                            header.inicio = procuraLetra->prox;
                        }
                        else if (procuraLetra->prox == nullptr && procuraLetra->ant != nullptr) {
                            (procuraLetra->ant)->prox = nullptr;
                            header.fim = procuraLetra->ant;
                        }
                        else if (procuraLetra->prox != nullptr && procuraLetra->ant != nullptr) {
                            (procuraLetra->ant)->prox = procuraLetra->prox;
                            (procuraLetra->prox)->ant = procuraLetra->ant;
                        }
                        delete procuraLetra;
                        procuraLetra = nullptr;
                    }
                    // Se não há pai, significa que é o nó raiz

                    delete palavraBuscada; // mesma coisa que o free aparentemente

                    std::cout << "Palavra removida no caso de 1 nó folha" << std::endl;
                }
                else if ((palavraBuscada->esq == nullptr && palavraBuscada->dir != nullptr) || (palavraBuscada->esq != nullptr && palavraBuscada->dir == nullptr)) {
                    if (palavraBuscada->esq != nullptr) {
                        if (palavraBuscada->pai != nullptr) {
                            if ((palavraBuscada->pai)->esq == palavraBuscada)
                                (palavraBuscada->pai)->esq = palavraBuscada->esq;
                            else
                                (palavraBuscada->pai)->dir = palavraBuscada->esq;
                        }
                        else
                            //se cair aqui é pq é raiz 
                            procuraLetra->raiz = palavraBuscada->esq;

                        (palavraBuscada->esq)->pai = palavraBuscada->pai;
                    }
                    else {
                        if (palavraBuscada->pai != nullptr) {
                            if ((palavraBuscada->pai)->esq == palavraBuscada)
                                (palavraBuscada->pai)->esq = palavraBuscada->dir;
                            else
                                (palavraBuscada->pai)->dir = palavraBuscada->dir;
                        }
                        else
                            //se cair aqui é pq é raiz 
                            procuraLetra->raiz = palavraBuscada->dir;


                        (palavraBuscada->dir)->pai = palavraBuscada->pai;

                    }

                    delete palavraBuscada;

                    std::cout << "Palavra removida no caso de 1 subarvore" << std::endl;
                }
                else if (palavraBuscada->esq != nullptr && palavraBuscada->dir != nullptr) {

                    Palavra* maiorEsquerda = palavraBuscada->esq;

                    while (maiorEsquerda->dir != nullptr) {
                        maiorEsquerda = maiorEsquerda->dir;
                    }
                    (maiorEsquerda->pai)->esq = maiorEsquerda->esq;
                    maiorEsquerda->pai = palavraBuscada->pai;
                    maiorEsquerda->dir = palavraBuscada->dir;
                    maiorEsquerda->esq = palavraBuscada->esq;

                    if (palavraBuscada->pai != nullptr) {
                        if ((palavraBuscada->pai)->esq == maiorEsquerda)
                            (palavraBuscada->pai)->esq = maiorEsquerda;
                        else
                            (palavraBuscada->pai)->dir = maiorEsquerda;
                    }
                    else
                        // Se não há pai, significa que é o nó raiz
                        procuraLetra->raiz = maiorEsquerda;

                    delete palavraBuscada;

                    std::cout << "Palavra removida no caso de 2 subarvores" << std::endl;
                }
                header.numeroPalavras--;
                if (procuraLetra != nullptr)
                    procuraLetra->qtdPalavras--;
            }
        }
    }
}

void ConsultaNumeroPalavras() {
    std::cout << "Foram inseridas no dicionario: " << header.numeroPalavras << std::endl;
}

void ConsultaOcorrenciaTotal() {
    std::cout << "O dicionario tem um total de " << header.totalOcorrencias << " ocorrencias totais de palavras" << std::endl;
}


void centralEsquerda(Palavra* a) {
    if (a != nullptr) {
        centralEsquerda(a->esq);
        std::cout << a->texto << std::endl;
        centralEsquerda(a->dir);
    }
}

void centralDireita(Palavra* a) {
    if (a != nullptr) {
        centralDireita(a->dir);
        std::cout << a->texto << std::endl;
        centralDireita(a->esq);
    }
}

void ExibirListaPalavras() {
    int opcao = 0;

    if (header.inicio != nullptr) {
        while (opcao != 1 && opcao != 2) {
            std::cout << "Deseja exibir em que ordem?\n";
            std::cout << "1) A-Z\n";
            std::cout << "2) Z-A\n";

            std::cin >> opcao;
        }

        if (opcao == 1) {
            Letra* procuraLetra = header.inicio;
            while (procuraLetra != nullptr) {
                Palavra* procuraPalavra = procuraLetra->raiz;
                centralEsquerda(procuraPalavra);
                procuraLetra = procuraLetra->prox;
            }
        }
        else if (opcao == 2) {
            Letra* procuraLetra = header.fim;
            while (procuraLetra != nullptr) {
                Palavra* procuraPalavra = procuraLetra->raiz;
                centralDireita(procuraPalavra);
                procuraLetra = procuraLetra->ant;
            }
        }
    }
    else {
        std::cout << "Nenhuma palavra inserida" << std::endl;
    }
}

void ExibirListaPalavrasPorLetra() {
    char letra;
    std::cout << "Informe a letra que deseja exibir:\n";
    std::cin >> letra;

    letra = toupper(letra);

    if (header.inicio != nullptr) {
        Letra* procuraLetra = header.inicio;

        while (procuraLetra != nullptr) {
            if (procuraLetra->letra == letra)
                break;
            else
                procuraLetra = procuraLetra->prox;
        }

        if (procuraLetra != nullptr) {
            int opcao = 0;
            while (opcao != 1 && opcao != 2) {
                std::cout << "Deseja exibir em que ordem?\n";
                std::cout << "1) A-Z\n";
                std::cout << "2) Z-A\n";

                std::cin >> opcao;
            }

            if (opcao == 1)
                centralEsquerda(procuraLetra->raiz);
            else if (opcao == 2)
                centralDireita(procuraLetra->raiz);
        }
        else {
            std::cout << "Nenhuma palavra inserida com esta letra ainda" << std::endl;
        }
    }
    else {
        std::cout << "Nenhuma palavra inserida" << std::endl;
    }
}
void ConsultarPalavraComMaiorNumeroDeOcorrencia() {
    if (header.inicio != nullptr) {
        if (header.maiorOcorrencia != nullptr) {
            MaiorOcorrencia* aux = header.maiorOcorrencia;

            std::cout << "A palavra ou palavras com mais ocorrencias, sao:\n";

            while (aux != nullptr) {
                std::cout << "Palavra: '" << (aux->palavra)->texto << "', Ocorrencias: " << (aux->palavra)->ocorrencias << std::endl;
                aux = aux->prox;
            }
        }
        else {
            maior = 0;
            //recalcular a ocorrencia
            if (header.inicio != nullptr) {
                Letra* procuraLetra = header.inicio;
                while (procuraLetra != nullptr) {
                    preOrdem(procuraLetra->raiz, false, true, false);
                    procuraLetra = procuraLetra->prox;
                }
                procuraLetra = header.inicio;

                while (procuraLetra != nullptr) {
                    preOrdem(procuraLetra->raiz, false, false, true);
                    procuraLetra = procuraLetra->prox;
                }
                if (header.maiorOcorrencia != nullptr)
                    ConsultarPalavraComMaiorNumeroDeOcorrencia();
                else
                    std::cout << "Nenhuma palavra com mais de uma ocorrencia inserida" << std::endl;
            }
        }
    }
    else {
        std::cout << "Nenhuma palavra inserida" << std::endl;
    }
}

void ConsultarPalavraComUmaOcorrencia() {
    if (header.inicio == nullptr)
        std::cout << "Nenhuma palavra inserida" << std::endl;
    else {
        Letra* letra = header.inicio;
        while (letra != nullptr) {
            preOrdem(letra->raiz, true, false, false);
            letra = letra->prox;
        }
    }
}

void ArvorePreFixada() {
    if (header.inicio != nullptr) {
        Letra* procuraLetra = header.inicio;
        while (procuraLetra != nullptr) {
            std::cout << "LETRA " << procuraLetra->letra << std::endl;
            preOrdem(procuraLetra->raiz, false, false, false);
            procuraLetra = procuraLetra->prox;
        }
    }
    else
        std::cout << "Nenhuma palavra inserida" << std::endl;
}

void ArvorePosFixada() {
    if (header.inicio != nullptr) {
        Letra* procuraLetra = header.inicio;
        while (procuraLetra != nullptr) {
            std::cout << "LETRA " << procuraLetra->letra << std::endl;
            posOrdem(procuraLetra->raiz);
            procuraLetra = procuraLetra->prox;
        }
    }
    else {
        std::cout << "Nenhuma palavra inserida" << std::endl;
    }
}

void ArvoreCentralEsquerda() {
    if (header.inicio != nullptr) {
        Letra* procuraLetra = header.inicio;
        while (procuraLetra != nullptr) {
            std::cout << "LETRA " << procuraLetra->letra << std::endl;
            centralEsquerda(procuraLetra->raiz);
            procuraLetra = procuraLetra->prox;
        }
    }
    else {
        std::cout << "Nenhuma palavra inserida" << std::endl;
    }
}

void ArvoreCentralDireita() {
    if (header.inicio != nullptr) {
        Letra* procuraLetra = header.inicio;
        while (procuraLetra != nullptr) {
            std::cout << "LETRA " << procuraLetra->letra << std::endl;
            centralDireita(procuraLetra->raiz);
            procuraLetra = procuraLetra->prox;
        }
    }
    else {
        std::cout << "Nenhuma palavra inserida" << std::endl;
    }
}
void TotalPalavrasPorLetra() {
    if (header.inicio != nullptr) {
        Letra* procuraLetra = header.inicio;
        while (procuraLetra != nullptr) {
            std::cout << "LETRA " << procuraLetra->letra << " TOTAL DE PALAVRAS -> " << procuraLetra->qtdPalavras << std::endl;
            procuraLetra = procuraLetra->prox;
        }
    }
    else {
        std::cout << "Nenhuma Palavra inserida" << std::endl;
    }
}


void menu() {
    int opcao;
    std::cout << "Escolha o que deseja fazer:\n\n";
    std::cout << "1) Inserir palavra\n";
    std::cout << "2) Consultar palavra\n";
    std::cout << "3) Remover uma palavra\n";
    std::cout << "4) Consultar numero de palavras\n";
    std::cout << "5) Consultar total de ocorrencias de palavras somadas\n";
    std::cout << "6) Exibir lista de palavras\n";
    std::cout << "7) Exibir lista de palavras por letra\n";
    std::cout << "8) Consultar palavra com maior número de ocorrencias\n";
    std::cout << "9) Consultar palavras que possuem somente uma ocorrencia\n";
    std::cout << "10) Exibir palavras da arvore em ordem pre-fixada\n";
    std::cout << "11) Exibir palavras da arvore em ordem pos-fixada\n";
    std::cout << "12) Exibir palavras da arvore em ordem central a esquerda\n";
    std::cout << "13) Exibir palavras da arvore em ordem central a direita\n";
    std::cout << "14) Exibir total de palavras por letra\n";
    std::cout << "0) Sair\n";

    std::cout << "\nOpcao: ";
    std::cin >> opcao;

    switch (opcao) {
    case 0:
        break;
    case 1:
        InserirPalavra();  // Certifique-se de que a função InserirPalavra() esteja definida
        break;
    case 2:
        ConsultarPalavra();  // Certifique-se de que a função ConsultarPalavra() esteja definida
        break;
    case 3:
        RemovePalavra();  // Certifique-se de que a função RemovePalavra() esteja definida
        break;
    case 4:
        ConsultaNumeroPalavras();  // Certifique-se de que a função ConsultaNumeroPalavras() esteja definida
        break;
    case 5:
        ConsultaOcorrenciaTotal();  // Certifique-se de que a função ConsultaOcorrenciaTotal() esteja definida
        break;
    case 6:
        ExibirListaPalavras();  // Certifique-se de que a função ExibirListaPalavras() esteja definida
        break;
    case 7:
        ExibirListaPalavrasPorLetra();  // Certifique-se de que a função ExibirListaPalavrasPorLetra() esteja definida
        break;
    case 8:
        ConsultarPalavraComMaiorNumeroDeOcorrencia();  // Certifique-se de que a função ConsultarPalavraComMaiorNumeroDeOcorrencia() esteja definida
        break;
    case 9:
        ConsultarPalavraComUmaOcorrencia();  // Certifique-se de que a função ConsultarPalavraComUmaOcorrencia() esteja definida
        break;
    case 10:
        ArvorePreFixada();  // Certifique-se de que a função ArvorePreFixada() esteja definida
        break;
    case 11:
        ArvorePosFixada();  // Certifique-se de que a função ArvorePosFixada() esteja definida
        break;
    case 12:
        ArvoreCentralEsquerda();  // Certifique-se de que a função ArvoreCentralEsquerda() esteja definida
        break;
    case 13:
        ArvoreCentralDireita();  // Certifique-se de que a função ArvoreCentralDireita() esteja definida
        break;
    case 14:
        TotalPalavrasPorLetra();  // Certifique-se de que a função ArvoreCentralDireita() esteja definida
        break;
    default:
        std::cout << "Opcao nao implementada\n";
        break;
    }

    if (opcao != 0)
        menu();
    else
        std::cout << "Saindo do sistema";
}


int main() {
    menu();
    return 0;
}