#include <iostream>

using namespace std;
struct Aula
{
    int numeroOrdem;
    int data; //modificar isso para uma melhor leitura
    int qtdHoras = 2;
    string conteudo;
    Aula *prox;
};

struct Disciplina
{
    int codigo;
    string nome;
    int ano, semestre;
    int quantAlunos;
    int quantProvas;
    int quantTrabalhos;
    int CHPrevista = 60;
    int CHRealizada = 0;
    float notaMin = 5.0;
    int frequenciaMin = 70;
    Disciplina *prox;
    Aula *inicioAula = NULL, *fimAula = NULL;
};

struct Alunos
{
    int numero;
    string nome;
    float notas[14];
    float exame;
};


//verifica se o semestre é valido.
bool semestreAceitavel(int ano, int semestre, Disciplina **inicio)
{
    Disciplina *aux;
    aux = *inicio;
    int total = 0;

    while (aux != NULL)
    {
        if (aux->ano == ano && aux->semestre == semestre)
            total++;
        aux = aux->prox;
    }
    if (total >= 5)
        return false;
    else
        return true;
}

//verifica se o código é válido.
bool codAceitavel(int codigo, Disciplina **atual, Disciplina **inicio)
{
    Disciplina *aux;
    aux = *inicio;
    while (aux != NULL)
    { //no caso de se editar uma disciplina, é necessária a verificação adicional de aux ser diferente
        //da que está sendo editada.
        if (aux->codigo == codigo && aux != *atual)
        {
            cout << "\nSinto muito este codigo ja existe para uma outra disciplina\n";
            return false;
        }
        aux = aux->prox;
    }
    return true;
}

//printa disciplinas.
void mostraDisciplinas(Disciplina **inicio)
{
    Disciplina *aux;

    aux = *inicio;
    cout << "\nMostrando todas as Disciplinas";

    while (aux != NULL)
    {
        cout << "\n"
             << aux->nome << " codigo: " << aux->codigo << "\nCarga horaria prevista: "
             << aux->CHPrevista << "\nCarga horaria cumprida: " << aux->CHRealizada << "\n/----------------/";
        aux = aux->prox;
    }
}

//da aula de uma determinada disciplina, pegando todas as informações necessárias.
void darAula(Disciplina **inicio)
{
    Disciplina *aux;
    int cod;

    mostraDisciplinas(inicio);
    cout << "\nDigite o codigo da Disciplina da aula: ";
    cin >> cod;

    aux = *inicio;
    while (aux != NULL && aux->codigo != cod)
    {
        aux = aux->prox;
    }

    if (aux == NULL)
    {
        cout << "Desculpe disciplina nao encontrada";
        return;
    }
    else
    {
        Aula *novo = new Aula();
        //contabilizando horas
        aux->CHRealizada += novo->qtdHoras;
        cout << "Digite o numero de Ordem da aula: ";
        cin >> novo->numeroOrdem;

        cout << "Data da Aula: ";
        cin >> novo->data;

        cout << "Conteudo da aula: ";
        cin.ignore();
        getline(cin, novo->conteudo);

        if (aux->inicioAula == NULL)
        {
            novo->prox = NULL;
            aux->inicioAula = novo;
            aux->fimAula = novo;
        }
        else
        {
            Aula *auxAula = aux->inicioAula;
            Aula *anterior = NULL;
            while (auxAula != NULL && novo->numeroOrdem > auxAula->numeroOrdem)
            {
                anterior = auxAula;
                auxAula = auxAula->prox;
            }
            if (anterior == NULL)
            {
                novo->prox = auxAula;
                aux->inicioAula = novo;
            }
            else if (auxAula == NULL)
            {
                novo->prox = NULL;
                aux->fimAula->prox = novo;
                aux->fimAula = novo;
            }
            else
            {
                anterior->prox = novo;
                novo->prox = auxAula;
            }
        }
    }
}

//printa as aulas de uma determinada disciplina.
void mostraAulas(Disciplina **inicio)
{
    Disciplina *aux;

    int cod;

    mostraDisciplinas(inicio);
    cout << "\nDigite o codigo da Disciplina que deseja ver as aulas: ";
    cin >> cod;

    aux = *inicio;
    while (aux != NULL && aux->codigo != cod)
    {
        aux = aux->prox;
    }

    if (aux == NULL)
    {
        cout << "Desculpe disciplina nao encontrada\n";
        return;
    }
    else
    {
        Aula *auxAula = aux->inicioAula;
        while (auxAula != NULL)
        {
            cout << "\nData: " << auxAula->data;
            cout << "\nConteudo: " << auxAula->conteudo << "\n";

            auxAula = auxAula->prox;
        }
    }
}

//pega os dados da disciplina e faz verificações.
//retorna true ou false para verificações futuras.
//é usado para criar disciplina e editar disciplina.
bool dadosDisciplina(Disciplina **inicio, Disciplina **novo)
{
    int semestre, ano;
    do
    {
        cout << "Ano: ";
        cin >> ano;
    } while (ano < 2013 || ano > 2030);

    do
    {
        cout << "Semestre: ";
        cin >> semestre;
    } while (semestre != 1 && semestre != 2);

    if (!semestreAceitavel(ano, semestre, inicio))
    {
        cout << "\nDesculpe mas um professor só pode ministrar 5 aulas por semestre";
        delete(*novo);
        return false;
    }
    else
    {
        //é necessario atribuit a aux somente após verificação no caso de edição de uma disciplina.
        (*novo)->semestre = semestre;
        (*novo)->ano = ano;
    }

    cout << "\nNome da Disciplina: ";

    cin.ignore();
    getline(cin, (*novo)->nome);

    do
    {
        cout << "Codigo da Disciplina (somente numeros): ";
        cin >> (*novo)->codigo;
    } while (!codAceitavel((*novo)->codigo, novo, inicio)); //passando novo em si para não incluir ele na verificação no caso de edição.
    return true;
}

//edita uma disciplina ja existente.
void editaDisciplina(Disciplina **inicio)
{
    mostraDisciplinas(inicio);
    Disciplina *aux;
    aux = *inicio;
    int codigo, achou = 0;
    cout << "\nDigite o Codigo da Disciplina: ";
    cin >> codigo;
    while (aux != NULL)
    {
        if (aux->codigo == codigo)
        {
            achou = 1;
            break;
        }
        aux = aux->prox;
    }
    if (achou)
    {
        dadosDisciplina(inicio, &aux);
    }
    else
        cout << "\nDisciplina nao encontrada.";
}

//cadastra uma disciplina.
void cadastraDisciplina(Disciplina **inicio, Disciplina **fim)
{

    Disciplina *novo = new Disciplina();
    //se retornar false, então ja chegou ao maximo de disciplinas por semestre, e não vai incluir na lista.
    if (dadosDisciplina(inicio, &novo))
    {

        if (*inicio == NULL)
        {
            novo->prox = NULL;
            *inicio = novo;
            *fim = novo;
        }
        else
        {
            Disciplina *aux;
            Disciplina *anterior = NULL;
            aux = *inicio;

            while (aux != NULL && novo->codigo > aux->codigo)
            {
                anterior = aux;
                aux = aux->prox;
            }

            if (anterior == NULL)
            {
                novo->prox = *inicio;
                *inicio = novo;
            }
            else if (aux == NULL)
            {
                (*fim)->prox = novo;
                *fim = novo;
                (*fim)->prox = NULL;
            }
            else
            {
                anterior->prox = novo;
                novo->prox = aux;
            }
        }
    }
}

int main()
{
    Disciplina *inicio = NULL, *fim = NULL;

    string nome;
    int qtdDisciplinas, op;

    cout << "Sistema de Notas e Frequencias";
    cout << "\nNome do professor: ";
    cin.ignore();
    getline(cin, nome);

    do
    {
        cout << "\nSistema de Notas e Frequencias";
        cout << "\n1- Cadastra nova Disciplina";
        cout << "\n2- Editar Disciplina";
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
            cadastraDisciplina(&inicio, &fim);
            qtdDisciplinas++;
            break;
        case 2:
            editaDisciplina(&inicio);
            break;
        case 3:
            mostraDisciplinas(&inicio);
            break;
        case 5:
            darAula(&inicio);
            break;
        case 6:
            mostraAulas(&inicio);
            break;
        default:
            break;
        }

    } while (op != 0);
}