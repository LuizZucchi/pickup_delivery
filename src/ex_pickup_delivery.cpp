// Project and Analysis of Algorithms
// 
// Laboratorio 1
//
// Send corrections/comments to Fl�vio K. Miyazawa


#include <iostream>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <lemon/list_graph.h>
#include "mygraphlib.h"
#include <string>
#include "myutils.h"
#include <lemon/concepts/digraph.h>
#include <lemon/preflow.h>
#include <vector>
#include <lemon/kruskal.h>
#include <algorithm>
#include <chrono>

// #define nao estava funcionando
// cte para definir o numero de 2-OPT realizados pela heuristica
const int swaps = 100;

using namespace lemon;
using namespace std;
using namespace std::chrono;

typedef vector<DNode> DNodeVector;

int cutcount = 0;

// Pickup_Delivery_Instance put all relevant information in one class.
class Pickup_Delivery_Instance {
public:
  Pickup_Delivery_Instance(Digraph &graph,
			   DNodeStringMap &vvname,
			   DNodePosMap &posx,
			   DNodePosMap &posy,
			   ArcValueMap &eweight,
			   DNode &sourcenode,
			   DNode &targetnode,
			   int &npairs,
			   DNodeVector& pickup,
			   DNodeVector& delivery);
  Digraph &g;
  DNodeStringMap &vname;
  DNodePosMap &px;
  DNodePosMap &py;
  ArcValueMap &weight;
  int nnodes;
  DNode &source;
  DNode &target;
  int npairs;
  DNodeVector &pickup;
  DNodeVector &delivery;
};

Pickup_Delivery_Instance::Pickup_Delivery_Instance(Digraph &graph,
				 DNodeStringMap &vvname,
				 DNodePosMap &posx,
				 DNodePosMap &posy,
				 ArcValueMap &eweight,
				 DNode &sourcenode,
						   DNode &targetnode,
						   int& vnpairs,
						   DNodeVector& vpickup,
						   DNodeVector& vdelivery):
  g(graph), vname(vvname), px(posx), py(posy), weight(eweight),
  source(sourcenode), target(targetnode),npairs(vnpairs),pickup(vpickup),delivery(vdelivery)
{
  nnodes = countNodes(g);
}

void PrintInstanceInfo(Pickup_Delivery_Instance &P)
{
  cout << endl << endl;
  cout << "Pickup Delivery Graph Informations" << endl;
  cout << "\tSource = " << P.vname[P.source] << endl;
  cout << "\tTarget = " << P.vname[P.target] << endl;
  for (int i=0;i<P.npairs;i++) {
    cout << "\tPair pickup-->delivery: "
	 << P.vname[P.pickup[i]]
	 << " --> "
	 << P.vname[P.delivery[i]] << endl;
  }
  cout<<endl;
}

void PrintSolution(Pickup_Delivery_Instance &P,DNodeVector &Sol,string msg)
{
  // Imprime a solucao no terminal.
  cout<<msg<<endl<< "\t";
  cout << P.vname[Sol[0]];
  for (int i=1;i<P.nnodes;i++)
    cout << "-->" << P.vname[Sol[i]];
  cout<<endl;

}

bool ReadPickupDeliveryDigraph(string filename,
			       Digraph &g,
			       DNodeStringMap& vname,
			       DNodePosMap& posx,
			       DNodePosMap& posy,
			       ArcValueMap& weight,
			       DNode& source,
			       DNode& target,
			       int &npairs,
			       DNodeVector& pickup,
			       DNodeVector& delivery)
{
  ReadDigraph(filename,g,vname,posx,posy,weight);
  int n=countNodes(g);
  DNode DN[n];
  if ((n<4) || (n%2)) {
    cout << "Numero de vertices "<<n<<" no grafo nao eh par ou eh menor que 4." << endl;
    return(0);
  }
  npairs = (n-2)/2;
  pickup.resize(npairs);
  delivery.resize(npairs);
  int i=0;
  for(DNodeIt v(g);v!=INVALID;++v) { DN[i] = v; i++; }
  
  source = DN[0];
  target = DN[1];
  for (int i=0;i<npairs;i++) {
    pickup[i] = DN[2*i+2];
    delivery[i] = DN[2*i+3];
  }
  return(1);
}

double sqr(double a) {  return(a*a); }
double dist(Pickup_Delivery_Instance &P,DNode u,DNode v)
{  return(sqrt(sqr(P.px[u]-P.py[v])+sqr(P.px[v]-P.py[v]))); }

// Heuristica apenas para testar a visualizacao das solucoes.
bool HeuristicaConstrutivaBoba(Pickup_Delivery_Instance &P,int time_limit,double &LB,double &UB,DNodeVector &Sol)
{
  cout << "Execucao da Heuristica Boba" << endl;
  cout << "\tEsta rotina deveria respeitar o tempo de no maximo "
       << time_limit << " segundos" << endl;
  cout<<"\tProvavelmente a solucao eh inviavel, pois mistura as coletas e entregas."<< endl;
  if (UB==MY_INF) { // Faz alguma coisa so' se ainda nao tem solucao
    Sol.resize(P.nnodes);
    Sol[0] = P.source; // insere o source
    for (int i=0;i<P.npairs;i++) Sol[i+1] = P.pickup[i]; // insere os pickup
    for (int i=0;i<P.npairs;i++) Sol[P.npairs+i+1] = P.delivery[i]; // insere os delivery
    Sol[2*P.npairs+1] = P.target; // insere o target.

    // Ordena os vertices, exceto pelo source e target, do mais baixo para mais alto
    int primeiro=1, ultimo=2*P.npairs;
    for (int i=primeiro;i<ultimo;i++) 
      for (int j=i+1;j<=ultimo;j++) 
	if (P.py[Sol[i]]>P.py[Sol[j]]){DNode aux;aux=Sol[i];Sol[i]=Sol[j];Sol[j]=aux;}

    // Atualiza o UB (Upper Bound) que eh o valor da solucao
    UB = 0.0;
    for (int i=1;i<P.nnodes;i++)
      for (OutArcIt a(P.g,Sol[i-1]);a!=INVALID;++a)
	if(P.g.target(a)==Sol[i]){UB += P.weight[a];break;}
  }
  cout<<endl;
  return(1);
}

bool ViewPickupDeliverySolution(Pickup_Delivery_Instance &P,double &LB,double &UB,DNodeVector &Sol,string msg)
{
  DigraphAttributes GA(P.g,P.vname,P.px,P.py);
  GA.SetDefaultDNodeAttrib("color=LightGray style=filled width=0.03 height=0.03 fixedsize=false");
  for (ArcIt a(P.g); a!=INVALID; ++a) GA.SetColor(a,"Invis");
  GA.SetColor(P.source,"Red"); // source and target are painted in White
  GA.SetColor(P.target,"Red");
  GA.SetAttrib(P.source,"shape=box");
  GA.SetAttrib(P.target,"shape=box");
  
  if (P.npairs <= 16){ // se tiver poucos pares, dah para pintar os pares de mesma cor.
    for (int i=0;i<P.npairs;i++){ // pinta 
      GA.SetColor(P.pickup[i],ith_VisualDistinctColorName(i));
      GA.SetColor(P.delivery[i],ith_VisualDistinctColorName(i));
    }}
  for (int i=1;i<P.nnodes;i++) {
    // pinta o arco Sol[i-1] -->  Sol[i]
    for (OutArcIt a(P.g,Sol[i-1]);a!=INVALID;++a)
      if(P.g.target(a)==Sol[i]){
	GA.SetColor(a,"Red");
	break;
      }
  }
  GA.SetLabel("Path from node "+P.vname[P.source]+" to node "+P.vname[P.target]+
	      " of value "+DoubleToString(UB)+". LB = "+DoubleToString(LB)+ ". "+msg);
  GA.View();
  return(1);
}



/*                      PROJETO 2                     */   
/*            Metaheuristica baseada em GRASP         */
/*                                                    */

/*                funcoes auxiliares             */

//Funcao para checar se um noh esta no vetor solucao
//Retorno: true - nao esta
//         false- esta
bool NotInSolution(Pickup_Delivery_Instance &P,DNodeVector &Sol, DNode node){
  bool notin = true;
  
  //checa se o node esta no vetor solucao
  for(int i; i < Sol.size(); i++){
    if(P.vname[Sol[i]] == P.vname[node]){
      notin = false;
    }
  }

  return notin;
}


//Funcao para identificar o index de um noh delivery
//Retorno: delivery index
int FindDeliveryIndex(Pickup_Delivery_Instance &P, DNode node){
  int i = 0;

  //procura pelo index no vetor delivery que represente o node
  for(i = 0; i < P.npairs; i++){
    if(P.vname[node] == P.vname[P.delivery[i]]){
      break;
    }
  }

  //caso nao encontre, o node eh um pickup e o i == P.npairs
  return i;
}


//Funcao para identificar se o noh eh um delivery ou pickup
//Retorno: pickup 
//         delivery
string CheckPickupDelivery(Pickup_Delivery_Instance &P, DNode node){

  if (FindDeliveryIndex(P, node) == P.npairs){
    return "pickup";
  }else{
    return "delivery";
  }
}


//Funcao para validar se a insercao do noh na solucao respeita a ordem pickupdelivery
//Retorno: true - valido
//         false- nao eh valido
bool PickupDeliveryValid(Pickup_Delivery_Instance &P, DNode node, DNodeVector &Sol){
  int idx = 0;
  bool isvalid;

  idx = FindDeliveryIndex(P, node);

  //caso o index seja igual o npairs, o node eh um pickup e pode ser inserido diretamente
  if(idx == P.npairs){

    isvalid = true;
  }
  //caso contrario, o node eh um delivery e deve ser checado se seu par pickup esta na solucao
  else{
    
    isvalid = !NotInSolution(P, Sol, P.pickup[idx]);
  }

  return isvalid;
}


//Funcao para trocar dois nos de posicao no vetor solucao
//Retorno:
void SwapNodes(DNodeVector &Sol, int i, int j){
  DNode aux;

  aux = Sol[i];
  Sol[i] = Sol[j];
  Sol[j] = aux;

  return ;
}


//Funcao para calcular o custo da nova solucao valida
//Retorno: custo da solucao
double CostSol(Pickup_Delivery_Instance &P, DNodeVector &Sol){
  double cost = 0.0;

  for (int i=1;i<P.nnodes;i++){
    for (OutArcIt a(P.g,Sol[i-1]);a!=INVALID;++a){
	    if(P.g.target(a)==Sol[i]){
        cost += P.weight[a];
        break;
      }
    }
  }

  return cost;
}

void CopyDNodeVector(DNodeVector &source, DNodeVector &target){
  target.resize(source.size());

  for(int i = 0; i < source.size(); i++){
    target[i] = source[i];
  }
}

/*      Heuristicas       */
                                                      
/*      RANDOM NEAREST NEIGHBOR adaptado PD        */
//Funcao que gera diferentes caminhos gulosos a cada chamada,
//pois escolhe um noh de partida pickup de forma randomica
//Retorno:
void GreedyHeuristic(Pickup_Delivery_Instance &P, double &UB, DNodeVector &Sol){
  DNode node = P.source;  // noh que ele comeca
  DNode next;             // noh mais proximo do atual
  double aux = MY_INF;    // custo infinito
  UB = 0.0;
  int randompos = 0;

  //inicializa o vetor solucao com o noh source em todas posicoes (dummy value para NULL)
  Sol.resize(P.nnodes);
  for(int k = 0; k < Sol.size(); k++){
    Sol[k] = node;
  }

  //insere o noh target no final do vetor
  Sol[Sol.size()-1] = P.target;

  //define um noh pickup aleatorio para comecar o vetor solucao na segunda posicao
  randompos = rand() % (P.npairs-1);

  //TODO: insere o par do pickup aleatorio na penultima pos do vetor solucao
  //Sol[Sol.size()-2] = delivery[randompos];

  //insere o noh pickup randomico na segunda posicao 
  Sol[1] = P.pickup[randompos];

  //visita todos os demais nos do grafo a partir do terceiro noh da solucao
  for(int i = 2; i < P.nnodes-1; i++){

    //visita todos os arcos de um noh menos o que leva para o target
    for (OutArcIt arcIt(P.g, node); arcIt != INVALID; ++arcIt) {

      //escolhe o proximo noh sendo o de menor custo excluindo o target e os nos ja contidos na solucao
      if((P.weight[arcIt] < aux) &&
         (P.g.target(arcIt) != P.target) &&
         (NotInSolution(P,Sol, P.g.target(arcIt)) &&
         (PickupDeliveryValid(P, P.g.target(arcIt), Sol)))){

          aux = P.weight[arcIt];
          next = P.g.target(arcIt);
      }
    }

    //adiciona o no de menor custo na solucao e seta ele como o proximo
    Sol[i] = next;
    node = next;

    aux = MY_INF;
  }

  //atualiza UB com a solucao encontrada
  UB = CostSol(P, Sol);

  return;
}

/*            2-OPT adaptado PD              */
//Funcao da Heuristica baseada em 2-OPT
//  1.Escolhe um noh da solucao por um numero randomico (excluindo source e target)
//  2.Checa se ele eh um pickup ou delivery
//    2.1.Caso pickup: troca de posicao com um delivery de posicao menor
//    2.2.Caso delivery: troca de posicao com um pickup de posicao maior
//  3. Se encontrar um custo menor que UB -> atualiza a solucao
//Retorno:
void BasedOn2OptHeuristic(Pickup_Delivery_Instance &P, int time_limit, double &UB, DNodeVector &Sol){
  int i = 0;
  double cost;


  for(int opt = 0; opt < swaps; opt++){
    //numero randomico de 1 até valor do penultimo noh no vetor solucao
    //ou seja, exclui o noh source e o target    
    i = (rand() % (Sol.size()-2)) + 1;

    //TODO: melhorar esse trecho
    if(CheckPickupDelivery(P, Sol[i]) == "pickup"){
      for(int k = 0; k < i; k++){
        if(CheckPickupDelivery(P, Sol[k]) == "delivery"){
          SwapNodes(Sol, i, k);
          cost = CostSol(P, Sol);
          if(cost < UB){
            UB = cost;
            //cout << UB << "pickup" << endl;
          }else{
            SwapNodes(Sol, i, k);
          }
          break;
        }
      }
    }else{
      for(int k = i+1; k < (Sol.size()-1); k--){
        if(CheckPickupDelivery(P, Sol[k]) == "pickup"){
          SwapNodes(Sol, i, k);
          cost = CostSol(P, Sol);
          if(cost < UB){
            UB = cost;
            //cout << UB << "delivery" << endl;
          }else{
            SwapNodes(Sol, i, k);
          }
          break;
        }
      }
    }
  }

  

  return;
}

//Funcao que chama as Heuristicas implementadas
bool Lab2(Pickup_Delivery_Instance &P,int time_limit,double &LB,double &UB,DNodeVector &Sol)
{
  DNodeVector solAux;
  double UBAux;
  std::vector<Arc> tree; //variavel p executar o algoritmo de kruskal do lemon e retornar o valor da MST

  //caso o grafo soh tenha source e target
  if(P.nnodes <= 2){
    return 0;
  }

  //considerar o tempo limite de execucao
  auto start = high_resolution_clock::now();
  auto check = high_resolution_clock::now();
  auto exectime = duration_cast<seconds>(check - start);
  auto limit = std::chrono::seconds(time_limit);
  
  //inicializa LB com o valor do custo da minimum spanning tree 
  LB = kruskal(P.g, P.weight, std::back_inserter(tree));

  while(exectime < limit){

    //chama uma heuristica gulosa para definir uma solucao valida inicial
    GreedyHeuristic(P, UBAux, solAux);
    //cout <<"Greedy UB: " <<UBAux << endl;

    //chama a heuristica baseada em 2-Opt para tentar otimizar a solucao enquanto houver tempo
    BasedOn2OptHeuristic(P, time_limit, UBAux, solAux);

    //checa se a solucao greedy recebeu melhoras
    if(UBAux < UB){
      CopyDNodeVector(solAux, Sol);
      UB = UBAux;
      cout << UB << endl;
    }

    //checa o tempo de execucao apos o laco
    check = high_resolution_clock::now();
    exectime = duration_cast<seconds>(check - start);
  }
  
  return(1);
}

/*****************************************/


int main(int argc, char *argv[]) 
{
  int maxtime;
  Digraph g;  // graph declaration
  string digraph_filename, source_node_name, target_node_name;
  DNodeStringMap vname(g);  // name of graph nodes
  DNodePosMap px(g),py(g);  // xy-coodinates for each node
  DNodeColorMap vcolor(g);// color of nodes
  ArcStringMap aname(g);  // name for graph arcs
  ArcColorMap ecolor(g); // color of edges
  ArcValueMap lpvar(g);    // used to obtain the contents of the LP variables
  ArcValueMap weight(g);   // edge weights
  vector <DNode> V;
  DNode sourcenode,targetnode;
  int seed=0;
  srand48(seed);

  // uncomment one of these lines to change default pdf reader, or insert new one
  //set_pdfreader("open");    // pdf reader for Mac OS X
  //set_pdfreader("xpdf");    // pdf reader for Linux
  //set_pdfreader("open -a Skim.app");
  set_pdfreader("evince");  // pdf reader for Linux
  
  // double cutoff;   // used to prune non promissing branches (of the B&B tree)
  if (argc!=3) {cout<<endl<<
      "Laboratorio de MC658: Rota com coleta e entrega de peso minimmo," << endl <<
      "the st-shortest path problem." << endl << endl <<
      "Usage: "<< argv[0]<<"  <pickup_delivery_digraph_filename> <maximum_time_sec>"<< endl << endl;
    cout << "Example:" << endl <<
      "\t" << argv[0] << " "<< getpath(argv[0])+"../instances/pickup_delivery_5.dig 10" << endl << endl <<
      "\t" << argv[0] << " "<< getpath(argv[0])+"../instances/pickup_delivery_10.dig 100" << endl << endl;
    exit(0);}

  digraph_filename = argv[1];
  maxtime = atoi(argv[2]);
  DNodeVector pickup,delivery;
  DNode source,target;
  int npairs;
  
  if (!ReadPickupDeliveryDigraph(digraph_filename,g,vname,px,py,weight,
				 source,target,npairs,pickup,delivery)){
    cout << "Erro na leitura do grafo de entrada." << endl;}
    
  Pickup_Delivery_Instance P(g,vname,px,py,weight,source,target,npairs,pickup,delivery);
  //PrintInstanceInfo(P);
  
  double LB = 0, UB = MY_INF; // considere MY_INF como infinito.
  DNodeVector Solucao;
  
  auto start = high_resolution_clock::now();
  bool melhorou = Lab2(P,maxtime,LB,UB,Solucao);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);

  //cout << "Time taken by function: "
  //     << duration.count() << " microseconds" << endl;

  cout << "UB found : " << UB << endl;
  cout << "LB found : " << LB << endl;
  //cout << UB << ">= " << "Optimum" << " >=" << LB << endl;

  double cost = 0.0;
  for (int i=1;i<P.nnodes;i++){
    for (OutArcIt a(P.g,Solucao[i-1]);a!=INVALID;++a){
	    if(P.g.target(a)==Solucao[i]){
        cost += P.weight[a];
        cout << P.vname[P.g.source(a)] << "->" << P.vname[P.g.target(a)] << " = " << P.weight[a] << endl;
        break;
      }
    }
  }
  cout << CostSol(P, Solucao) << endl;

  if (melhorou) {
    //ViewPickupDeliverySolution(P,LB,UB,Solucao,"Solucao do Lab.");
    //PrintSolution(P,Solucao,"Solucao do Lab2.");
  }
  return 0;
}

