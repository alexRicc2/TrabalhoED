#include <iostream>
#include <string>
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
  int quantProvasCumpridas;
  float maiorNota, menorNota, TotalNota;

  aux = *inicio;
  cout << "\n---Mostrando todas as Disciplinas---\n";

  while (aux != NULL)
  {
    int qtdAvaliacoes = aux->provasDadas + aux->trabalhosDados;
    int faltas=0,presente=0;

    cout << "\n"
         << aux->nome << " codigo: " << aux->codigo << "\nCarga horaria prevista: "
         << aux->CHPrevista << "\nCarga horaria cumprida: " << aux->CHRealizada << "\nNumero de alunos: " << aux->quantAlunos << "\nProvas dadas: " << aux->provasDadas << "\nTrabalhos dados: " << aux->trabalhosDados;

         //se a disciplina possuir alunos
         if(aux->inicioAluno != NULL){
            Alunos *auxAluno = aux->inicioAluno;

            maiorNota=0, menorNota=10, TotalNota=0, quantProvasCumpridas=0;

            do{ //percorre todos alunos

              //se possuir avaliacoes
              if(qtdAvaliacoes > 0){
                //calcula a maior,menor e media da nota
                for(int i=0; i< 14 && auxAluno->notas[i].nota != -1; i++){
              
                if(auxAluno->notas[i].nota > maiorNota)maiorNota = auxAluno->notas[i].nota;
                if(auxAluno->notas[i].nota < menorNota)menorNota = auxAluno->notas[i].nota;
                TotalNota += auxAluno->notas[i].nota;
                quantProvasCumpridas++;
                
                }
              }
              //se possuir aulas
              if(aux->inicioAula != NULL){
                Presenca *auxPres;
                auxPres = auxAluno->inicioPres;

                //contabiliza as presencas de um aluno
                while(auxPres != NULL){
                  if(auxPres->p == 'f')faltas++;
                  else presente++;

                  auxPres = auxPres->prox;
                }
              }

              auxAluno = auxAluno->prox;
            }while(auxAluno != aux->inicioAluno);
            
            //printando infromacoes de notas caso foram dadas
            if(qtdAvaliacoes > 0){
              cout << "\nMaior nota tirada em uma avaliacao: " << maiorNota;
              cout << "\nMenor nota tirada em uma avaliacao: " << menorNota;
              cout << "\nMedia de notas nas avaliacoes: " << TotalNota / quantProvasCumpridas;
            }
            if(aux->inicioAula != NULL){
              float percentual = faltas/float((faltas+presente)) * 100;

              cout << "\nQuantidade faltas: " << faltas;
              cout << "\nPercentual de faltas: " << percentual << "%";
            }
         }
         
         (aux->semestreFechado) ? cout << "\nSemestre fechado!!" : cout << "\nSemestre aberto!!";     
         cout <<"\n/----------------/";
    aux = aux->prox;
  }
}
//funcao que recebe a data distorcida ao contrario e printa corretamente
void printaData(int num){ 
	
		int aux;
		
		aux = num%100;
		if(aux >=10)cout << "\ndata: "<< aux << "/";
    else cout << "\ndata: 0" << aux << "/";
		
		aux = num%10000;
		aux = aux/100;

    if(aux >=10) cout << aux << "/";
		else cout << "0" << aux << "/";

		aux = num/10000;
    cout << aux;
	
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
    if(aux->semestreFechado)
      cout << "Semestre fechado nao eh possivel dar aulas";
    //se não tem alunos não tem como dar a aula.
    else if (aux->quantAlunos == 0)
      cout << "Disciplina sem alunos.";
    else
    {
      int dia,mes,ano,dataSemestre,dataAula;
      dataSemestre = aux->ano * 10000;
      if(aux->semestre == 2)dataSemestre += 601;

      cout << "1 para aula, 2 para prova, 3 para trabalho: ";
      cin >> op;
      if (op == 1)
      {
        Aula *novo = new Aula();
        
        //contabilizando horas
        aux->CHRealizada += novo->qtdHoras;

        cout << "Digite o numero de Ordem da aula: ";
        cin >> novo->numeroOrdem;

        //verificacao de data ser apos inicio do semestre
        while(true){
        cout << "\nData da Aula DD MM YYYY: ";
        cin >> dia >> mes >> ano;
        
        dataAula = ano * 10000 + mes * 100 + dia;
        if(dataAula >= dataSemestre)break;

        cout << "A data deve ser depois do inicio do semestre";
        printaData(dataSemestre);
        }
        novo->data = dataAula;

        cout << "Conteudo da aula: ";
        cin.ignore();
        getline(cin, novo->conteudo);
        
        //cadastro completo
        if(padrao == 1){
          cout << "Carga horaria da aula: ";
          cin >> novo->qtdHoras;
        }
        //marcando presenças
        marcarPresenca(&aux->inicioAluno, novo->qtdHoras);

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
          while(true){
            cout << "\nData da Ativadade DD MM YYYY: ";
            cin >> dia >> mes >> ano;
        
            dataAula = ano * 10000 + mes * 100 + dia;
            if(dataAula >= dataSemestre)break;

            cout << "A data deve ser depois do inicio do semestre";
            printaData(dataAula);
          }
          novo->data = dataAula;

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

  do
  {
    cout << "Ano [2013 - 2030]: ";
    cin >> ano;
  } while (ano < 2013 || ano > 2030);

  do
  {
    cout << "Semestre [1 ou 2]: ";
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

  cout << "Nome da Disciplina: ";

  cin.ignore();
  getline(cin, (*novo)->nome);

  do
  {
    cout << "Codigo da Disciplina (somente numeros): ";
    cin >> (*novo)->codigo;
  } while (!codAceitavel((*novo)->codigo, novo, inicio)); //passando novo em si para não incluir ele na verificação no caso de edição.

  //cadastro completo
  if(padrao == 1){ 
    cout << "Carga Horaria prevista: ";
    cin >> (*novo)->CHPrevista;

    do{
      cout << "Nota minima para aprovacao [0 a 10]: ";
      cin >> (*novo)->notaMin;
    }while((*novo)->notaMin < 0 || (*novo)->notaMin > 10);

    do{
      cout << "Presenca minima para aprovacao [0 a 100]: ";
      cin >> (*novo)->frequenciaMin;
    }while((*novo)->frequenciaMin < 0 || (*novo)->frequenciaMin > 100);
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
    if(aux->semestreFechado)
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
float calculaPresenca(Presenca **pres, int cargaTotal){
  float presencaFinal = 0;
  
  //percorre todas as presencas de um aluno
  while(*pres != NULL){
            
    if((*pres)->p == 'P' || (*pres)->p == 'p'){
      presencaFinal += (*pres)->aulaCarga;
    }
    *pres= (*pres)->prox;
  }
  presencaFinal = presencaFinal / cargaTotal;
  
  //eh retornado a presenca em porcentagem
  return presencaFinal*100;
}

void fechaSemestre(Disciplina **inicio){
  Disciplina *aux;

  //se encontrar a discplina
  if (encontraDisciplina(inicio, &aux)){
    
    Alunos *auxAluno; 
    auxAluno = aux->inicioAluno;
    ProvaTrab *auxProvaTrab;
    auxProvaTrab = aux->inicioProva;
    Aula *auxAula;
    auxAula = aux->inicioAula;
    
    cout << "\n---Fechando semestre de " << aux->nome <<"---\n";

    //a disciplina sendo fechada possui alunos
    if(auxAluno != NULL){
      
      bool temAvaliacao, temAula;

      if(auxProvaTrab != NULL)temAvaliacao=true; 
      if(aux->CHRealizada != 0)temAula = true;

      //a disciplina possui aulas e avaliacoes para gerar notas e presenca
      if(temAvaliacao && temAula){

        aux->semestreFechado = true;
        
        do{ //percorre todos os alunos e calcula suas notas e presencas

          Presenca *auxPres;
          auxPres = auxAluno->inicioPres;
          float notaFinal = 0.0;
          float presencaFinal = 0.0;
          
          //retorna a presenca de um aluno em porcentagem
          presencaFinal = calculaPresenca(&auxPres, aux->CHRealizada);

          //loop para calculo de media
          for(int i=0;i<14 && auxAluno->notas[i].nota != -1 ;i++){
              notaFinal += auxAluno->notas[i].nota * auxAluno->notas[i].peso;
          }
          auxAluno->media = notaFinal;

          if(presencaFinal > aux->frequenciaMin && notaFinal > aux->notaMin)auxAluno->aprovado = true;
          else if(presencaFinal > aux->frequenciaMin && notaFinal < aux->notaMin)auxAluno->exame = true;

          cout << "\nAluno: " << auxAluno->nome;
          cout << "\nNota final: " << auxAluno->media;
          cout << "\nPercentual de presenca: " << presencaFinal;
          if(auxAluno->aprovado) cout << "\nAprovado: true\n";
          
          //se nao estiver aprovado
          else{ 
            cout << "\nAprovado: false";

            //se estiver de exame
            if(auxAluno->exame){
              cout <<"\nEsta de exame: true";
              cout << "\nNota necessaria no exame para " << auxAluno->nome <<  " passar: " << aux->notaMin*2 - auxAluno->media;            
              cout << "\n--Aplicando exame para " << auxAluno->nome << "--";
              cout << "\nNota no exame: ";
              cin >> auxAluno->notaExame;

              auxAluno->media = (auxAluno->media + auxAluno->notaExame) / 2;

              if(auxAluno->media > aux->notaMin){
                cout << "Aluno Aprovado\n";
                auxAluno->aprovado = true;
              }
              else cout << "Aluno Reprovado\n";  
            }
            //nao esta qualificado para exame
            else cout << "\nEsta de exame: false\n";
          }

          auxAluno = auxAluno->prox;
        }while(auxAluno != aux->inicioAluno);
        
        
      }
      //caso tente fechar o semestre possuindo alunos, porem sem avaliacoes ou aulas
      else if(!temAvaliacao && !temAula)cout << "\nNao eh possivel dar notas sem avaliacoes e presenca sem aulas";
      else if(!temAvaliacao)cout << "\nNao "<< aux->nome<< " possui avaliacoes";
      else cout << "\nNao "<<  aux->nome <<" possui aulas";
      
    }
    //fechando semestre sem alunos
    else{
      cout << "\nSemestre fechado sem alunos";
      aux->semestreFechado = true;
    }
    
  }
}

void printaAprovadosReprovados(Disciplina **inicio){
  Disciplina *aux;
  //se encontrar a disciplina.
  if (encontraDisciplina(inicio, &aux)){
    //se a disciplina ja tiver fechado o semestre.
    if(aux->semestreFechado){

      cout << "\n--Mostrando situacoes dos alunos de " << aux->nome << "--";

      Alunos *auxAlunos = aux->inicioAluno;
      if(auxAlunos != NULL){
        do{
          cout << "\n\nNome: " << auxAlunos->nome << "\nMedia: "<< auxAlunos->media;
          if(auxAlunos->aprovado) cout << "\nAprovado";
          
          else{ // se o aluno for reprovado.
          
            float presenca = calculaPresenca(&auxAlunos->inicioPres, aux->CHRealizada);
            if(presenca < aux->frequenciaMin){

              if(auxAlunos->media < aux->notaMin)cout << "\nReprovado por frequencia e nota";
              else cout << "\nReprovado por frequencia"; 
            }
            else cout << "\nReprovado por nota";
          }

          auxAlunos = auxAlunos->prox;
        }while(auxAlunos != aux->inicioAluno); //percorre por todos os alunos na lista circular.
          
      }
    }
    else{ //se o semestre ainda estiver aberto
      cout << "\nSemestre de " << aux->nome << " nao foi fechado, nao ha aprovados e reprovados";
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
  int op, padrao;
  cout << "Sistema de Notas e Frequencias";
  cout << "\nNome do professor: ";
  cin.ignore();
  getline(cin, nome);
  cout << "\n1- Fazer cadastramentos completos";
  cout << "\n2- Utilizar algumas propriedades padrao";
  cout << "\nEscolha: ";
  cin >> padrao;

  do
  {
    cout << "\n\nSistema de Notas e Frequencias";
    cout << "\n1- Cadastra nova Disciplina";
    cout << "\n2- Editar Disciplina";
    cout << "\n3- Mostrar Disciplinas";
    cout << "\n4- Mostrar alunos de determinada disciplina";
    cout << "\n5- Ministrar aula de uma disciplina / dar Avaliacao";
    cout << "\n6- Mostrar aulas de uma Disciplina"; 
    cout << "\n7- Cadastrar alunos em uma Disciplina";
    cout << "\n8- Fechamento de semestre com Aplicacao de exames/recuperacao";
    cout << "\n9- Relatorio dos Aprovados e Reprovados"; 
    cout << "\nSua escolha: ";

    cin >> op;

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
      printaAluno(&inicio);
      break;
    case 5:
      darAula(&inicio, padrao);
      break;
    case 6:
      mostraAulas(&inicio);
      break;
    case 7:
      cadastraAluno(&inicio);
      break;
    case 8:
      fechaSemestre(&inicio);
      break;
    case 9:
      printaAprovadosReprovados(&inicio);
      break;
    default:
      break;
    }

  } while (op != 0);
}