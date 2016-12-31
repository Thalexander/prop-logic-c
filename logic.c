#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */

int Fsize=50;
int cases=10;


int i;
int j;

/*typedef struct tableau tableau;*/

struct tableau {
  char *root;
  struct  tableau *left;
  struct tableau *right;
  struct tableau *parent;
}*tab, *node, *node1, *kid, *pa;

char *formula(char *g);
char *negatedFormula(char *g);
char *atomicFormula(char *g);
char *binaryFormula(char *g);

/*put all your functions here.  You will need
1.
int parse(char *g) which returns 1 if a proposition, 2 if neg, 3 if binary, ow 0 */

int parse(char *g)
{
    int formulaType;
    switch(*g)
    {
        case '-': formulaType = 2; break;
        case '(': formulaType = 3; break;
        default:  formulaType = 0; break;
    }

    if (formulaType = 0) {
        if (*g > 96 && *g < 123) {
	    return 1;
	}
	return 0;
    }
    g = formula(g);
    if (*g == 0)
        return formulaType;
    else
        return 0;
}

/* 2.
void complete(struct tableau *t)
which expands the root of the tableau and recursively completes any child tableaus. */

void complete(struct tableau *t) {
    int formulaType = parse(t->root);
    if (formulaType = 1) {// proposition
        return;
    }
    else if (formulaType = 2) {
        completeNegation(t);
    }
    else if (formulaType = 3) {
        completeBinary(t);
    }
    
    if (t->left)
        complete(t->left);
    if (t->right)
        complete(t->right);
}

/* 3.
int closed(struct tableau *t) */



/* which checks if the whole tableau is closed.
Of course you will almost certainly need many other functions.

You may vary this program provided it reads 10 formulas in a file called "input.txt" and outputs in the way indicated below to a file called "output.txt".
*/

















int main()

{ /*input a string and check if its a propositional formula */


  char *name = malloc(Fsize);
  FILE *fp, *fpout;
 
  /* reads from input.txt, writes to output.txt*/
  if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<cases;j++)
    {
      fscanf(fp, "%s",name);/*read formula*/
      switch (parse(name))
	{case(0): fprintf(fpout, "%s is not a formula.  ", name);break;
	case(1): fprintf(fpout, "%s is a proposition.  ", name);break;
	case(2): fprintf(fpout, "%s is a negation.  ", name);break;
	case(3):fprintf(fpout, "%s is a binary.  ", name);break;
	default:fprintf(fpout, "What the f***!  ");
	}

      /*make new tableau with name at root, no children, no parent*/
  
      struct tableau t={name, NULL, NULL, NULL};

      /*expand the root, recursively complete the children*/
      if (parse(name)!=0)
	{ complete(&t);
	  if (closed(&t)) fprintf(fpout, "%s is not satisfiable.\n", name);
	  else fprintf(fpout, "%s is satisfiable.\n", name);
	}
      else fprintf(fpout, "I told you, %s is not a formula.\n", name);
    }
 
  fclose(fp);
  fclose(fpout);
  free(name);

  return(0);
}

char *formula(char *g) /* Check validity of formula */
{
    switch(*g)
    {
        case '-': g = negatedFormula(g); break;
        case '(': g = binaryFormula(g); break;
        default:  {
            if (*g > 96 && *g < 123) {
	        g++;
	        break;
	    }
	    else {
	        *g = '0';
	        break;
	    }
	}
    }
    return g;
}

char *proposition(char *g) {
    if (*g > 96 && *g < 123) {
        g++;
        return g;
    }
    *g = '0';
    return g;
}

char *negatedFormula(char *g)
{
    g++;
    return formula(g);
}

char *binaryFormula(char *g)
{
    g++;
    g = formula(g);
    if (*g != 'v' && *g != '^' && *g != '>')
    {
        *g = '0';
        return g;
    }
    g++;
    g = formula(g);
    if (*g == ')')
    {
        g++;
        return g;
    }
    else
    {
        *g = '0';
        return g;
    }
}

char *partone(char *g)
/*
Given a formula (A*B) this should return A
 */
{
    g++;
    char *left = malloc(Fsize);
    char *first = left;
    int brackets = 0;
    while (brackets != 0 || (*g != 'v' && *g != '^' && *g != '>'))
    {
        if (*g == '(')
            brackets++;
        if (*g == ')')
            brackets--;
        *left = *g;
        left++;
        g++;
    }
    *left = 0;
    return first;
}

char *parttwo(char *g)
/*
Given a formula (A*B) this should return B
 */
{
    g++;
    char *right = malloc(Fsize);
    char *first = right;
    int brackets = 0;
    while (brackets != 0 || (*g != 'v' && *g != '^' && *g != '>'))
    {
        if (*g == '(')
            brackets++;
        if (*g == ')')
            brackets--;
        g++;
    }
    g++;
    while (brackets != 0 || *g != ')')
    {
        if (*g == '(')
            brackets++;
        if (*g == ')')
            brackets--;
        *right = *g;
        right++;
        g++;
    }
    return first;
}

char bin(char *g)
/*
Given a formula (A*B) this should return the character *
 */
{
    g++;
    int brackets = 0;
    while (brackets != 0 || (*g != 'v' && *g != '^' && *g != '>'))
    {
        if (*g == '(')
            brackets++;
        if (*g == ')')
            brackets--;
        g++;
    }
    return *g;
}

void appendAll(struct tableau *root,
	           char           *fmla) {
    if (!root->left) {
        appendLeft(root, fmla);
    }
    else {
        appendAll(root->left, fmla);
    }

    if (!root->right) {
        appendRight(root, fmla);
    }
    else {
        appendAll(root->right, fmla);
    }

}

void appendLeft(struct tableau *root,
                char           *fmla) {
    if (root->left) {
        appendAll(root->left, fmla);
    }
    else {
        struct tableau t = {fmla, NULL, NULL, root};
        root->left = &t;
    }
}


void appendRight(struct tableau *root,
                 char            *fmla) {
    if (root->right) {
        appendAll(root->right, fmla);
    }
    else {
        struct tableau t = {fmla, NULL, NULL, root};
        root->left = &t;
    }
}

char* negate(char* fmla) {
    char *negate = malloc(Fsize);
    char *first = negate;
    int brackets = 0;
    while (brackets != 0 || *fmla != 0)
    {
        if (*fmla == '(')
            brackets++;
        if (*fmla == ')')
            brackets--;
        *negate = *fmla;
        neagte++;
        fmla++;
    }
    *negate = 0;
    return first;
}

void completeNegation(struct tableau *root) {
    char* fmla = root->root
    fmla++;
    int formulaType = parse(fmla)
    if (formulaType == 1) {
        return;
    }
    else if (formulaType == 2) {
        fmla++;
        appendAll(root, fmla);
        return;
    }
    else if (formulaType == 3) {
        char binType = bin(fmla);
        char* a = partOne(fmla);
        char* b = partTwo(fmla);
        switch (binType) {
            case 'v': appendAll(root, negate(a)); appendAll(root, negate(b)); free(a); free(b); break;
            case '^': appendLeft(root, negate(a)); appendRight(root, negate(b)); free(a); free(b); break;
            case '>': appendAll(root, a); appendAll(root, negate(b)); free(b); break;
        }
    }
}

void completeBinary(struct tableau *root) {
    char* fmla = root->root
    char binType = bin(fmla);
    char* a = partOne(fmla);
    char* b = partTwo(fmla);
    switch (binType) {
        case 'v': appendLeft(root, a); appendRight(root, b); break;
        case '^': appendAll(root, a); appendAll(root, b); break;
        case '>': appendLeft(root, b); appendRight(root, negate(a)); appendRight(root, negate(b)); free(a); break;
    }
}
