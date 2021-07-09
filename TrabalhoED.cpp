#include <iostream>
#include <string>
using namespace std;

//lista duplamente encadeada.
struct Presenca
{
  char p;
  Presenca *prox;
  Presenca *ant;
};

//será um vetor de 14 posições.
struct Nota
{
  string nome;
  float nota = -1; //-1 para saber quando parar de ler o vetor.
  float peso = -1;
};

//lista duplamente encadeada circular ordenada alfabeticamente.
struct Alunos
{
  int numero;
  string nome;
  Nota notas[14];
  float exame;
  Alunos *prox;
  Alunos *ant;
  Presenca *inicioPres = NULL, *fimPres = NULL;
};

//lista simplesmente encadeada ordenada
struct Aula
{
  int numeroOrdem;
  int data; //modificar isso para uma melhor leitura
  int qtdHoras = 2;
  string conteudo;
  Aula *prox;
};

//lista simplesmente encadeada
struct ProvaTrab
{
  string nome;
  char tipo; //P T E
  int data;
  float peso; //0.2 20%;
  ProvaTrab *prox;
};

//lista simplesmente encadeada ordenada
struct Disciplina
{
  int codigo;
  string nome;
  int ano, semestre;
  int quantAlunos;
  int quantProvas;
  int provasDadas = 0;
  int quantTrabalhos;
  int trabalhosDados = 0;
  int CHPrevista = 60;
  int CHRealizada = 0;
  float notaMin = 5.0;
  int frequenciaMin = 70;
  Disciplina *prox;
  Aula *inicioAula = NULL, *fimAula = NULL;
  ProvaTrab *inicioProva = NULL, *fimProva = NULL;
  Alunos *inicioAluno = NULL, *fimAluno = NULL;
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
         << aux->CHPrevista << "\nCarga horaria cumprida: " << aux->CHRealizada << "\nNumero de alunos: " << aux->quantAlunos << "\nProvas dadas: " << aux->provasDadas << "\nTrabalhos dados: " << aux->trabalhosDados << "\n/----------------/";
    aux = aux->prox;
  }
}

//aux será a disciplina encontrada. Se não encontrar retorna false.
bool encontraDisciplina(Disciplina **inicio, Disciplina **aux)
{
  //printa as disciplinas
  mostraDisciplinas(inicio);
  int cod;
  cout << "\nDigite o codigo da Disciplina: ";
  cin >> cod;

  *aux = *inicio;
  //procurando a disciplina
  while (*aux != NULL && (*aux)->codigo != cod)
  {
    *aux = (*aux)->prox;
  }

  //chegou ao fim da lista e não encontrou, retorna false
  if (*aux == NULL)
  {
    cout << "Desculpe disciplina nao encontrada\n";
    return false;
  }
  //se encontrou retorna true
  else
    return true;
}

void marcarPresenca(Alunos **inicio)
{
  Alunos *aux;
  aux = *inicio;
  //vai passar por todos os alunos da disciplina atualizando a lista deles de presença.
  do
  {
    Presenca *novo = new Presenca();
    //verificação input
    while (true)
    {
      cout << "Presenca do aluno " << aux->nome << "(P ou F):";
      cin >> novo->p;
      if (novo->p == 'P' || novo->p == 'p' || novo->p == 'F' || novo->p == 'f')
        break;
      else
        cout << "Valor invalido.\n";
    }
    //se a lista estiver vazia
    if (aux->inicioPres == NULL && aux->fimPres == NULL)
    {
      novo->prox = NULL;
      novo->ant = NULL;
      aux->inicioPres = novo;
      aux->fimPres = novo;
    }
    //se não estiver vazia
    else
    {
      aux->fimPres->prox = novo;
      novo->ant = aux->fimPres;
      novo->prox = NULL;
      aux->fimPres = novo;
    }
    aux = aux->prox;
  } while (aux != *inicio);
}

//da aula de uma determinada disciplina, pegando todas as informações necessárias.
void darAula(Disciplina **inicio)
{
  Disciplina *aux;
  int cod;
  int op;
  //se encontrou a disciplina
  if (encontraDisciplina(inicio, &aux))
  { //se não tem alunos não tem como dar a aula.
    if (aux->quantAlunos == 0)
      cout << "Disciplina sem alunos.";
    else
    {
      //marcando presenças
      marcarPresenca(&aux->inicioAluno);
      cout << "1 para aula, 2 para prova, 3 para trabalho: ";
      cin >> op;
      if (op == 1)
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
      //se for aula de prova/trabalho
      else if (op == 2 || op == 3)
      {
        ProvaTrab *novo = new ProvaTrab();
        //checando se não ultrapassou o limite.
        if ((aux->provasDadas >= 4 && op == 2) || (aux->trabalhosDados >= 10 && op == 3))
        {
          cout << "Nao e possivel dar mais uma dessa atividade.";
          delete (novo);
        }
        else
        {
          cout << "Digite o nome da atividade: ";
          cin.ignore();
          getline(cin, novo->nome);
          //atribuindo tipo e incrementando numero de provas/trabalhos dados.
          if (op == 2)
          {
            novo->tipo = 'P';
            aux->provasDadas++;
          }
          else if (op == 3)
          {
            novo->tipo = 'T';
            aux->trabalhosDados++;
          }
          cout << "Data da Atividade: ";
          cin >> novo->data;
          cout << "Peso da Atividade(0.2 seria 20%): ";
          cin >> novo->peso;
          //se inicio for null, a lista está vazia.
          if (aux->inicioProva == NULL)
          {
            aux->inicioProva = novo;
            aux->fimProva = novo;
            novo->prox = NULL;
          }
          //se não, coloca no final.
          else
          {
            aux->fimProva->prox = novo;
            novo->prox = NULL;
            aux->fimProva = novo;
          }
          //aplicando notas para cada aluno da disciplina.
          Alunos *auxAlunos = aux->inicioAluno;
          do
          {
            for (int i = 0; i < 14; i++)
            {
              if (auxAlunos->notas[i].nota == -1)
              {
                cout << "Digite a nota do aluno " << auxAlunos->nome << ": ";
                cin >> auxAlunos->notas[i].nota;
                auxAlunos->notas[i].peso = novo->peso;
                auxAlunos->notas[i].nome = novo->nome;
                break;
              }
            }
            auxAlunos = auxAlunos->prox;
          } while (auxAlunos != aux->inicioAluno);
        }
      }
    }
  }
}

//printa as aulas de uma determinada disciplina.
void mostraAulas(Disciplina **inicio)
{
  Disciplina *aux;

  int cod;
  //se encontrou a disciplina
  if (encontraDisciplina(inicio, &aux))
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
    delete (*novo);
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
  Disciplina *aux;
  //se encontrou a disciplina
  if (encontraDisciplina(inicio, &aux))
  {
    dadosDisciplina(inicio, &aux);
  }
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

void cadastraAluno(Disciplina **inicio)
{
  int cod;
  int numAlunos;
  Disciplina *aux;
  Alunos *auxAlunos;

  if (encontraDisciplina(inicio, &aux))
  {
    //maximo de 50
    if (aux->quantAlunos == 50)
    {
      cout << "Desculpe, disciplina lotada.";
      return;
    }
    else
    {
      //maximo de 50
      while (true)
      {
        cout << "\nDigite o numero de alunos que deseja cadastrar: ";
        cin >> numAlunos;
        if (numAlunos + aux->quantAlunos <= 50)
          break;
        else
          cout << "\nEsta quantidade ira sobrecarregar a disciplina.";
      }
      for (int i = 0; i < numAlunos; i++)
      {
        aux->quantAlunos++;
        Alunos *novo = new Alunos();
        Alunos *auxAlunos;
        //pega o nome primeiro para ordenar a lista
        cout << "\nDigite o nome do aluno: ";
        cin >> novo->nome;
        //se vazia
        if (aux->inicioAluno == NULL)
        {
          novo->prox = novo;
          novo->ant = novo;
          aux->inicioAluno = novo;
          aux->fimAluno = novo;
        }
        else
        {
          //se for menor que inicioAluno
          if (novo->nome.compare(aux->inicioAluno->nome) < 0)
          {
            aux->fimAluno->prox = novo;
            novo->ant = aux->fimAluno;
            novo->prox = aux->inicioAluno;
            aux->inicioAluno->ant = novo;
            aux->inicioAluno = novo;
          }
          //se for maior ou igual a fimAluno
          else if (novo->nome.compare(aux->fimAluno->nome) >= 0)
          {
            aux->fimAluno->prox = novo;
            novo->prox = aux->inicioAluno;
            aux->inicioAluno->ant = novo;
            novo->ant = aux->fimAluno;
            aux->fimAluno = novo;
          }
          //se não for nenhum daqueles
          else
          {
            auxAlunos = aux->inicioAluno;
            //novo tem q ficar antes de aux
            while (auxAlunos != aux->fimAluno)
            {
              if (novo->nome.compare(auxAlunos->nome) < 0)
                break;
              auxAlunos = auxAlunos->prox;
            }
            novo->prox = auxAlunos;
            novo->ant = auxAlunos->ant;
            auxAlunos->ant->prox = novo;
            auxAlunos->ant = novo;
          }
        }
        int achou;
        //após colocar na devida posição, pede o numero e faz verificações.
        do
        {
          auxAlunos = aux->inicioAluno;
          achou = 0;
          cout << "\nDigite o numero do aluno: ";
          cin >> novo->numero;
          do
          {
            if (novo->numero == auxAlunos->numero && auxAlunos != novo)
            {
              cout << "\nNumero ja existente.";
              achou = 1;
              break;
            }
            auxAlunos = auxAlunos->prox;
          } while (auxAlunos != aux->inicioAluno);
        } while (achou);
      }
    }
  }
}
//printa as informações dos alunos de uma disciplina.
void printaAluno(Disciplina **inicio)
{
  Disciplina *aux;
  Alunos *auxAlunos;
  Presenca *auxPresenca;
  //se encontrar a disciplina
  if (encontraDisciplina(inicio, &aux))
  {
    auxAlunos = aux->inicioAluno;
    //se ela tiver alunos
    if (auxAlunos != NULL)
    {
      do
      {
        //printa numero e nome
        cout << "Numero: " << auxAlunos->numero << "\n"
             << "Nome: " << auxAlunos->nome << "\n";
        auxPresenca = auxAlunos->inicioPres;
        //se ja tiver tido pelomenos uma aula, printa as presenças.
        if (auxPresenca != NULL)
        {
          cout << "Presencas: ";
          do
          {
            cout << auxPresenca->p << " ";
            auxPresenca = auxPresenca->prox;
          } while (auxPresenca != NULL);
        }
        //se ja tiver tido pelomenos uma nota atribuida
        if (auxAlunos->notas[0].nota != -1)
        {
          cout << "\nNotas: \n";
          //enquanto não passar de 14 e não encontrar uma posição vazia(indicada por -1)
          for (int i = 0; auxAlunos->notas[i].nota != -1 && i < 14; i++)
          {
            cout << "Nome: " << auxAlunos->notas[i].nome << "\n"
                 << "Peso: " << auxAlunos->notas[i].peso << "\n"
                 << "Nota: " << auxAlunos->notas[i].nota << "\n";
            cout << "/----/\n";
          }
        }
        auxAlunos = auxAlunos->prox;
        cout << "\n/-----------------------/\n";
      } while (auxAlunos != aux->inicioAluno);
    }
    else
      cout << "Nao existem alunos nessa disciplina.\n";
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
    cout << "\n4- Mostrar alunos de determinada disciplina";
    cout << "\n5- Ministrar aula de uma disciplina";
    cout << "\n6- Mostrar aulas de uma Disciplina"; //ajeitar data
    cout << "\n7- Cadastrar alunos em uma Disciplina";
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
    case 4:
      printaAluno(&inicio);
      break;
    case 5:
      darAula(&inicio);
      break;
    case 6:
      mostraAulas(&inicio);
      break;
    case 7:
      cadastraAluno(&inicio);
      break;
    default:
      break;
    }

  } while (op != 0);
}