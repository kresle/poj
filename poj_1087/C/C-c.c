/* Copyright Charlie Clarke (ACM ICPC ECNA 1999) */

#include <stdio.h>
#include <string.h>

#define NRE 100
#define NDE 100
#define NAD 100
#define NPL (NRE + NDE + 2*NAD)

int de[NDE];
int x[NPL][NPL];
int bg[NDE][NRE];
int cde[NDE], cre[NRE];
int nre, nde, nad, npl;
char *plugName[NPL];

int
plug(char *pn)
{
  int i;

  for (i = 0; i < npl; i++)
    if (strcmp (plugName[i], pn) == 0)
      return i;

  plugName[npl] = strdup(pn);

  return npl++;
}

setup ()
{
  int i;
  char a[80], b[80];
  scanf ("%d\n", &nre);

  for (i = 0; i < nre; i++)
    {
      scanf ("%s\n", a);
      plug (a);
    }

  scanf ("%d", &nde);
  for (i = 0; i < nde; i++)
    {
      scanf ("%s %s\n", a, b);
      de[i] = plug (b);
    }

  scanf ("%d", &nad);
  for (i = 0; i < nad; i++)
    {
      scanf ("%s %s\n", a, b);
      x[plug (a)][plug (b)] = 1;
    }

  for (i = 0; i < npl; i++)
    x[i][i] = 1;

  for (i = 0; i < nde; i++)
    cde[i] = -1;

  for (i = 0; i < nre; i++)
    cre[i] = -1;
}

tc ()
{
  int i, j, k;

  for (i = 0; i < npl; i++)
    for (j = 0; j < npl; j++)
      if (x[j][i])
	for (k = 0; k < npl; k++)
	  x[j][k] |= x[i][k];
}

bbg ()
{
  int i, j;
  
  for (i = 0; i < nde; i++)
    for (j = 0; j < nre; j++)
      bg[i][j] = x[de[i]][j];
}

int
augment ()
{
  int vde[NDE], wde[NDE], iwde = 0;
  int vre[NRE], wre[NRE], iwre = 0;
  int i, j;

  for (i = 0; i < nde; i++)
    vde[i] = -1;
  for (i = 0; i < nre; i++)
    vre[i] = -1;

  for (i = 0; i < nde; i++)
    if (cde[i] == -1)
      {
	vde[i] = -2;
	for (j = 0; j < nre; j++)
	  if (bg[i][j])
	    if (cre[j] == -1)
	      {
		cde[i] = j;
		cre[j] = i;
		return 1;
	      }
	    else if (vre[j] == -1)
	      {
		wre[iwre++] = j;
		vre[j] = i;
	      }
      }

  for (;;)
    {
      if (iwre == 0) return 0;

      for (j = 0; j < iwre; j++)
	{
	  i = cre[wre[j]];
	  if (vde[i] == -1)
	    {
	      wde[iwde++] = i;
	      vde[i] = wre[j];
	    }
	}
      iwre = 0;

      if (iwde == 0) return 0;

      for (i = 0; i < iwde; i++)
	for (j = 0; j < nre; j++)
	  if (bg[wde[i]][j] && vre[j] == -1)
	    if (cre[j] == -1)
	      {
		int ar = j, ad = wde[i];

		for (;;)
		  {
		    cre[ar] = ad;
		    cde[ad] = ar;
		    if (vde[ad] == -2) return 1;
		    ar = vde[ad];
		    ad = vre[ar];
		  }
	      }
	    else
	      {
		wre[iwre++] = j;
		vre[j] = wde[i];
	      }
      iwde = 0;
    }
}

int
powerless ()
{
  int i, n = 0;

  for (i = 0; i < nde; i++)
    if (cde[i] == -1) n++;
  return n;
}

main ()
{
  setup ();
  tc ();
  bbg ();
  while (augment ());
  printf ("%d\n", powerless ());
}
