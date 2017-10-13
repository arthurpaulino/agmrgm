#include "mincut.h"

static int algum_cinza(int n, char *cor)
{
	int i;
	for (i=0;i<n;i++)
	{
		if (cor[i]==1) break;
	}
	return i;
}

static char existe_caminho(int n, double **G, int entrada, int saida, int *pai)
{
	char cor[n];
	int i;
	for (i=0;i<n;i++) cor[i] = 0;

	cor[entrada]=1;
	int cinza=entrada;
	
	while (cinza < n)
	{
		for (i=0;i<n;i++)
		{
			if (G[i][cinza]>0 && cor[i]==0 && i!=cinza)
			{
				pai[i] = cinza;
				if (i==saida) return 1;
				cor[i]=1;
			}
		}
		cor[cinza] = 2;
		cinza = algum_cinza(n,cor);
	}
	return 0;
}

static void alcance(int n, double **G, int entrada, int *nS, int *S)
{
	char cor[n];
	int i;
	for (i=0;i<n;i++) cor[i] = 0;
	cor[entrada]=1;
	int cinza=entrada;

	S[0] = cinza;
	*nS = 1;
	while (cinza < n)
	{
		for (i=0;i<n;i++)
		{
			if (G[cinza][i]>0 && i!=cinza && cor[i]==0)
			{
				S[*nS] = i;
				(*nS)++;
				cor[i]=1;
			}
		}
		cor[cinza] = 2;
		cinza = algum_cinza(n,cor);
	}
}

double corte_minimo(int n, double **M, int entrada, int saida, int *nS, int *S, double **f, double **Gf)
{
	int i, j, pai[n];
	
	for (i=0;i<n;i++)
	{
		for (j=0;j<n;j++)
		{
			f[i][j] = 0;
			Gf[i][j] = M[i][j];
		}
	}
	
	double menor;
	int ultimo_vertice;
	
	while (existe_caminho(n,Gf,saida,entrada,pai))
	{
		menor = MAXINT;
		i = entrada;
		while (i != saida)
		{
			if (Gf[i][pai[i]] > 0 && menor > Gf[i][pai[i]])
			{
				ultimo_vertice = i;
				menor = Gf[i][pai[i]];
			}
			i = pai[i];
		}
		
		i = entrada;
		while (i != saida)
		{
			f[i][pai[i]] = f[i][pai[i]] + menor;
			Gf[i][pai[i]] = M[i][pai[i]] - f[i][pai[i]];
			f[pai[i]][i] = -f[i][pai[i]];
			Gf[pai[i]][i] = M[pai[i]][i] - f[pai[i]][i];
			i = pai[i];
		}
		
	}
	double total = 0;
	
	for (i=0;i<n;i++) total = i!=entrada ? total + f[entrada][i] : total;
	
	alcance(n,Gf,entrada,nS,S);
	
	return total;
}
