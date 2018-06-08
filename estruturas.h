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

typedef struct 
{
	Trabalho *t;
} Dados_compartilhados, *ptr_thread_arg;