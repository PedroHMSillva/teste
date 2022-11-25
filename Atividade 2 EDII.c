#include <stdio.h>
#include <stdlib.h>

typedef enum {FALSE = 0, TRUE = 1} boolean;
   5 typedef enum {FOLHA, NO} tipoNo;
   6 typedef enum {INVALIDO, PILHA_VAZIA, OPERADOR, OPERANDO, FIM} tipoResultado;
   7
   8 struct elemento;
   9
  10 struct noArvore
  11 {
  12     tipoNo tipo;
  13     union
  14     {
  15         int valor;  // Folha
  16         struct noBinario
  17         {
  18             struct noArvore *esquerdo;
  19             char operador;
  20             struct noArvore *direito;
  21         } info;
  22     };
  23 };
  24
  25 struct elemento
  26 {
  27     struct noArvore *no;
  28     struct elemento *prox;
  29 };
  30
  31 tipoResultado obterSimbolo(struct noArvore **ppRaiz)
  32 {
  33     char c;
  34     int valor = 0;
  35     // remover brancos
  36     do
  37     {
  38         c = getc(stdin);
  39     }
  40     while (c == ' ' || c == '\t' || c == '\n');
  41     // obter simbolo
  42     if (c >= '0' && c <= '9')
  43     {
  44         do
  45         {
  46             valor = valor * 10 + (int)c - (int)'0';
  47             c = getc(stdin);
  48         } while (c >= '0' && c <= '9');
  49         ungetc(c, stdin);
  50         *ppRaiz = (struct noArvore *)malloc(sizeof(struct noArvore));
  51         (*ppRaiz)->tipo = FOLHA;
  52         (*ppRaiz)->valor = valor;
  53         return OPERANDO;
  54     }
  55     else
  56     {
  57         if (c == '+' || c == '-' || c == '*' || c == '/')
  58         {
  59             *ppRaiz = (struct noArvore *)malloc(sizeof(struct noArvore));
  60             (*ppRaiz)->tipo = NO;
  61             (*ppRaiz)->info.esquerdo = NULL;
  62             (*ppRaiz)->info.operador = c;
  63             (*ppRaiz)->info.direito = NULL;
  64             return OPERADOR;
  65         }
  66         else
  67         {
  68             if (c == '#') return FIM;
  69             else return INVALIDO;
  70         }
  71     }
  72 }
  73
  74 boolean vazia(struct elemento *pPilha)
  75 {
  76     return pPilha == NULL;
  77 }
  78
  79 void empilhar(struct elemento **ppPilha, struct noArvore *pRaiz)
  80 {
  81     struct elemento *p;
  82     p = (struct elemento *)malloc(sizeof(struct elemento));
  83     p->no = pRaiz;
  84     p->prox = *ppPilha;
  85     *ppPilha = p;
  86 }
  87
  88 boolean desempilhar(struct elemento **ppPilha, struct noArvore **ppRaiz)
  89 {
  90     struct elemento *p;
  91     if (vazia(*ppPilha))
  92     {
  93         *ppRaiz = NULL;
  94         return FALSE;
  95     }
  96     else
  97     {
  98         p = *ppPilha;
  99         *ppRaiz = (*ppPilha)->no;
 100         *ppPilha = (*ppPilha)->prox;
 101         free(p);
 102         return TRUE;
 103     }
 104 }
 105
 106 void liberarArvore(struct noArvore *pRaiz)
 107 {
 108     if (pRaiz != NULL)
 109     {
 110         if (pRaiz->tipo == NO)
 111         {
 112             liberarArvore(pRaiz->info.esquerdo);
 113             liberarArvore(pRaiz->info.direito);
 114         }
 115         free(pRaiz);
 116     }
 117 }
 118
 119 void liberarPilha(struct elemento **ppPilha)
 120 {
 121     struct noArvore *pRaiz;
 122     struct elemento *p;
 123     while (*ppPilha != NULL)
 124     {
 125         pRaiz = (*ppPilha)->no;
 126         p = *ppPilha;
 127         *ppPilha = (*ppPilha)->prox;
 128         free(p);
 129         liberarArvore(pRaiz);
 130     }
 131 }
 132
 133 tipoResultado processarSufixo(struct elemento **ppPilha)
 134 {
 135     struct noArvore *pRaiz = NULL;
 136     tipoResultado tipo = obterSimbolo(&pRaiz);
 137     while (tipo != INVALIDO && tipo != FIM && tipo != PILHA_VAZIA)
 138     {
 139         switch (pRaiz->tipo)
 140         {
 141         case NO:
 142             if (desempilhar(ppPilha, &(pRaiz->info.direito)))
 143                 if (desempilhar(ppPilha, &(pRaiz->info.esquerdo)))
 144                     empilhar(ppPilha, pRaiz);
 145                 else tipo = PILHA_VAZIA;
 146             else tipo = PILHA_VAZIA;
 147             break;
 148         case FOLHA:
 149             empilhar(ppPilha, pRaiz);
 150             break;
 151         }
 152         if (tipo == PILHA_VAZIA) liberarArvore(pRaiz);
 153         else tipo = obterSimbolo(&pRaiz);
 154     }
 155     return tipo;
 156 }
 157
 158 void apresentarInfixo(struct noArvore *pRaiz)
 159 {
 160     if (pRaiz != NULL)
 161     {
 162         if (pRaiz->tipo == FOLHA) printf("%d", pRaiz->valor);
 163         else
 164         {
 165             printf("(");
 166             apresentarInfixo(pRaiz->info.esquerdo);
 167             printf(" %c ", pRaiz->info.operador);
 168             apresentarInfixo(pRaiz->info.direito);
 169             printf(")");
 170         }
 171     }
 172 }
 173
 174 int main(void)
 175 {
 176     tipoResultado tipo;
 177     struct elemento *pilha = NULL;
 178     if ((tipo = processarSufixo(&pilha)) == FIM)
 179         if (pilha == NULL) printf("Expressao vazia\n");
 180         else
 181             if(pilha->prox == NULL)
 182             {
 183                 apresentarInfixo(pilha->no);
 184                 putchar('\n');
 185             }
 186             else printf("Expressao incompleta\n");
 187     else
 188         if (tipo == PILHA_VAZIA) printf("Expressao incompleta\n");
 189         else printf("Simbolo invalido\n");
 190     liberarPilha(&pilha);
 191     return 0;
 192 }
}
