#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>  

#define MAX 100

typedef struct {
    char nome[MAX];
    char senha[MAX]; 
} Usuario;

void incluirUsuario();
void alterarUsuario();
void excluirUsuario();
void listarUsuarios();
void exibirMenu(int opcaoSelecionada);

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define WHITE "\033[37m"

int CriptografarTexto(char texto[], char chave[], char textoCriptografado[]);
int DescriptografarTexto(char texto[], char chave[], char textoDescriptografado[]);

int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    int opcaoSelecionada = 0; 
    int tecla;

    do {
        exibirMenu(opcaoSelecionada); 

        tecla = getch();  
        
        if (tecla == 224) {
            tecla = getch(); 
            if (tecla == 72) {  
                opcaoSelecionada--;
                if (opcaoSelecionada < 0) opcaoSelecionada = 4;  
            } else if (tecla == 80) {  
                opcaoSelecionada++;
                if (opcaoSelecionada > 4) opcaoSelecionada = 0;  
            }
        } else if (tecla == 13) {  
            switch(opcaoSelecionada) {
                case 0:
                    incluirUsuario();
                    break;
                case 1:
                    alterarUsuario();
                    break;
                case 2:
                    excluirUsuario();
                    break;
                case 3:
                    listarUsuarios();
                    break;
                case 4:
                    printf("Saindo...\n");
                    break;
            }
        }
    } while(tecla != 27);  

    return 0;
}

void exibirMenu(int opcaoSelecionada) {
    system("cls");  
    printf("Use as setas para navegar e Enter para selecionar uma opção:\n\n");

    printf("%sIncluir novo usuário%s\n", (opcaoSelecionada == 0) ? GREEN : WHITE, RESET);
    printf("%sAlterar usuário%s\n", (opcaoSelecionada == 1) ? GREEN : WHITE, RESET);
    printf("%sExcluir usuário%s\n", (opcaoSelecionada == 2) ? GREEN : WHITE, RESET);
    printf("%sListar usuários%s\n", (opcaoSelecionada == 3) ? GREEN : WHITE, RESET);
    printf("%sSair%s\n", (opcaoSelecionada == 4) ? GREEN : WHITE, RESET);
    printf("\nPressione 'ESC' para sair.\n");
}

void incluirUsuario() {
    FILE *arquivo = fopen("usuarios.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Usuario usuario;
    char chave[] = "AbCd";  

    
    printf("Digite o nome do usuário: ");
    fgets(usuario.nome, MAX, stdin);
    usuario.nome[strcspn(usuario.nome, "\n")] = '\0';

   
    printf("Digite a senha: ");
    fgets(usuario.senha, MAX, stdin);
    usuario.senha[strcspn(usuario.senha, "\n")] = '\0';

    
    char nomeCriptografado[MAX];
    char senhaCriptografada[MAX];
    CriptografarTexto(usuario.nome, chave, nomeCriptografado);
    CriptografarTexto(usuario.senha, chave, senhaCriptografada);

    
    fprintf(arquivo, "%s;%s\n", nomeCriptografado, senhaCriptografada);
    fclose(arquivo);

    printf("Usuário incluído com sucesso!\n");
    system("pause");
}

void alterarUsuario() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo ou nenhum usuário cadastrado.\n");
        system("pause");
        return;
    }

    char nomeBusca[MAX];
    printf("Digite o nome do usuário que deseja alterar: ");
    fgets(nomeBusca, MAX, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    Usuario usuario;
    char linha[MAX * 2];
    char chave[] = "AbCd";  
    int usuarioEncontrado = 0;
    FILE *tempFile = fopen("temp.txt", "w");

    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%[^;];%s", usuario.nome, usuario.senha);

        
        char nomeDescriptografado[MAX];
        DescriptografarTexto(usuario.nome, chave, nomeDescriptografado);

       
        if (strcmp(nomeDescriptografado, nomeBusca) == 0) {
            usuarioEncontrado = 1;
            printf("Usuário encontrado!\n");
           
            printf("Digite a nova senha: ");
            fgets(usuario.senha, MAX, stdin);
            usuario.senha[strcspn(usuario.senha, "\n")] = '\0';

            char nomeCriptografado[MAX];
            char novaSenhaCriptografada[MAX];
            CriptografarTexto(nomeDescriptografado, chave, nomeCriptografado);
            CriptografarTexto(usuario.senha, chave, novaSenhaCriptografada);

            fprintf(tempFile, "%s;%s\n", nomeCriptografado, novaSenhaCriptografada);
        } else {
            
            fprintf(tempFile, "%s;%s\n", usuario.nome, usuario.senha);
        }
    }

    fclose(arquivo);
    fclose(tempFile);

    
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");

    if (usuarioEncontrado) {
        printf("Usuário alterado com sucesso!\n");
    } else {
        printf("Usuário não encontrado!\n");
    }
    system("pause");
}

void excluirUsuario() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo ou nenhum usuário cadastrado.\n");
        system("pause");
        return;
    }

    char nomeBusca[MAX];
    printf("Digite o nome do usuário que deseja excluir: ");
    fgets(nomeBusca, MAX, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    Usuario usuario;
    char linha[MAX * 2];
    char chave[] = "AbCd";  
    int usuarioEncontrado = 0;
    FILE *tempFile = fopen("temp.txt", "w");

    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%[^;];%s", usuario.nome, usuario.senha);

        char nomeDescriptografado[MAX];
        DescriptografarTexto(usuario.nome, chave, nomeDescriptografado);

        if (strcmp(nomeDescriptografado, nomeBusca) == 0) {
            usuarioEncontrado = 1;
            continue;
        }

        fprintf(tempFile, "%s;%s\n", usuario.nome, usuario.senha);
    }

    fclose(arquivo);
    fclose(tempFile);


    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");

    if (usuarioEncontrado) {
        printf("Usuário excluído com sucesso!\n");
    } else {
        printf("Usuário não encontrado!\n");
    }
    system("pause");
}

void listarUsuarios() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo ou nenhum usuário cadastrado.\n");
        system("pause");
        return;
    }

    Usuario usuario;
    char linha[MAX * 2];
    char chave[] = "AbCd"; 

    printf("\nUsuários cadastrados:\n");
    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%[^;];%s", usuario.nome, usuario.senha);

        char nomeDescriptografado[MAX];
        char senhaDescriptografada[MAX];
        DescriptografarTexto(usuario.nome, chave, nomeDescriptografado);
        DescriptografarTexto(usuario.senha, chave, senhaDescriptografada);

        printf("Nome: %s | Senha: %s\n", nomeDescriptografado, senhaDescriptografada);
    }

    fclose(arquivo);
    system("pause");
}

int CriptografarTexto(char texto[], char chave[], char textoCriptografado[]) {
    int tamanhoTexto = strlen(texto);
    int tamanhoChave = strlen(chave);

    for (int i = 0; i < tamanhoTexto; i++) {
        textoCriptografado[i] = texto[i] ^ chave[i % tamanhoChave];
    }
    textoCriptografado[tamanhoTexto] = '\0'; 
    return 0;
}

int DescriptografarTexto(char texto[], char chave[], char textoDescriptografado[]) {
    return CriptografarTexto(texto, chave, textoDescriptografado);  
}

