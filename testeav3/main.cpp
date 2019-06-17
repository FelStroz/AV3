#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>
using namespace std;

/** Definição do tipo de dados 'byte'
* 'unsigned char': É o tipo que consegue gravar no intervalo que vai de 0 a 255 bytes
*/
typedef unsigned char byte;

/** Definição da árvore */
typedef struct Arv{
    int freq;
    byte b;
    struct Arv *esq;
    struct Arv *dir;
} NO;

/** Definição da fila de prioridade (implementada como lista simplesmente encadeada) */

typedef struct List
{
    Arv *t;
    struct List *prox;
} ls;

typedef struct lista
{
    List *head;
    int elem;
} lista;

/**
* A função strdup é dependente de implementação nas plataformas não POSIX (Windows, etc)
* Segue uma implementação desta função como solução para o problema.
*/
/*
char *strdup(const char *s)
{
    char *p = static_cast<char *>(malloc(strlen(s) + 1));
    if (p) strcpy(p, s);
    return p;
}
*/
/** Função que faz alocação de memória e trata os ponteiros soltos acerca de nós da lista encadeada.
* Obs: cada nó da lista encadeada é conectado a um nó 'raiz' de árvore.
* @param: um nó de uma árvore.
*/

List *CriaList(Arv *nArv)
{
    // Aloca memória
    List *novo = (List*)malloc(sizeof(List));
    if (novo == NULL) return NULL;

    // Adiciona a árvore ao nó
    novo->t = nArv;

    // Faz o campo próximo apontar para NULL
    novo->prox = NULL;

    return novo;
}

/** Função que faz alocação de memória e trata os ponteiros soltos acerca de nós da árvore
* @param: o byte a ser gravado no nó, a frequencia do byte, ponteiros para os nós filhos
*/

Arv *novoNodeArvore(byte c, int frequencia, Arv *esquerda, Arv *direita)
{
// Aloca memória
Arv *novo =(Arv*)malloc(sizeof(Arv));

if ( novo == NULL ) return NULL;

// Atribui na árvore os valores passados como parâmetro
novo->b = c;
novo->freq = frequencia;
novo->esq = esquerda;
novo->dir = direita;

return novo;
}

/** Função que um novo nó na lista encadeada que representa a fila de prioridade.
* @param: um nó previamente criado, a lista que receberá o nó
*/

void insereLista(List *n, lista *l)
{
    // Se a lista passada como parâmetro não tem um nó no início (vazia), insira o nó no início
    if (!l->head)
    {
        l->head = n;
    }

        // Se o campo 'frequência' do 'nó' parâmetro for menor que o campo 'frequência' do primeiro item (head)
        // da lista, incluir o novo nó como head, e colocar o head antigo como next desse novo

    else if (n->t->freq < l->head->t->freq)
    {
        n->prox = l->head;
        l->head = n;
    }
    else
    {
        // nó auxiliar que inicia apontando para o segundo nó da lista (head->prox)
        List *aux = l->head->prox;
        // nó auxiliar que inicia apontando para o primeiro nó da lista
        List *aux2 = l->head;

        // Laço que percorre a lista e insere o nó na posição certa de acordo com sua frequência.
        //
        // Se sabe que aux começa apontando para o segundo item da lista e aux2 apontando para o primeiro.
        // Sendo assim, os ponteiros seguirão mudando de posição enquanto aux não for o fim da lista,
        // e enquanto a frequência do nó apontado por aux for menor ou igual a frequência do 'nó' parâmetro.

        while (aux && aux->t->freq <= n->t->freq)
        {
            aux2 = aux;
            aux = aux2->prox;
        }

        // Se insere o nó na posição certa.
        aux2->prox = n;
        n->prox = aux;
    }

    // Incrementa quantidade de elem
    l->elem++;
}

/** Função que 'solta' o nó apontado por 'head' da lista (o de menor frequência)
* (faz backup do nó e o desconecta da lista)
* @param: uma lista encadeada.
*/

Arv *popMinLista(lista *l)
{

    // Ponteiro auxilar que aponta para o primeiro nó da lista
    List *aux = l->head;

    // Ponteiro auxiliar que aponta para a árvore contida em aux (árvore do primeiro nó da lista)
    Arv *aux2 = aux->t;

    // Aponta o 'head' da lista para o segundo elemento dela
    l->head = aux->prox;

    // Libera o ponteiro aux
    free(aux);
    aux = NULL;

    // Decrementa a quantidade de elem
    l->elem--;

    return aux2;
}

/** Função que conta a frequência de ocorrências dos bytes de um dado arquivo
* @param: um arquivo, uma lista de bytes
*/

void getByteFrequency(FILE *entrada, unsigned int *listaBytes)
{

    byte c;

    /***
    *
    * fread( array/bloco de memoria , tamanho de cada elemento, quantos elementos, arquivo de entrada )
    * fread retorna a quantidade de blocos lidos com sucesso
    *
    * Faz a leitura de 1 bloco de tamanho 1 byte a partir do arquivo 'entrada'
    * e salva no espaco de memoria de 'c'.
    *
    * Converte esse byte num valor decimal, acessa o bucket correspondente e incrementa o valor (frequência).
    *
    ***/

    while (fread(&c, 1, 1, entrada) >= 1)
    {
        listaBytes[(byte)c]++;
    }
    rewind(entrada); // "rebobina o arquivo"

}

#include <stdbool.h>
#include <iostream>
#include <sstream>

//  Obtem o código começando no nó t, utilizando o byte salvo em 'b', preenchendo 'buffer', desde o bucket 'tamanho'

/**
/ Função recursiva que percorre uma árvore de huffman e para ao encontrar o byte procurado (c)
/ @param: nó para iniciar a busca, byte a ser buscado, buffer para salvar os nós percorridos, posição para escrever
**/

bool pegaCodigo(Arv *n, byte c, char *buffer, int tamanho)
{

// Caso base da recursão:
// Se o nó for folha e o seu valor for o buscado, colocar o caractere terminal no buffer e encerrar

if (!(n->esq || n->dir) && n->b == c)
{
buffer[tamanho] = '\0';
return true;
}
else
{
bool encontrado = false;

// Se existir um nó à esq
if (n->esq)
{
// Adicione '0' no bucket do buffer correspondente ao 'tamanho' nodeAtual
buffer[tamanho] = '0';

// fazer recursão no nó esquerdo
encontrado = pegaCodigo(n->esq, c, buffer, tamanho + 1);
}

if (!encontrado && n->dir)
{
buffer[tamanho] = '1';
encontrado = pegaCodigo(n->dir, c, buffer, tamanho + 1);
}
if (!encontrado)
{
buffer[tamanho] = '\0';
}
return encontrado;
}

}

// Algoritmo para construir a árvore de huffman, inspirado no seguinte pseudocódigo:
// http://www.cs.gettysburg.edu/~ilinkin/courses/Spring-2014/cs216/assignments/a8.html
//
// procedure BUILD-TREE(frequencies):
//     pq ← make empty priority queue
//     for each (symbol, key) in frequencies:
//         h ← make a leaf node for the (symbol, key) pair
//         INSERT(pq, h)
//
//     t ← size[pq]
//     for i = 1 to t-1:
//         h1 ← POP-MIN(pq)
//         h2 ← POP-MIN(pq)
//         h3 ← MAKE-NODE(h1, h2)
//         INSERT(pq, h3)
//
//     return POP-MIN(pq)

/** Função que constrói a árvore de huffman
* @param: a fila de prioridade.
*/

Arv *BuildHuffmanTree(unsigned *listaBytes)
{
    // Lista com head apontando pra NULL e com campo 'elem' valendo 0;
    lista l = {NULL, 0};

    // Popula usando a array 'listaBytes' (que contém as frequências) uma lista encadeada de nós.
    // Cada nó contém uma árvore.
    for (int i = 0; i < 256; i++)
    {
        if (listaBytes[i]) // Se existe ocorrência do byte
        {
            // Insere na lista 'l' um nó referente ao byte i e sua respectiva frequência (guardada em listaBytes[i]).
            // Faz os nós esquerdo e direito das árvores apontarem para NULL;
            insereLista(CriaList(novoNodeArvore(i, listaBytes[i], NULL, NULL)), &l);
        }
    }

    while (l.elem > 1) // Enquanto o número de elem da lista for maior que 1
    {
        // Nó esquerdo chama a função popMinLista() que vai na lista e pega a árvore fixada no primeiro nó
        // (que é a que contém a menor frequência)
        Arv *nodeEsquerdo = popMinLista(&l);

        // Pega o outro nó que tem menor frequência
        Arv *nodeDireito = popMinLista(&l);

        // Preenche com '#' o campo de caractere do nó da árvore.
        // Preenche o campo 'frequência' com a soma das frequências de 'nodeEsquerdo' e 'nodeDireito'.
        // Aponta o nó esquerdo para nodeEsquerdo e o nó direito para nodeDireito
        Arv *soma = novoNodeArvore(
                '#',
                nodeEsquerdo->freq + nodeDireito->freq, nodeEsquerdo, nodeDireito
        );

        // Reinsere o nó 'soma' na lista l
        insereLista(CriaList(soma), &l);
    }

    return popMinLista(&l);
}

/** Função que libera memória da árvore de huffman
* @param: nó de uma (sub)árvore.
*/

void soltaoHuf(Arv *n)
{
    // Caso base da recursão, enquanto o nó não for NULL
    if (!n) return;
    else
    {
        Arv *esquerda = n->esq;
        Arv *direita = n->dir;
        free(n);
        soltaoHuf(esquerda);
        soltaoHuf(direita);
    }
}

/** Função que faz bitmasking no byte lido e retorna um valor booleano confirmando sua existência
* Ideia do bitmasking surgiu da leitura de http://ellard.org/dan/www/CS50-95/s10.html
* @param: arquivo para ler o byte, posição que se deseja mascarar o byte, byte a ser feita a checagem
*/

int geraBit(FILE *entrada, int posicao, byte *aux )
{
    // É hora de ler um bit?
    (posicao % 8 == 0) ? fread(aux, 1, 1, entrada) : NULL == NULL ;

    // Exclamação dupla converte para '1' (inteiro) se não for 0. Caso contrário, deixa como está (0)
    // Joga '1' na casa binária 'posicao' e vê se "bate" com o byte salvo em *aux do momento
    // Isso é usado para percorrer a árvore (esq e dir)
    return !!((*aux) & (1 << (posicao % 8)));
}

/** Função para notificar ausência do arquivo. Encerra o programa em seguida.
*/
void erroArquivo()
{
    printf("Arquivo nao encontrado\n");
    exit(0);
}

/** Função que comprime um arquivo utilizando a compressão de huffman
* @param: arquivo a comprimir, arquivo resultado da compressão
*/

void CompressFile(const char *arquivoEntrada, const char *arquivoSaida)
{

    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    unsigned listaBytes[256] = {0};

    // Abre arquivo do parâmetro arquivoEntrada no modo leitura de binário
    FILE *entrada = fopen(arquivoEntrada, "rb");
    (!entrada) ? printf("Arquivo nao encontrado\n") : NULL == NULL ;

    // Abre arquivo do parâmetro arquivoSaida no modo escrita de binário
    FILE *saida = fopen(arquivoSaida, "wb");
    (!saida) ? printf("Arquivo nao encontrado\n") : NULL == NULL ;

    getByteFrequency(entrada, listaBytes);

    // Populando a árvore com a lista de frequência de bytes
    Arv *raiz = BuildHuffmanTree(listaBytes);

    // Grava a lista de frequência nos primeiros 256 bytes do arquivo
    fwrite(listaBytes, 256, sizeof(listaBytes[0]), saida);

    // Move o ponteiro do stream 'saida' para a posição posterior ao tamanho de um unsigned int
    // É aqui que posteriormente será salvo o valor da variável 'tamanho'
    fseek(saida, sizeof(unsigned int), SEEK_CUR);

    byte c;
    unsigned tamanho = 0;
    byte aux = 0;

    /***
    * fread( array/bloco de memoria , tamanho de cada elemento, quantos elementos, arquivo de entrada )
    * fread retorna a quantidade de bytes lidos com sucesso
    *
    * Faz a leitura de 1 bloco de tamanho 1 byte a partir do arquivo 'entrada'
    * e salva no espaco de memoria de 'c'.
    ***/

    while (fread(&c, 1, 1, entrada) >= 1)
    {

        // Cria um buffer vazio
        char buffer[1024] = {0};

        // Busca o código começando no nó 'raiz', utilizando o byte salvo em 'b', preenchendo 'buffer', desde o bucket deste último
        pegaCodigo(raiz, c, buffer, 0);

        // Laço que percorre o buffer
        for (char *i = buffer; *i; i++)
        {
            // Se o caractere na posição nodeAtual for '1'
            if (*i == '1')
            {
                // 2 elevado ao resto da divisão de 'tamanho' por 8
                // que é o mesmo que jogar um '1' na posição denotada por 'tamanho % 8'
                //aux = aux + pow(2, tamanho % 8);
                aux = aux | (1 << (tamanho % 8));
            }

            tamanho++;

            // Já formou um byte, é hora de escrevê-lo no arquivo
            if (tamanho % 8 == 0)
            {
                fwrite(&aux, 1, 1, saida);
                // Zera a variável auxiliar
                aux = 0;
            }
        }
    }

    // Escreve no arquivo o que sobrou
    fwrite(&aux, 1, 1, saida);

    // Move o ponteiro do stream para 256 vezes o tamanho de um unsigned int, a partir do início dele (SEEK_SET)
    fseek(saida, 256 * sizeof(unsigned int), SEEK_SET);

    // Salva o valor da variável 'tamanho' no arquivo saida
    fwrite(&tamanho, 1, sizeof(unsigned), saida);

    final = clock();
    //tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC;

    // Calcula tamanho dos arquivos
    fseek(entrada, 0L, SEEK_END);
    double tamanhoEntrada = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    double tamanhoSaida = ftell(saida);

    soltaoHuf(raiz);

    printf("Arquivo de entrada: %s (%g bytes)\nArquivo de saida: %s (%g bytes)\nTempo gasto: %gs\n", arquivoEntrada, tamanhoEntrada / 1000, arquivoSaida, tamanhoSaida / 1000, tempoGasto);
    printf("Taxa de compressao: %d%%\n", (int)((100 * tamanhoSaida) / tamanhoEntrada));

    fclose(entrada);
    fclose(saida);

}

/** Função que descomprime um arquivo utilizando a compressão de huffman
* @param: arquivo a descomprimir, arquivo resultado da descompressão
*/

void DecompressFile(const char *arquivoEntrada, const char *arquivoSaida)
{

    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    unsigned listaBytes[256] = {0};

    // Abre arquivo do parâmetro arquivoEntrada no modo leitura de binário
    FILE *entrada = fopen(arquivoEntrada, "rb");
    (!entrada) ? printf("Arquivo nao encontrado\n") : NULL == NULL ;

    // Abre arquivo do parâmetro arquivoSaida no modo escrita de binário
    FILE *saida = fopen(arquivoSaida, "wb");
    (!saida) ? printf("Arquivo nao encontrado\n") : NULL == NULL ;

    // Lê a lista de frequência que encontra-se nos primeiros 256 bytes do arquivo
    fread(listaBytes, 256, sizeof(listaBytes[0]), entrada);

    // Constrói árvore
    Arv *raiz = BuildHuffmanTree(listaBytes);

    // Lê o valor dessa posição do stream para dentro da variável tamanho
    unsigned tamanho;
    fread(&tamanho, 1, sizeof(tamanho), entrada);

    unsigned posicao = 0;
    byte aux = 0;

    // Enquanto a posicao for menor que tamanho
    while (posicao < tamanho)
    {
        // Salvando o nodeArvore que encontramos
        Arv *nodeAtual = raiz;

        // Enquanto nodeAtual não for folha
        while ( nodeAtual->esq || nodeAtual->dir )
        {
            nodeAtual = geraBit(entrada, posicao++, &aux) ? nodeAtual->dir : nodeAtual->esq;
        }

        fwrite(&(nodeAtual->b), 1, 1, saida);
    }

    soltaoHuf(raiz);

    //final = clock();
   // tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC;

    fseek(entrada, 0L, SEEK_END);
    double tamanhoEntrada = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    double tamanhoSaida = ftell(saida);

    //printf("Arquivo de entrada: %s (%g bytes)\nArquivo de saida: %s (%g bytes)\nTempo gasto: %gs\t", arquivoEntrada, tamanhoEntrada / 1000, arquivoSaida, tamanhoSaida / 1000, tempoGasto);
    printf("Taxa de descompressao: %d%%\n", (int)((100 * tamanhoSaida) / tamanhoEntrada));

    fclose(saida);
    fclose(entrada);
}

void menu(){
    cout << "  |-------------------------------------------|" << endl;
    cout << "  |            Digite a sua opcao:            |" << endl;
    cout << "  |-------------------------------------------|" << endl;
    cout << "  | 1.  Comprimir arquivo                     |" << endl;
    cout << "  | 2.  Descomprimir arquivo                  |" << endl;
    cout << "  | 3.  Sair                                  |" << endl;
    cout << "  |-------------------------------------------|" << endl;
}

int main()
{


    cout << "Digite o nome do arquivo comprimido..."<< endl;
    //cout << pap;
    char pap[100];
    char *vaa = pap;
    ostringstream papa;
    cin >> pap;
    papa<<"/home/bluelab-29/Área de Trabalho/BlueLab/"<<vaa<<".phpzin";
    menu();

    int op;
    cin>>op;


    while(op>=1 && op<=2) {
        if(op == 1){

            CompressFile("/home/bluelab-29/Área de Trabalho/BlueLab/aula 4.2 - exercicios(1).pdf",papa.str().c_str());
           // system("pause");
        }
        if(op == 2){
            DecompressFile(papa.str().c_str(), "/home/bluelab-29/Área de Trabalho/BlueLab/arqBosta.pdf");
            //system("pause");
        }
        menu();
        cin >> op;
    }
    cout << "Saindo...";

    return 0;
}