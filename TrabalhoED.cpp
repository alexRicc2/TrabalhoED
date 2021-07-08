#include <iostream>

using namespace std;
struct Aula{
    int numeroOrdem;
    int data; //modificar isso para uma melhor leitura
    int qtdHoras = 2;
    string conteudo;
    Aula *prox;
};

struct Disciplina {
    int codigo;
    string nome;
    int ano, semestre;
    int quantAlunos;
    int quantProvas;
    int quantTrabalhos;
    int CHPrevista = 60;
    int CHRealizada;
    float notaMin = 5.0;
    int frequenciaMin = 70;
    Disciplina *prox;
    Aula *inicioAula = NULL,*fimAula=NULL;

};

struct Alunos{
    int numero;
    string nome;
    float notas[14];
    float exame;


};

Disciplina *inicio=NULL, *fim=NULL;
Disciplina *aux;

bool semestreAceitavel(int ano, int semestre){
    aux = inicio;
    int total=0;

    while(aux != NULL){
        if(aux->ano == ano && aux->semestre == semestre)total++;
        aux = aux->prox;
    }
    if(total >= 5)return false;
    else return true;
}
bool codAceitavel(int codigo){
    aux = inicio;
    while(aux != NULL){
        if(aux->codigo == codigo){
            cout << "\nSinto muito este codigo ja existe para uma outra disciplina\n";
            return false;
        }
        aux = aux->prox;
    }
    return true;
}
void mostraDisciplinas(){

    aux = inicio;
    cout << "\nMostrando todas as Disciplinas";
    
    while(aux != NULL){
        cout << "\n" << aux->nome << " codigo: " << aux->codigo;
        aux = aux->prox;
    }
}
void darAula(){
    int cod;

    mostraDisciplinas();
    cout << "\nDigite o codigo da Disciplina da aula: ";
    cin >> cod;

    aux = inicio;
    while(aux != NULL && aux->codigo != cod){
        aux = aux->prox;
    }

    if(aux == NULL){
        cout << "Desculpe disciplina nao encontrada";
        return;
    }
    else{
        
        Aula *novo = new Aula();
        cout << "Digite o numero de Ordem da aula: ";
        cin >> novo->numeroOrdem;

        cout << "Data da Aula: ";
        cin >> novo->data;

        cout << "Conteudo da aula: ";
        cin.ignore();
        getline(cin, novo->conteudo);

        if(aux->inicioAula == NULL){
            novo->prox = NULL;
            aux->inicioAula = novo;
            aux->fimAula = novo;
        }
        else{
            Aula *auxAula = aux->inicioAula;
            Aula *anterior = NULL;
            while(auxAula != NULL && novo->numeroOrdem > auxAula->numeroOrdem){
                anterior = auxAula;
                auxAula = auxAula->prox;
            }
            if(anterior == NULL){
                novo->prox = auxAula;
                aux->inicioAula = novo;
            }
            else if(auxAula == NULL){
                novo->prox = NULL;
                aux->fimAula->prox = novo;
                aux->fimAula = novo;
            }
            else{
                anterior->prox = novo;
                novo->prox = auxAula;
            }
            
        }

    }

}
void mostraAulas(){

    int cod;

    mostraDisciplinas();
    cout << "\nDigite o codigo da Disciplina que deseja ver as aulas: ";
    cin >> cod;

    aux = inicio;
    while(aux != NULL && aux->codigo != cod){
        aux = aux->prox;
    }

    if(aux == NULL){
        cout << "Desculpe disciplina nao encontrada\n";
        return;
    }
    else{
        Aula *auxAula = aux->inicioAula;
        while(auxAula != NULL){
            cout << "\nData: " << auxAula->data;
            cout << "\nConteudo: " << auxAula->conteudo << "\n";

            auxAula = auxAula->prox;
        }
    }

    
}
void cadastraDisciplina(){
    
    Disciplina *novo = new Disciplina();

    cout << "\nNome da Disciplina: ";
    cin.ignore();
    getline(cin ,novo->nome);

    do{
        cout << "Codigo da Disciplina (somente numeros): ";
        cin >> novo->codigo;
    }while(!codAceitavel(novo->codigo));
    
    do{
        cout << "Ano: ";
        cin >> novo->ano;
    }while(novo->ano < 2013 || novo->ano > 2030);
   
    do{
        cout << "Semestre: ";
        cin >> novo->semestre;
    }while(novo->semestre != 1 && novo->semestre != 2);
    

    if(!semestreAceitavel(novo->ano, novo->semestre)){
        cout << "\nDesculpe mas um professor só pode ministrar 5 aulas por semestre";
        return;
    }
    

    
    if(inicio == NULL){
        novo->prox = NULL;
        inicio = novo;
        fim = novo;
    }
    else{
       Disciplina *anterior = NULL;
       aux = inicio;

       while(aux != NULL && novo->codigo > aux->codigo){
           anterior = aux;
           aux = aux->prox;
       }

       if(anterior == NULL){
           novo->prox = inicio;
           inicio = novo;
       }
       else if(aux == NULL){
           fim->prox = novo;
           fim = novo;
           fim->prox = NULL;
       }
       else{
           anterior->prox = novo;
           novo->prox = aux;
       }
    }
}



int main(){

    string nome;
    int qtdDisciplinas,op;

    cout << "Sistema de Notas e Frequencias";
    cout << "\nNome do professor: ";
    cin.ignore();
    getline(cin, nome);


    do{
        cout << "\nSistema de Notas e Frequencias";
        cout << "\n1- Cadastra nova Disciplina";
        cout << "\n2- Editar Disciplina (nao feito)";
        cout << "\n3- Mostrar Disciplinas";
        cout << "\n4- Remover Disciplina (nao feito)";
        cout << "\n5- Ministrar aula de uma disciplina";
        cout << "\n6- Mostrar aulas de uma Disciplina";
        cout << "\n7- Cadastrar aluno em uma Disciplina";
        cout << "\nSua escolha: ";

        cin >> op;

        switch (op)
        {
        case 1:
            cadastraDisciplina();
            qtdDisciplinas++;
            break;
        case 3:
            mostraDisciplinas();
            break;
        case 5:
            darAula();
            break;
        case 6:
            mostraAulas();
            break;
        default:
            break;
        }

    }while(op != 0);


}