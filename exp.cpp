#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "gerador.h"
#include "solver.h"

#define MAXINT 2147483647

char arquivo_existe(char *nome)
{
	FILE *arquivo;

	if(arquivo = fopen(nome, "r"))
	{  		
		fclose(arquivo);
		return 1;
	}
	return 0;
}

void imprimir_instancia(int n, int **M, int nc, int d)
{
	char nome[21], resposta;
	do
	{
		resposta='1';
		printf("Nome do arquivo: (maximo 20 caracteres) ");
		scanf("%s",nome);
		
		if (arquivo_existe(nome))
		{
			printf("Arquivo %s ja existe. Deseja sobrescrever? (0 para nao) ",nome);
			scanf(" %c",&resposta);
		}
		
	}while(resposta == '0');
	
	printf("Imprimir arestas entre terminais? (1 para sim) ");
	scanf(" %c",&resposta);
	
	FILE *arquivo = fopen(nome, "w");
	
	fprintf(arquivo,"%d %d\n", n, d);
	int i,j;
	for (i=0;i<nc;i++) fprintf(arquivo,"%d 1\n",i+1);
	
	for (i;i<n;i++) fprintf(arquivo,"%d 0\n",i+1);
	
	for (i=0;i<n;i++)
	{
		for (j=i+1;j<n;j++)
		{
			if (M[i][j]!=MAXINT)
			{
				if (resposta == '1')
				{
					fprintf(arquivo,"%d %d %d\n",i+1,j+1,M[i][j]);
				}
				else if (i<nc) fprintf(arquivo,"%d %d %d\n",i+1,j+1,M[i][j]);
			}
		}
	}
		
	fclose(arquivo);
}

void gera(int n, int **M, int nc, int d, float p)
{
	gera_grafo(n, M, nc, p);
	resolve(n,M,nc,d);
	char resposta;
	printf("Salvar instancia em arquivo texto? (1 para 'sim') ");
	scanf(" %c",&resposta);
	if (resposta == '1') imprimir_instancia(n,M,nc,d);
}

void troca(int tam, int **M, int i, int j)
{
	int aux, percorre;
	
	for (percorre=0;percorre<tam;percorre++)
	{
		aux = M[i][percorre];
		M[i][percorre] = M[j][percorre];
		M[j][percorre] = aux;
	}
	
	for (percorre=0;percorre<tam;percorre++)
	{
		aux = M[percorre][i];
		M[percorre][i] = M[percorre][j];
		M[percorre][j] = aux;
	}
}

int main(int argc, char *argv[])
{
	if (argc==1)
	{
		srand((unsigned)time(NULL));
		
		int n, nc, d, ncmax=n;
		float p;

		printf("Quantidade de vertices: ");
		scanf("%d",&n);
	
		int *M[n];
		
		for (d=0;d<n;d++)
		{
			M[d] = (int*)malloc(n*sizeof(int));
		}

		printf("Grau minimo dos vertices centrais: ");
		scanf("%d",&d);

		if (d>1)	ncmax = (n-2)/(d-1);

		printf("Quantidade maxima de vertices centrais tal que a instancia tenha solucao: %d.\n",ncmax);
		printf("Quantidade de vertices centrais: ");
		scanf("%d",&nc);

		printf("Probabilidade de ocorrencia de aresta entre dois vertices ]0,1]: ");
		do
		{
			scanf("%f",&p);
		}while (p==0);
	
		gera(n,M,nc,d,p);
		
		for (d=0;d<n;d++) free(M[d]);
	}
	else
	{
		FILE *instancia = fopen(argv[1],"r");
		int n, nc=0, d, i, j, c;
		
		fscanf(instancia,"%d%d",&n,&d);
		int *M[n], centrais[n];
		
		for (i=0;i<n;i++)
		{
			M[i] = (int*)malloc(n*sizeof(int));
			for (j=0;j<n;j++)
			{
				M[i][j] = MAXINT;
			}
		}
		
		for (i=0;i<n;i++)
		{
			fscanf(instancia,"%d%d",&j,&centrais[i]);
			if (centrais[i]==1) nc++;
		}
		
		while(fscanf(instancia,"%d%d%d",&i,&j,&c)!=EOF)
		{
			M[i-1][j-1] = c;
			M[j-1][i-1] = c;			
		}
		
		fclose(instancia);
	
		for (i=0;i<n-1;i++)
		{
			for (j=i+1;j<n;j++)
			{
				if (centrais[i]<centrais[j])
				{
					centrais[i] = 1 - centrais[i];
					centrais[j] = 1 - centrais[j];
					troca(n, M, i, j);
				}
			}
		}
		
		resolve(n, M, nc, d);
	}	
	return 0;
}
