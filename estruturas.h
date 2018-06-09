typedef struct
{
	int x;
	int y;
} Ponto;

typedef struct
{
	Ponto inicial;
	Ponto final;
} Trabalho;
typedef Trabalho* trab;

typedef struct {
    void   *dados;
    void *proximo;
} No;
typedef No* no;

typedef struct ListaEncadeada {
	no       inicial;
	int      tamanho;
	int tamanhoDados; // Define o tamanho dos dados (Trabalho ou Resultado)
} ListaEncadeada;

typedef ListaEncadeada* lista;

struct dadosCompartilhados
{
	int comprimento;
	int altura;
	lista sacoDeTarefas;
};