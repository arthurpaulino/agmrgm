#include <cstdlib>
#include "gerador.h"

int uniforme (int i, int j)
{
    return(i + rand () % (j-i+1));
}

static int peso(double p, int maxcusto)
{
	double chance = ((double)uniforme(0,100000))/100000.0;
	if (chance <= p) return uniforme (1,maxcusto);
	else return MAXINT;
}

static int algum_cinza(int n, char *cor)
{
	int i;
	for (i=0;i<n;i++)
	{
		if (cor[i]==1) break;
	}
	return i;
}

static char conexo(int n, int **G, char *cor)
{
	int i,j;
	for (i=0;i<n;i++) cor[i] = 0;
	
	cor[0]=1;
	int cinza=0;
	
	while (cinza < n)
	{
		for (j=0;j<n;j++)
		{
			if (G[cinza][j] != MAXINT && cor[j]==0) cor[j]=1;
		}
		cor[cinza] = 2;
		cinza = algum_cinza(n,cor);
	}
	j=0;
	for (i=0;i<n;i++)
	{
		if (cor[i]==2) j++;
	}
	if (j!=n) return 0;
	return 1;
}

static void cria_uma_aresta(int n, int **G, char *cor, int maxcusto)
{
	int i,j;
	do
	{
		i = uniforme(0,n-1);
	}while (cor[i]!=2);
	
	do
	{
		j = uniforme(0,n-1);
	}while (cor[j]!=0);
	
	G[i][j] = uniforme(0,maxcusto);
	G[j][i] = G[i][j];
}

void gera_grafo(int n, int **G, int nc, float p)
{
	int i,j,maxcusto = n*(n-1)/2;
	
	for (i=0;i<n;i++)
	{
		for (j=0;j<n;j++)
		{
			G[i][j] = MAXINT;
		}
	}
	
	for (i=0;i<n;i++)
	{
		for (j=i+1;j<n;j++)
		{
			G[i][j] = peso(p,maxcusto);
			G[j][i] = G[i][j];
		}
	}
	
	char cor[n];
	
	while (conexo(n,G,cor)==0)	cria_uma_aresta(n,G,cor,maxcusto);
	while (conexo(nc,G,cor)==0)	cria_uma_aresta(nc,G,cor,maxcusto);
}
