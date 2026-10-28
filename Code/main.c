#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NG 14
#define C 17
#define SF 16


typedef struct nd{
    char *word;
    struct nd *next;
}pezzo;


typedef pezzo *lista;


typedef struct node{
    char *word;
    struct node *right;
    struct node *left;
    struct node *parent;
    int c;
} node;


typedef struct red_black_tree{
    node *root;
    node *NIL;
}red_black_tree;


//Function declarations
red_black_tree* new_rb_tree();          //funzioni degli alberi
node* new_tree_node(char*);
void insert(red_black_tree*, node*);
void insertion_fixup(red_black_tree*, node*);
void left_rotate(red_black_tree*, node*);
void right_rotate(red_black_tree*, node*);
int search(red_black_tree* ,node*, char*);
void dealloca_albero(red_black_tree*, node*);
void in_order(red_black_tree*, node*);
int contanodi(lista);                           //funzioni liste
lista inizializza_lista(red_black_tree*, node*, lista, int, char*, char*, int*, int*);
lista inserimento_in_testa(lista, char*);
lista inserimento_in_mezzo(lista, char*);
lista reverse_list(lista);
lista filtra(lista, char*, char*, int, int*, int*);
void print(lista);
void dealloca_lista(lista);
void newgame(red_black_tree*, int, char*, char*, char*, char*);         //funzioni di gioco
void inserisci_inizio(red_black_tree*, int, char*);
char* wordle(char*, char*, char*, int);         //supporti
int contaoccorrenze(const char*, char);
int occorrenzecorrette(const char*, const char*, char);
int controllo_parole(char*, char*, const char*, int, const int*, const int*);
char* clear(char*);
int fast_compare( const char *, const char *, int );




int main() {
    char *ammissibile, *str_int, *inpu, *riferimento, *tentativo, *simboli;
    int flag=1;
    char new_g[NG] = {"+nuova_partita"};
    char ins_ini[C] = {"+inserisci_inizio"};
    red_black_tree *t = new_rb_tree();

    str_int = (char*)malloc(4*sizeof(char));
    if(fgets(str_int, 4, stdin)==NULL){
        return 0;
    }              //insert lenght threshold
    int k = strtol(str_int, NULL, 10);          //convert the string in an int
    free(str_int);

    riferimento = (char*)malloc(32*sizeof(char));
    tentativo = (char*)malloc(32*sizeof(char));
    simboli = (char*)malloc(32*sizeof(char));
    inpu = (char*)malloc(32*sizeof(char));
    ammissibile = (char*)malloc(32*sizeof(char));        //take a work from stdin
    while(flag!=0){
        if(fgets(ammissibile, 31, stdin)==NULL){
            return 0;               
        }

        if(fast_compare(ammissibile, new_g, 14)!=0){
            ammissibile[k] = '\0';
            node* nuovo_nodo;
            nuovo_nodo = new_tree_node(ammissibile);
            insert(t, nuovo_nodo);
        }
        else{
            flag = 0;
            newgame(t, k, riferimento, tentativo, simboli, inpu);
        }
    }
    while(fgets(ammissibile, 31, stdin)!=NULL){
        if(fast_compare(new_g, ammissibile, 14)==0){
            newgame(t, k, riferimento, tentativo, simboli, inpu);
        }
        else if(fast_compare(ins_ini, ammissibile, 17)==0){
            inserisci_inizio(t, k, inpu);
        }
    }
    free(ammissibile);
    free(tentativo);
    free(simboli);
    free(riferimento);
    free(inpu);
    dealloca_albero(t, t->root);
    free(t->NIL);
    free(t);
}



//Trees support functions
void in_order(red_black_tree*t, node*n){
    if(n != t->NIL){
        in_order(t, n->left);
        printf("%s\n", n->word);
        in_order(t, n->right);
    }
}


red_black_tree *new_rb_tree(){
    red_black_tree *t = malloc(sizeof(red_black_tree));
    node *nil_node = malloc(sizeof(node));
    nil_node->parent = NULL;
    nil_node->right = NULL;
    nil_node->left = NULL;
    nil_node->c = 0;
    nil_node->word = NULL;
    t->NIL = nil_node;
    t->root = t->NIL;

    return t;
}


node* new_tree_node(char* parola){
    node* n= malloc(sizeof(node));
    n->right = NULL;
    n->left = NULL;
    n->parent = NULL;
    n->c = 1;
    n->word = (char*)malloc(32*sizeof(char));
    strcpy(n->word, parola);
    return n;
}


void insert(red_black_tree *t, node* z){
    node* y = t->NIL;
    node* temp = t->root;

    while(temp != t->NIL){
        y = temp;

        //int cmp = strncmp(z->word, temp->word, k)<0;

        if(strcmp(z->word, temp->word)<0){
            temp = temp->left;
        }
        else{
            temp = temp->right;
        }
    }
    z->parent = y;

    if(y == t->NIL){
        t->root = z;
    }
    else if(strcmp(z->word, y->word)<0){
        y->left = z;
    }
    else{
        y->right = z;
    }
    z->right = t->NIL;
    z->left = t->NIL;
    insertion_fixup(t, z);
}


void insertion_fixup(red_black_tree *t, node *z){
    while(z->parent->c == 1){
        if(z->parent == z->parent->parent->left){
            node *y = z -> parent->parent->right;       //uncle of z
            if(y->c == 1){
                z->parent->c = 0;
                y->c = 0;
                z->parent->parent->c = 1;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->right){
                    z = z->parent;
                    left_rotate(t,z);
                }
                z->parent->c = 0;
                z->parent->parent->c = 1;
                right_rotate(t, z->parent->parent);
            }
        }

        else{
            node *y = z->parent->parent->left;
            if(y->c == 1){
                z->parent->c = 0;
                y->c = 0;
                z->parent->parent->c = 1;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->left){
                    z = z->parent;
                    right_rotate(t, z);
                }
                z->parent->c = 0;
                z->parent->parent->c = 1;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->c = 0;
}


void left_rotate(red_black_tree *t, node *x){
    node *y = x->right;
    x->right = y->left;
    if(y->left != t->NIL){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == t->NIL){
        t->root = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}


void right_rotate(red_black_tree *t, node *x){
    node *y = x->left;
    x->left = y->right;
    if(y->right != t->NIL){
        y->right->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == t->NIL){
        t->root = y;
    }
    else if(x == x->parent->right){
        x->parent->right = y;
    }
    else{
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}


int search(red_black_tree *t, node *x, char* str){
    int j=0;
    if(x != t->NIL){
        if(strcmp(x->word, str) == 0){
            j=1;
        }
        else if(strcmp(str, x->word)<0) {
            j = search(t, x->left, str);
        }
        else if(strcmp(str, x->word)>0){
            j = search(t, x->right, str);
        }
    }
    return j;
}


void dealloca_albero(red_black_tree* t, node *x){
    if(x != t->NIL){
        dealloca_albero(t, x->left);
        dealloca_albero(t, x->right);
        free(x->word);
        free(x);
    }
}



//Lists Functions
int contanodi(lista lis){
    int numeronodi=0;
    lista temp = lis;
    while (temp!=NULL) {
        numeronodi++;
        temp = temp->next;
    }
    return numeronodi;
}


void print(lista lis){
    lista temp = lis;
    while(temp != NULL){
        printf("%s\n", temp->word);
        temp = temp->next;
    }
}


lista inizializza_lista(red_black_tree* t, node* x, lista lis, int k, char *tent, char *simb, int* occ_rif, int *occ_tent){
    if(x != t->NIL){                    
        lis = inizializza_lista(t, x->right, lis, k, tent, simb, occ_rif, occ_tent);
        if(controllo_parole(x->word, tent, simb, k, occ_rif, occ_tent) == 1){
            lis = inserimento_in_testa(lis, x->word);
        }
        lis = inizializza_lista(t, x->left, lis, k, tent, simb, occ_rif, occ_tent);
    }
    return lis;
}


lista inserimento_in_testa(lista lis, char* str){
    lista punt;
    punt = (lista)malloc(sizeof(pezzo));
    punt->word = (char*)malloc(32*sizeof(char));            //should copy also the terminator but in case we always keep track
    strcpy(punt->word,str);
    punt->next = lis;
    return punt;
}


lista inserimento_in_mezzo(lista lis, char *str){
    if((!lis) || (strcmp(lis->word, str) > 0)){
        return inserimento_in_testa(lis, str);
    }
    lis->next = inserimento_in_mezzo(lis->next, str);
    return lis;
}


lista reverse_list(lista head){
    lista prev = NULL;
    lista current = head;
    lista tmp;
    while(current){
        tmp = current->next;
        current->next = prev;
        prev = current;
        current = tmp;
    }
    return prev;
}



lista filtra(lista temp, char* str1, char* str2, int len,  int* occ_rif, int *occ_tent){        //the most convenient thing is to create a new empty list and add the items at the end, then deallocate the old list
    int is_ok;                          
    lista copy = temp;
    lista new = NULL;
    char* string;
    string = (char*)malloc(32*sizeof(char));

    while(copy != NULL){
        is_ok = controllo_parole(copy->word, str1, str2, len, occ_rif, occ_tent);
        if(is_ok==0){               //don't add the node to the new list
            copy = copy->next;
        }
        else{               //the word should be saved
            strcpy(string, copy->word);
            new = inserimento_in_testa(new, string);         //invert it after
            copy = copy->next;
        }
    }
    free(string);
    dealloca_lista(copy);
    dealloca_lista(temp);
    return new;
}


void dealloca_lista(lista lis){
    lista temp;
    while(lis){
        temp = lis->next;
        free(lis->word);
        free(lis);
        lis = temp;
    }
}


//Game support functions
void newgame(red_black_tree *t, int k, char*riferimento, char*tentativo, char*simboli, char*inpu){
    char *integer;
    lista spare = NULL;
    int occ_rif[32], occ_tent[32];

    char ins_ini[C] = {"+inserisci_inizio"};            //16
    char s_fil[SF] = {"+stampa_filtrate"};              //15
    char fine[C]={"+inserisci_fine"};

    if(fgets(riferimento, 31, stdin)==NULL){
        return;
    }                          //i read the word to guess
    riferimento[k] = '\0';          //make sure about terminator

    integer = (char*)malloc(4*sizeof(char));
    if(fgets(integer, 4, stdin)==NULL){
        return;
    }                                    //i read the maximum number of attempts
    int n = strtol(integer, NULL, 10);             //n = number of attempts
    free(integer);

    while(n>0){
        if(fgets(tentativo, 31, stdin)==NULL){
            return;
        }               //i have the insertion of a guess

        if(fast_compare(s_fil, tentativo, 16)==0){
            if(spare == NULL){
                in_order(t, t->root);
            }
            else{
                print(spare);
            }

        }

        else if(fast_compare(ins_ini, tentativo, 17) == 0){
            int flag=1;
            while(flag!=0){
                if(fgets(inpu, 31, stdin)==NULL){
                    return;
                }
                if(fast_compare(fine, inpu, 15)!=0){
                    inpu[k] = '\0';
                    node* new_node;
                    new_node = new_tree_node(inpu);
                    insert(t, new_node);
                    if(spare != NULL){
                        spare = inserimento_in_mezzo(spare, inpu);
                    }
                }
                else{
                    flag = 0;
                }
            }
        }

        else{
            tentativo[k] = '\0';

            int in_tree = search(t, t->root, tentativo);
            if(in_tree == 0){                           //if the word is not in the tree
                printf("not_exists\n");
            }
            else{                                           //if the word is in the tree
                if(strcmp(tentativo, riferimento)==0){
                    printf( "ok\n");
                    n = 0;
                }
                else{
                    n--;
                    if(n==0){
                        printf("ko\n");
                    }
                    else{
                        simboli = wordle(tentativo, riferimento, simboli, k);       
                        printf("%s\n", simboli);       

                        for(int q=0; q<k; q++){
                            occ_rif[q] = contaoccorrenze(riferimento, tentativo[q]);
                            occ_tent[q] = contaoccorrenze(tentativo, tentativo[q]);
                        }

                        if(spare == NULL){              //start a filtering process
                            spare = inizializza_lista(t, t->root, spare, k, tentativo, simboli, occ_rif, occ_tent);
                        }
                        else if(spare != NULL){
                            spare = filtra(spare, tentativo, simboli, k, occ_rif, occ_tent);
                            spare = reverse_list(spare);
                        }
                        simboli = clear(simboli);
                        printf("%d\n", contanodi(spare));
                    }
                }
            }
        }
    }
    dealloca_lista(spare);
}



void inserisci_inizio(red_black_tree *t, int k, char* inpu){
    int flag=1;
    char fine[C]={"+inserisci_fine"};

    while(flag!=0){
        if(fgets(inpu, 31, stdin)==NULL){
            return;
        }
        if(fast_compare(fine, inpu, 15)!=0){
            inpu[k] = '\0';
            node* new_node;
            new_node = new_tree_node(inpu);
            insert(t, new_node);
        }
        else{
            flag = 0;
        }
    }
}




char* wordle(char *tentativo, char *riferimento, char *simboli, int k){
    int i, v, n, c, j;
    for(i=0; i<k; i++){
        if(tentativo[i]==riferimento[i]){
            simboli[i]='+';
        }
        else{
            n = contaoccorrenze(riferimento, tentativo[i]);
            if(n>0){            //altrimenti /
                c = occorrenzecorrette(riferimento, tentativo, tentativo[i]);
                v = n - c;
                for(j=0; j<i; j++){
                    if((simboli[j]=='|') && (tentativo[i] == tentativo[j])){
                        v--;
                    }
                }
                if(v>0){                //altrimenti /
                    simboli[i] = '|';
                }
                else{
                    simboli[i] = '/';
                }
            }
            else{
                simboli[i] = '/';
            }
        }
    }
    simboli[k]='\0';
    return simboli;
}


int contaoccorrenze(const char *s, char c){
    int count = 0, i;
    for(i=0; s[i]!='\0'; i++){
        if(s[i]==c){
            count++;
        }
    }
    return count;
}


int occorrenzecorrette(const char *s, const char *t, char c){
    int count=0, i=0;
    while(s[i]!='\0' && t[i]!='\0'){
        if(s[i] == t[i]  &&  s[i] == c){
            count++;
        }
        i++;
    }
    return count;
}


int controllo_parole(char *check, char *frase, const char *simboli, int l, const int* occ_rif,const int* occ_tent){      
    int i;
    for(i=0; i<l; i++){

        switch(simboli[i])
        {
            case '+':
                if(frase[i] != check[i]){            
                    return 0;
                }
                break;

            case '|':
                if(frase[i]==check[i]){         //cannot be in the same position
                    return 0;
                }               
                int h = contaoccorrenze(check, frase[i]);       //k<n 0 k>n gives info about the words
                if(h == 0){           //cannot have zero occurrences
                    return 0;
                }

                else{
                    if(occ_tent[i] > occ_rif[i]){            //so that I must have n characters in check
                        if(h != occ_rif[i]){
                            return 0;
                        }
                    }
                    else{          //so that I must have at least k characters in check
                        if(h < occ_tent[i]){
                            return 0;
                        }
                    }
                }
                break;


            case '/':
                if(contaoccorrenze(check, frase[i]) != occ_rif[i]){                   //exact indications on the number of occurrences of the character in the word
                    return 0;
                }
                break;

            default:
                break;
        }
    }
    return 1;
}



char* clear(char* str){
    int i=0;
    while(str[i]!='\0'){
        str[i] = ' ';
        i++;
    }
    return str;
}


int fast_compare( const char *ptr0, const char *ptr1, int len ){
    int fast = len/sizeof(size_t) + 1;
    int offset = (fast-1)*sizeof(size_t);
    int current_block = 0;

    if( len <= sizeof(size_t)){ fast = 0; }


    size_t *lptr0 = (size_t*)ptr0;
    size_t *lptr1 = (size_t*)ptr1;

    while( current_block < fast ){
        if( (lptr0[current_block] ^ lptr1[current_block] )){
            int pos;
            for(pos = current_block*sizeof(size_t); pos < len ; ++pos ){
                if( (ptr0[pos] ^ ptr1[pos]) || (ptr0[pos] == 0) || (ptr1[pos] == 0) ){
                    return  (int)((unsigned char)ptr0[pos] - (unsigned char)ptr1[pos]);
                }
            }
        }

        ++current_block;
    }

    while( len > offset ){
        if( (ptr0[offset] ^ ptr1[offset] )){
            return (int)((unsigned char)ptr0[offset] - (unsigned char)ptr1[offset]);
        }
        ++offset;
    }
    return 0;
}