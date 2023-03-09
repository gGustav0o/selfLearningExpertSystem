//------------------------INCLUDE------------------------//
#include <iostream>
//-----------------------NAMESPACE-----------------------//
using namespace std;
//------------------------DEFINE------------------------//
#define MEMORY_ERROR 2
#define FILE_ERROR 3
#define path R"(C:\Users\Kurai\Desktop\0\Proga\sles\save.txt)"
#define defaultPath R"(C:\Users\Kurai\Desktop\0\Proga\sles\save1.txt)"
//------------------------STRUCT------------------------//
struct tree{
    char* line;
    tree* left;
    tree* right;
};
//-------------------ERROR--FUNCTIONS-------------------//
void memoryError(){
    cerr<<"Memory error\n";
    exit(MEMORY_ERROR);
}
void fileError(){
    cerr<<"File error\n";
    exit(FILE_ERROR);
}
//------------------------------------------------------//
char* getString(int* length) {
    *length = 0;
    int capacity = 1;
    char* s = (char*)malloc(sizeof(char));
    if (s == nullptr) {
        memoryError();
    }
    char c = (char)getchar();
    if(c == '\n') c = (char)getchar();
    while (c != '\n') {
        s[(*length)++] = c;
        if (*length >= capacity) {
            capacity *= 2;
            char* temp = (char*)realloc(s, capacity * sizeof(char));
            if (temp) {
                s = temp;
            }
            else {
                memoryError();
            }
        }
        c = (char)getchar();
    }
    s[*length] = '\0';
    return s;
}
char* getString(int* length, FILE* file) {
    *length = 0;
    int capacity = 1;
    char* s = (char*)malloc(sizeof(char));
    if (s == nullptr) {
        memoryError();
    }
    char c = (char)fgetc(file);
    if(c == '\n') c = (char)fgetc(file);
    while (c != '\n' && c != '&' && c != '\0') {
        s[(*length)++] = c;
        if (*length >= capacity) {
            capacity *= 2;
            char* temp = (char*)realloc(s, capacity * sizeof(char));
            if (temp) {
                s = temp;
            }
            else {
                memoryError();
            }
        }
        c = (char)fgetc(file);
    }
    s[*length] = '\0';
    return s;
}
void print_menu() {
    printf("\nWhat do you want to do?\n");
    printf("1. Continue\n");
    printf("2. New one\n");
    printf("3. Overwrite\n");
    printf("4. Exit\n");
    printf(">");
}
void secondMenu() {
    printf("\nYour answer?\n");
    printf("1. Yes\n");
    printf("2. No\n");
    printf("3. Exit\n");
    printf(">");
}
int getVariant(int count) {
    int variant;
    char s[100];
    s[99] = '\0';
    int flag = -1;
    flag = scanf("%s", s);
    if (flag != 1) exit(-1);
    while (sscanf(s, "%d", &variant) != 1 || variant < 1 || variant > count)
    {
        printf("Incorrect input. Try again: \n");
        flag = scanf("%s", s);
        if (flag != 1) exit(-1);
    }
    return variant;
}
void saveTree(FILE *file, tree *t){
    if (t){
        fprintf(file, "%s", t->line);
        fputc('&', file);
        if (t->left){
            fputc('+',file);
            fputc('&', file);}
        else{
            fputc('-',file);
            fputc('&', file);}
        if (t->right){
            fputc('+',file);
            fputc('&', file);}
        else{
            fputc('-',file);
            fputc('&', file);}
        saveTree(file,t->left);
        saveTree(file,t->right);
    }
}
tree *readTree(FILE *file){
    char *line, *lineLeft, *lineRight;
    tree *t = nullptr;
    int length;
    line = getString(&length, file);
    lineLeft = getString(&length, file);
    lineRight = getString(&length, file);
    if (!feof(file)){
        t = (tree *)malloc(sizeof(tree));
        t->line = line;
        t->left= nullptr;
        t->right= nullptr;
        if (lineLeft[0] != '-')
            t->left=readTree(file);
        if (lineRight[0] != '-')
            t->right=readTree(file);
    }
    return t;
}
void destroy(tree *t){
    if (t){
        destroy(t->left);
        destroy(t->right);
        free(t->line);
        free(t);
    }
}
tree* create(char* line){
    tree* root = (tree*)malloc(sizeof(tree));
    if (!root) memoryError();
    root->line = line;
    root->left = nullptr;
    root->right = nullptr;
    return root;
}
void game(tree* t){
    printf("%s", t->line);
    printf("%c", '?');
    secondMenu();
    int variant = getVariant(3);
    if(variant == 3) exit(0);
    if ((variant == 2 && !t->right->right) || (variant == 1 && !t->left->left)){
        printf("The answer is ");
        if (variant == 1) printf("%s\n", t->left->line);
        else printf("%s\n", t->right->line);
        printf("Am I right?\n");
        secondMenu();
        int variant2 = getVariant(3);
        if (variant2 == 1) printf("gg\n");
        if(variant2 == 2){
            printf("What is it then? \n");
            int length;
            char* answer = getString(&length);
            printf("How would u describe it? \n");
            char* quest = getString(&length);
            if (variant == 1) {
                char* temp = t->left->line;
                t->left->line = quest;
                tree* ans1 = create(answer);
                t->left->left = ans1;
                t->left->right = create(temp);
            } else{
                char* temp = t->right->line;
                t->right->line = quest;
                tree* ans1 = create(answer);
                t->right->left = ans1;
                t->right->right = create(temp);
            }
        }
        if(variant2 == 3) exit(0);
    } else {
        switch (variant) {
            case 1:
                game(t->left);
                break;
            case 2:
                game(t->right);
                break;
            default:
                //some error
                break;
        }
    }
}
int main() {
    FILE *save;
    if( (save = fopen(path, "r" )) == nullptr ) fileError();
    tree* root = readTree(save);
    fclose(save);
    print_menu();
    int variant = getVariant(4);
    while (variant != 4){
        if (variant == 1) game(root);
        if (variant == 2){
            destroy(root);
            if((save = fopen(defaultPath, "r" )) == nullptr ) fileError();
            root = readTree(save);
        }
        if (variant == 3) {
            if((save = fopen(path, "w" )) == nullptr ) fileError();
            saveTree(save, root);
            fclose(save);
        }
        print_menu();
        variant = getVariant(4);
    }
    return 0;
}
