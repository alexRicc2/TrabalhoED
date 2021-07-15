 //Diego Lopes da Silva
 //Alex Ricardo Rodrigues Sant'Anna
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//lista duplamente encadeada.
struct Presenca
{
  char p;
  int aulaCarga; //esse atributo foi adicionado para evitar uma busca de dois whiles aninhados para achar a carga
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
  bool exame = false;
  bool aprovado = false;
  float media;
  float notaExame;
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
  int provasDadas = 0;
  int trabalhosDados = 0;
  int CHPrevista = 60;
  int CHRealizada = 0;
  float notaMin = 5.0;
  int frequenciaMin = 70;
  bool semestreFechado = false;
  Disciplina *prox;
  Aula *inicioAula = NULL, *fimAula = NULL;
  ProvaTrab *inicioProva = NULL, *fimProva = NULL;
  Alunos *inicioAluno = NULL, *fimAluno = NULL;
};

//verifica se o semestre é valido (maximo de 5 por semestre).
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
  if (codigo < 0)
  {
    cout << "Codigo invalido.\n";
    return false;
  }
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
//se op for 1(ou true), ira printar em arquivo.
void mostraDisciplinas(Disciplina **inicio, bool op = 0)
{
  ofstream myfile;
  Disciplina *aux;
  int quantProvasCumpridas;
  float maiorNota, menorNota, TotalNota;

  aux = *inicio;
  if (op != 1)
    cout << "\n---Mostrando todas as Disciplinas---\n";

  while (aux != NULL)
  {
    if (op == 1)
    {
      myfile.open(aux->nome + ".txt");
      cout << "\nArquivo " << aux->nome << ".txt criado.\n";
    }
    int qtdAvaliacoes = aux->provasDadas + aux->trabalhosDados;
    int faltas = 0, presente = 0;

    if (op == 1)
      myfile << "nome: " << aux->nome << "\ncodigo: " << aux->codigo << "\nCarga horaria prevista: "
             << aux->CHPrevista << "\nCarga horaria cumprida: " << aux->CHRealizada << "\nNumero de alunos: " << aux->quantAlunos << "\nProvas dadas: " << aux->provasDadas << "\nTrabalhos dados: " << aux->trabalhosDados;
    else
      cout << "\nnome: " << aux->nome << "\ncodigo: " << aux->codigo << "\nCarga horaria prevista: "
           << aux->CHPrevista << "\nCarga horaria cumprida: " << aux->CHRealizada << "\nNumero de alunos: " << aux->quantAlunos << "\nProvas dadas: " << aux->provasDadas << "\nTrabalhos dados: " << aux->trabalhosDados;

    //se a disciplina possuir alunos
    if (aux->inicioAluno != NULL)
    {
      Alunos *auxAluno = aux->inicioAluno;

      maiorNota = 0, menorNota = 10, TotalNota = 0, quantProvasCumpridas = 0;

      do
      { //percorre todos alunos

        //se possuir avaliacoes
        if (qtdAvaliacoes > 0)
        {
          //calcula a maior,menor e media da nota
          for (int i = 0; i < 14 && auxAluno->notas[i].nota != -1; i++)
          {

            if (auxAluno->notas[i].nota > maiorNota)
              maiorNota = auxAluno->notas[i].nota;
            if (auxAluno->notas[i].nota < menorNota)
              menorNota = auxAluno->notas[i].nota;
            TotalNota += auxAluno->notas[i].nota;
            quantProvasCumpridas++;
          }
        }
        //se possuir aulas
        if (aux->inicioAula != NULL)
        {
          Presenca *auxPres;
          auxPres = auxAluno->inicioPres;

          //contabiliza as presencas de um aluno
          while (auxPres != NULL)
          {
            if (auxPres->p == 'f' || auxPres->p == 'F')
              faltas++;
            else
              presente++;

            auxPres = auxPres->prox;
          }
        }

        auxAluno = auxAluno->prox;
      } while (auxAluno != aux->inicioAluno);

      //printando informacoes de notas caso foram dadas
      if (qtdAvaliacoes > 0)
      {
        if (op == 1)
        {
          myfile << "\nMaior nota tirada em uma avaliacao: " << maiorNota;
          myfile << "\nMenor nota tirada em uma avaliacao: " << menorNota;
          myfile << "\nMedia de notas nas avaliacoes: " << TotalNota / quantProvasCumpridas;
        }
        else
        {
          cout << "\nMaior nota tirada em uma avaliacao: " << maiorNota;
          cout << "\nMenor nota tirada em uma avaliacao: " << menorNota;
          cout << "\nMedia de notas nas avaliacoes: " << TotalNota / quantProvasCumpridas;
        }
      }
      if (aux->inicioAula != NULL)
      {
        float percentual = faltas / float((faltas + presente)) * 100;

        if (op == 1)
        {
          myfile << "\nQuantidade faltas da turma: " << faltas;
          myfile << "\nPercentual de faltas da turma: " << percentual << "%";
        }
        else
        {
          cout << "\nQuantidade faltas da turma: " << faltas;
          cout << "\nPercentual de faltas da turma: " << percentual << "%";
        }
      }
    }
    if (op == 1)
    {
      (aux->semestreFechado) ? myfile << "\nSemestre fechado!!\n" : myfile << "\nSemestre aberto!!\n";
      myfile << "\n/------ALUNOS-----/\n";
      myfile.close();
    }
    else
    {
      (aux->semestreFechado) ? cout << "\nSemestre fechado!!" : cout << "\nSemestre aberto!!";
      cout << "\n/----------------/";
    }
    aux = aux->prox;
  }
}
//funcao que recebe a data distorcida ao contrario e printa corretamente
void printaData(int num)
{
  //Ex a data 20/08/2020 eh armazenada como 20200820 multipliacando o ano por 10000 mes por 100 e a adição do dia, essa função faz o processo contrario para printar a data legivel
  int aux;

  aux = num % 100;
  if (aux >= 10)
    cout << "\ndata: " << aux << "/";
  else
    cout << "\ndata: 0" << aux << "/";

  aux = num % 10000;
  aux = aux / 100;

  if (aux >= 10)
    cout << aux << "/";
  else
    cout << "0" << aux << "/";

  aux = num / 10000;
  cout << aux;
}
//aux será a disciplina encontrada. Se não encontrar retorna false.
bool encontraDisciplina(Disciplina **inicio, Disciplina **aux)
{
  //printa as disciplinas
  mostraDisciplinas(inicio);
  int cod;
  while (true)
  {
    cout << "\nDigite o codigo da Disciplina: ";
    cin >> cod;
    if (cod >= 0)
      break;
    else
      cout << "\nOpcao invalida.";
  }

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

void marcarPresenca(Alunos **inicio, int aulaCarga)
{
  Alunos *aux;
  aux = *inicio;
  //vai passar por todos os alunos da disciplina atualizando a lista deles de presença.
  do
  {
    Presenca *novo = new Presenca();
    novo->aulaCarga = aulaCarga;
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
void darAula(Disciplina **inicio, int padrao)
{
  Disciplina *aux;
  int cod;
  int op;
  //se encontrou a disciplina
  if (encontraDisciplina(inicio, &aux))
  {
    if (aux->semestreFechado)
      cout << "Semestre fechado nao eh possivel dar aulas";
    //se não tem alunos não tem como dar a aula.
    else if (aux->quantAlunos == 0)
      cout << "Disciplina sem alunos.";
    else
    {
      int dia, mes, ano, dataSemestre, dataAula;
      dataSemestre = aux->ano * 10000;
      if (aux->semestre == 2)
        dataSemestre += 601;
      while (true)
      {
        cout << "1 para aula, 2 para prova, 3 para trabalho: ";
        cin >> op;
        if (op > 0 && op < 4)
          break;
        else
          cout << "Opcao invalida.\n";
      }
      if (op == 1)
      {
        Aula *novo = new Aula();

        //contabilizando horas
        aux->CHRealizada += novo->qtdHoras;
        while (true)
        {
          cout << "Digite o numero de Ordem da aula: ";
          cin >> novo->numeroOrdem;
          if (novo->numeroOrdem >= 0)
            break;
          else
            cout << "Opcao invalida.\n";
        }

        //verificacao de data ser apos inicio do semestre
        while (true)
        {
          cout << "\nData da Aula DD MM YYYY: ";
          cin >> dia >> mes >> ano;

          dataAula = ano * 10000 + mes * 100 + dia;
          if (dataAula >= dataSemestre)
            break;

          cout << "A data deve ser depois do inicio do semestre";
          printaData(dataSemestre);
        }
        novo->data = dataAula;

        cout << "Conteudo da aula: ";
        cin.ignore();
        getline(cin, novo->conteudo);

        //cadastro completo
        if (padrao == 1)
        {
          cout << "Carga horaria da aula: ";
          cin >> novo->qtdHoras;
        }
        //marcando presenças e adicionando à lista.
        marcarPresenca(&aux->inicioAluno, novo->qtdHoras);
        //adicionando aula a lista.
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
          //verificacao da data apos inicio do semestre
          while (true)
          {
            cout << "\nData da Ativadade DD MM YYYY: ";
            cin >> dia >> mes >> ano;

            dataAula = ano * 10000 + mes * 100 + dia;
            if (dataAula >= dataSemestre)
              break;

            cout << "A data deve ser depois do inicio do semestre";
            printaData(dataAula);
          }
          novo->data = dataAula;
          while (true)
          {
            cout << "Peso da Atividade(0.2 seria 20%): ";
            cin >> novo->peso;
            if (novo->peso > 0 && novo->peso <= 1)
              break;
            else
              cout << "Opcao invalida.\n";
          }
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
                while (true)
                {
                  cout << "Digite a nota do aluno " << auxAlunos->nome << ": ";
                  cin >> auxAlunos->notas[i].nota;
                  if (auxAlunos->notas[i].nota > 0 && auxAlunos->notas[i].nota <= 10)
                    break;
                  else
                    cout << "Nota invalida.\n";
                }
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
      printaData(auxAula->data);
      cout << "\nConteudo: " << auxAula->conteudo << "\n";

      auxAula = auxAula->prox;
    }
  }
}

//pega os dados da disciplina e faz verificações.
//retorna true ou false para verificações futuras.
//é usado para criar disciplina e editar disciplina.
bool dadosDisciplina(Disciplina **inicio, Disciplina **novo, int padrao)
{
  int semestre, ano;

  while (true)
  {
    cout << "Ano [2013 - 2030]: ";
    cin >> ano;
    if (ano >= 2013 && ano <= 2030)
      break;
    else
      cout << "Ano invalido.\n";
  }

  while (true)
  {
    cout << "Semestre [1 ou 2]: ";
    cin >> semestre;
    if (semestre > 0 && semestre < 3)
      break;
    else
      cout << "Semestre invalido.\n";
  }

  if (!semestreAceitavel(ano, semestre, inicio))
  {
    cout << "\nDesculpe mas um professor só pode ministrar 5 aulas por semestre";
    delete (*novo);
    return false;
  }
  else
  {
    //é necessario atribuir a aux somente após verificação no caso de edição de uma disciplina.
    (*novo)->semestre = semestre;
    (*novo)->ano = ano;
  }

  cout << "Nome da Disciplina: ";

  cin.ignore();
  getline(cin, (*novo)->nome);

  do
  {
    cout << "Codigo da Disciplina (somente numeros): ";
    cin >> (*novo)->codigo;
  } while (!codAceitavel((*novo)->codigo, novo, inicio)); //passando novo em si para não incluir ele na verificação no caso de edição.

  //cadastro completo
  if (padrao == 1)
  {
    cout << "Carga Horaria prevista: ";
    cin >> (*novo)->CHPrevista;

    while (true)
    {
      cout << "Nota minima para aprovacao [0 a 10]: ";
      cin >> (*novo)->notaMin;
      if ((*novo)->notaMin > 0 && (*novo)->notaMin <= 10)
        break;
      else
        cout << "Nota invalida.\n";
    }

    while (true)
    {
      cout << "Presenca minima para aprovacao [0 a 100]: ";
      cin >> (*novo)->frequenciaMin;
      if ((*novo)->frequenciaMin > 0 && (*novo)->frequenciaMin <= 100)
        break;
      else
        cout << "Opcao invalida.\n";
    }
  }
  return true;
}

//edita uma disciplina ja existente.
void editaDisciplina(Disciplina **inicio)
{
  Disciplina *aux;
  //se encontrou a disciplina
  if (encontraDisciplina(inicio, &aux))
  {
    dadosDisciplina(inicio, &aux, 1); //ediçao completa
  }
}

//cadastra uma disciplina.
void cadastraDisciplina(Disciplina **inicio, Disciplina **fim, int padrao)
{

  Disciplina *novo = new Disciplina();
  //se retornar false, então ja chegou ao maximo de disciplinas por semestre, e não vai incluir na lista.
  if (dadosDisciplina(inicio, &novo, padrao))
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

  //se encontrar a disciplina
  if (encontraDisciplina(inicio, &aux))
  {
    //maximo de 50
    if (aux->semestreFechado)
      cout << "Semestre fechado, nao eh possivel cadastrar alunos";

    else if (aux->quantAlunos == 50)
    {
      cout << "Desculpe, disciplina lotada.";
      return;
    }
    else
    {
      //maximo de 50
      while (true)
      {
        //nao pode numero negativo
        while (true)
        {
          cout << "\nDigite o numero de alunos que deseja cadastrar: ";
          cin >> numAlunos;
          if (numAlunos > 0)
            break;
          else
            cout << "Numero invalido.";
        }
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
        cout << "Digite o nome do aluno: ";
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
          while (true)
          {
            cout << "Digite o numero do aluno: ";
            cin >> novo->numero;
            if (novo->numero >= 0)
              break;
            else
              cout << "Opcao invalida.\n";
          }
          do
          {
            if (novo->numero == auxAlunos->numero && auxAlunos != novo)
            {
              cout << "Numero ja existente.\n";
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
float calculaPresenca(Presenca **pres, int cargaTotal)
{
  float presencaFinal = 0;

  //percorre todas as presencas de um aluno
  while (*pres != NULL)
  {

    if ((*pres)->p == 'P' || (*pres)->p == 'p')
    {
      presencaFinal += (*pres)->aulaCarga;
    }
    *pres = (*pres)->prox;
  }
  presencaFinal = presencaFinal / cargaTotal;

  //eh retornado a presenca em porcentagem
  return presencaFinal * 100;
}

void fechaSemestre(Disciplina **inicio)
{
  Disciplina *aux;

  //se encontrar a discplina
  if (encontraDisciplina(inicio, &aux))
  {

    Alunos *auxAluno;
    auxAluno = aux->inicioAluno;
    ProvaTrab *auxProvaTrab;
    auxProvaTrab = aux->inicioProva;
    Aula *auxAula;
    auxAula = aux->inicioAula;

    cout << "\n---Fechando semestre de " << aux->nome << "---\n";

    //a disciplina sendo fechada possui alunos
    if (auxAluno != NULL)
    {

      bool temAvaliacao = false, temAula = false;

      if (auxProvaTrab != NULL)
        temAvaliacao = true;
      if (aux->CHRealizada != 0)
        temAula = true;

      //a disciplina possui aulas e avaliacoes para gerar notas e presenca
      if (temAvaliacao && temAula)
      {

        aux->semestreFechado = true;

        do
        { //percorre todos os alunos e calcula suas notas e presencas

          Presenca *auxPres;
          auxPres = auxAluno->inicioPres;
          float notaFinal = 0.0;
          float presencaFinal = 0.0;

          //retorna a presenca de um aluno em porcentagem
          presencaFinal = calculaPresenca(&auxPres, aux->CHRealizada);

          //loop para calculo de media
          for (int i = 0; i < 14 && auxAluno->notas[i].nota != -1; i++)
          {
            notaFinal += auxAluno->notas[i].nota * auxAluno->notas[i].peso;
          }
          auxAluno->media = notaFinal;

          if (presencaFinal >= aux->frequenciaMin && notaFinal >= aux->notaMin)
            auxAluno->aprovado = true;
          else if (presencaFinal >= aux->frequenciaMin && notaFinal < aux->notaMin)
            auxAluno->exame = true;

          cout << "\nAluno: " << auxAluno->nome;
          cout << "\nNota final: " << auxAluno->media;
          cout << "\nPercentual de presenca: " << presencaFinal;
          if (auxAluno->aprovado)
            cout << "\nAprovado: true\n";

          //se nao estiver aprovado
          else
          {
            cout << "\nAprovado: false";

            //se estiver de exame
            if (auxAluno->exame)
            {
              cout << "\nEsta de exame: true";
              cout << "\nNota necessaria no exame para " << auxAluno->nome << " passar: " << aux->notaMin * 2 - auxAluno->media;
              cout << "\n--Aplicando exame para " << auxAluno->nome << "--";
              while (true)
              {
                cout << "\nNota no exame: ";
                cin >> auxAluno->notaExame;
                if (auxAluno->notaExame >= 0 && auxAluno->notaExame <= 10)
                  break;
                else
                  cout << "\nNota invalida";
              }

              auxAluno->media = (auxAluno->media + auxAluno->notaExame) / 2;

              if (auxAluno->media > aux->notaMin)
              {
                cout << "Aluno Aprovado\n";
                auxAluno->aprovado = true;
              }
              else
                cout << "Aluno Reprovado\n";
            }
            //nao esta qualificado para exame
            else
              cout << "\nEsta de exame: false\n";
          }

          auxAluno = auxAluno->prox;
        } while (auxAluno != aux->inicioAluno);
      }
      //caso tente fechar o semestre possuindo alunos, porem sem avaliacoes ou aulas
      else if (!temAvaliacao && !temAula)
        cout << "\nNao eh possivel dar notas sem avaliacoes e presenca sem aulas.";
      else if (!temAvaliacao)
        cout << "\n"
             << aux->nome << " Nao possui avaliacoes.";
      else
        cout << "\n"
             << aux->nome << " Nao possui aulas.";
    }
    //fechando semestre sem alunos
    else
    {
      cout << "\nSemestre fechado sem alunos";
      aux->semestreFechado = true;
    }
  }
}

void printaAprovadosReprovados(Disciplina **inicio)
{
  Disciplina *aux;
  //se encontrar a disciplina.
  if (encontraDisciplina(inicio, &aux))
  {
    //se a disciplina ja tiver fechado o semestre.
    if (aux->semestreFechado)
    {

      cout << "\n--Mostrando situacoes dos alunos de " << aux->nome << "--";

      Alunos *auxAlunos = aux->inicioAluno;
      if (auxAlunos != NULL)
      {
        do
        {
          cout << "\n\nNome: " << auxAlunos->nome << "\nMedia: " << auxAlunos->media;
          if (auxAlunos->aprovado)
            cout << "\nAprovado";

          else
          { // se o aluno for reprovado.

            float presenca = calculaPresenca(&auxAlunos->inicioPres, aux->CHRealizada);
            if (presenca < aux->frequenciaMin)
            {

              if (auxAlunos->media < aux->notaMin)
                cout << "\nReprovado por frequencia e nota";
              else
                cout << "\nReprovado por frequencia";
            }
            else
              cout << "\nReprovado por nota";
          }

          auxAlunos = auxAlunos->prox;
        } while (auxAlunos != aux->inicioAluno); //percorre por todos os alunos na lista circular.
      }
    }
    else
    { //se o semestre ainda estiver aberto
      cout << "\nSemestre de " << aux->nome << " nao foi fechado, nao ha aprovados e reprovados";
    }
  }
}
//função usada na função printaAlunos
void printaAlunoAux(Disciplina **aux, bool opFile, bool opAluno = 0)
{
  ofstream myfile;
  Alunos *auxAlunos;
  Presenca *auxPresenca;
  int numero;
  int achou = 0;
  if (opFile == 1)
    myfile.open((*aux)->nome + ".txt", ios_base::app);
  auxAlunos = (*aux)->inicioAluno;
  //se for true, então é pra printar apenas um aluno.
  if (opAluno)
  {
    while (true)
    {
      cout << "Digite o numero do aluno: ";
      cin >> numero;
      if (numero >= 0)
        break;
      else
        cout << "Opcao invalida.\n";
    }
    do
    {
      if (auxAlunos->numero == numero)
      {
        achou = 1;
        break;
      }
      auxAlunos = auxAlunos->prox;
    } while (auxAlunos != (*aux)->inicioAluno);
    if (!achou)
    {
      cout << "Aluno nao encontrado.\n";
      return;
    }
  }
  //se ela tiver alunos
  if (auxAlunos != NULL)
  {
    do
    {
      auxPresenca = auxAlunos->inicioPres;
      //printa numero e nome
      if (opFile == 1)
      {
        myfile << "\n/--------------/\n";
        myfile << "Nome: " << auxAlunos->nome << "\n"
               << "Numero: " << auxAlunos->numero << "\n";
      }
      else
      {
        cout << "\n/----------------/\n";
        cout << "Nome: " << auxAlunos->nome << "\n"
             << "Numero: " << auxAlunos->numero << "\n";
      }
      auxPresenca = auxAlunos->inicioPres;
      //se ja tiver tido pelomenos uma aula, printa as presenças.
      if (auxPresenca != NULL)
      {
        int presencas = 0, faltas = 0;
        do
        {
          if (auxPresenca->p == 'P' || auxPresenca->p == 'p')
            presencas++;
          else
            faltas++;
          auxPresenca = auxPresenca->prox;
        } while (auxPresenca != NULL);
        if (opFile == 1)
        {
          myfile << "Presencas: " << presencas << "\n";
          myfile << "Faltas: " << faltas << "\n";
          myfile << "Percentual de faltas: " << (float)(faltas * 100) / (faltas + presencas) << "%\n";
        }
        else
        {
          cout << "Presencas: " << presencas << "\n";
          cout << "Faltas: " << faltas << "\n";
          cout << "Percentual de faltas: " << (float)(faltas * 100) / (faltas + presencas) << "%\n";
        }
      }
      //se ja tiver tido pelomenos uma nota atribuida
      if (auxAlunos->notas[0].nota != -1)
      {
        if (opFile == 1)
          myfile << "/----Notas----/ \n";
        else
          cout << "/----Notas----/ \n";
        //enquanto não passar de 14 e não encontrar uma posição vazia(indicada por -1)
        for (int i = 0; auxAlunos->notas[i].nota != -1 && i < 14; i++)
        {
          if (opFile == 1)
          {
            myfile << "Nome: " << auxAlunos->notas[i].nome << "\n"
                   << "Peso: " << auxAlunos->notas[i].peso << "\n"
                   << "Nota: " << auxAlunos->notas[i].nota << "\n";
            myfile << "/-------/\n";
          }
          else
          {
            cout << "Nome: " << auxAlunos->notas[i].nome << "\n"
                 << "Peso: " << auxAlunos->notas[i].peso << "\n"
                 << "Nota: " << auxAlunos->notas[i].nota << "\n";
            cout << "/-------/\n";
          }
        }
      }
      if ((*aux)->semestreFechado)
      {
        if (opFile == 1)
        {
          myfile << "Media Final: " << auxAlunos->media << "\nSituacao: ";
          if (auxAlunos->aprovado)
            myfile << "Aprovado.\n";
          else if (auxAlunos->media < (*aux)->notaMin)
            myfile << "Reprovado por nota.\n";
          else
            myfile << "Reprovado por presenca.\n";
        }
        else
        {
          cout << "Media Final: " << auxAlunos->media << "\nSituacao: ";
          if (auxAlunos->aprovado)
            cout << "Aprovado.\n";
          else if (auxAlunos->media < (*aux)->notaMin)
            cout << "Reprovado por nota.\n";
          else
            cout << "Reprovado por presenca.\n";
        }
      }
      auxAlunos = auxAlunos->prox;
      //enquanto não chegar ao fim, e não opAluno(se for true, vai printar só um aluno.)
    } while (auxAlunos != (*aux)->inicioAluno && !opAluno);
  }
  else if (!opFile)
    cout << "\nNao existem alunos nessa disciplina.";
}

//printa as informações dos alunos de uma disciplina.
void printaAluno(Disciplina **inicio, bool opFile = 0, int op = 0)
{
  Disciplina *aux;
  //se quiser printar alunos de todas as disciplinas
  if (op == 3)
  {
    aux = *inicio;
    do
    {
      printaAlunoAux(&aux, opFile);
      aux = aux->prox;
    } while (aux != NULL);
  }
  //se quiser de uma só disciplina
  else if (op == 2)
  {
    if (encontraDisciplina(inicio, &aux))
    {
      printaAlunoAux(&aux, opFile);
    }
  }
  //printa um aluno de uma disciplina.
  else if (op == 1)
  {
    if (encontraDisciplina(inicio, &aux))
    {
      printaAlunoAux(&aux, opFile, 1);
    }
  }
}

//imprime em arquivos separados para cada disciplina os dados dela e dos alunos.
void printaRelatorioTotal(Disciplina **inicio)
{
  Disciplina *aux = *inicio;
  if (aux == NULL)
    cout << "Nao existem disciplinas.";
  else
  {
    mostraDisciplinas(inicio, 1);
    printaAluno(inicio, 1, 3);
  }
}

int main()
{
  Disciplina *inicio = NULL, *fim = NULL;

  string nome;
  int op, padrao;
  cout << "Sistema de Notas e Frequencias";
  cout << "\nNome do professor: ";
  cin.ignore();
  getline(cin, nome);
  while (true)
  {
    cout << "\n1- Fazer cadastramentos completos";
    cout << "\n2- Utilizar algumas propriedades padrao";
    cout << "\nEscolha: ";
    cin >> padrao;
    if (padrao == 1 || padrao == 2)
      break;
    else
      cout << "Escolha invalida.\n";
  }

  do
  {
    while (true)
    {
      cout << "\n\nSistema de Notas e Frequencias";
      cout << "\n1- Cadastra nova Disciplina";
      cout << "\n2- Editar Disciplina";
      cout << "\n3- Mostrar Disciplinas";
      cout << "\n4- Mostrar alunos de determinada disciplina";
      cout << "\n5- Mostrar um aluno de uma disciplina";
      cout << "\n6- Ministrar aula de uma disciplina / dar Avaliacao";
      cout << "\n7- Mostrar aulas de uma Disciplina";
      cout << "\n8- Cadastrar alunos em uma Disciplina";
      cout << "\n9- Fechamento de semestre com Aplicacao de exames/recuperacao";
      cout << "\n10- Relatorio dos Aprovados e Reprovados";
      cout << "\n11- Imprimir relatório geral de cada disciplina";
      cout << "\n12- Sair";
      cout << "\nSua escolha: ";

      cin >> op;
      if (op > 0 && op < 13)
        break;
      else
        cout << "\nEscolha invalida.";
    }

    switch (op)
    {
    case 1:
      cadastraDisciplina(&inicio, &fim, padrao);
      break;
    case 2:
      editaDisciplina(&inicio);
      break;
    case 3:
      mostraDisciplinas(&inicio);
      break;
    case 4:
      printaAluno(&inicio, 0, 2);
      break;
    case 5:
      printaAluno(&inicio, 0, 1);
      break;
    case 6:
      darAula(&inicio, padrao);
      break;
    case 7:
      mostraAulas(&inicio);
      break;
    case 8:
      cadastraAluno(&inicio);
      break;
    case 9:
      fechaSemestre(&inicio);
      break;
    case 10:
      printaAprovadosReprovados(&inicio);
      break;
    case 11:
      printaRelatorioTotal(&inicio);
      break;
    default:
      break;
    }
  } while (op != 12);
  return 0;
}