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
#include <../lemon/lemon-1.3.1/lemon/list_graph.h>
#include "../include/mygraphlib.h"
#include <string>
#include "myutils.h"
#include <lemon/concepts/digraph.h>
#include <lemon/preflow.h>

#include "lab1.h"

#define INF DBL_MAX

using namespace lemon;
using namespace std;

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
  for (int i=0;i<P.nnodes;i++)
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
bool HeuristicaConstrutivaBoba(Pickup_Delivery_Instance &P,int time_limit,
			       double &LB,double &UB,DNodeVector &Sol)
{
  cout << "Execucao da Heuristica Boba" << endl;
  cout << "\tEsta rotina deveria respeitar o tempo de no maximo "
       << time_limit << " segundos" << endl;
  cout<<"\tProvavelmente a solucao eh inviavel, pois mistura as coletas e entregas."<< endl;
  if (UB==MY_INF) { // Faz alguma coisa so' se ainda nao tem solucao
    Sol.resize(P.nnodes);
    Sol[0] = P.source; // insere o source
    for (int i = 0; i < P.npairs; i++) Sol[i+1] = P.pickup[i]; // insere os pickup
    for (int i = 0; i < P.npairs; i++) Sol[P.npairs+i+1] = P.delivery[i]; // insere os delivery
    Sol[2*P.npairs+1] = P.target; // insere o target.

    // Ordena os vertices, exceto pelo source e target, do mais baixo para mais alto
    int primeiro=1, ultimo=2*P.npairs;
    for (int i = primeiro; i < ultimo; i++) {
      for (int j = i+1; j <= ultimo; j++) {
        if (P.py[Sol[i]] > P.py[Sol[j]]){
          DNode aux;
          aux=Sol[i];
          Sol[i]=Sol[j];
          Sol[j]=aux;
        }

      }
    }
      
	
    // Atualiza o UB (Upper Bound) que eh o valor da solucao
    UB = 0.0;
    for (int i=1; i<P.nnodes; i++) {
      for (OutArcIt a(P.g,Sol[i-1]); a!=INVALID; ++a) {
        if(P.g.target(a) == Sol[i]) {
          UB += P.weight[a];
          break;
        }
      }
    }
  }
  cout << "cost: " << UB << endl;
  cout << endl;
  return(1);
}

bool ViewPickupDeliverySolution(Pickup_Delivery_Instance &P,double &LB,double &UB,DNodeVector &Sol,string msg)
{
  DigraphAttributes GA(P.g,P.vname,P.px,P.py);
  GA.SetDefaultDNodeAttrib("color=LightGray style=filled width=0.2 height=0.2 fixedsize=true");
  for (ArcIt a(P.g); a!=INVALID; ++a) GA.SetColor(a,"Invis");
  GA.SetColor(P.source,"Red"); // source and target are painted in White
  GA.SetColor(P.target,"Red");
  GA.SetAttrib(P.source,"shape=box");
  GA.SetAttrib(P.target,"shape=box");

  if (P.npairs <= 16){ // se tiver poucos pares, dah para pintar os pares de mesma cor.
    for (int i=0;i<P.npairs;i++){ // pinta 
      GA.SetColor(P.pickup[i],ith_VisualDistinctColorName(i));
      GA.SetColor(P.delivery[i],ith_VisualDistinctColorName(i));
    }
  }

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
/*implementacao da heuristica/metaheuristica utilizada*/
/*                                                    */

bool NotInSolution(Pickup_Delivery_Instance &P,DNodeVector &Sol, DNode node){
  bool notin = true;
  
  for(int i = 0; i < Sol.size(); i++){
    if(P.vname[Sol[i]] == P.vname[node]){
      notin = false;
    }
  }

  return notin;
}

int findDeliveryIndex(Pickup_Delivery_Instance &P, DNode node, string type){
  int i = 0;

  if(type == "delivery"){
    for(i = 0; i < P.npairs; i++){
      if(P.vname[node] == P.vname[P.delivery[i]]){
        break;
      }
    }
  }

  return i;
}

bool PickupDeliveryValid(Pickup_Delivery_Instance &P, DNode node, DNodeVector &Sol){
  int idx = 0;
  bool isvalid;

  if((stoi(P.vname[node]) % 2) == 0 ){
    idx = findDeliveryIndex(P, node, "delivery");

    isvalid = !NotInSolution(P,Sol, P.pickup[idx]);
  }else{

    isvalid = true;
  }

  return isvalid;
}

bool Lab2(Pickup_Delivery_Instance &P,int time_limit,double &LB,double &UB,DNodeVector &Sol)
{
  DNode node = P.source;  // noh que ele comeca
  DNode next;             // noh mais proximo do atual
  double aux = MY_INF;    // custo infinito

  //inicializa o vetor solucao com o noh source em todas posicoes (dummy value para NULL)
  Sol.resize(P.nnodes);
  for(int k = 0; k < Sol.size(); k++){
    Sol[k] = node;
  }

  //visita todos os demais nos do grafo a partir do segundo noh da solucao
  for(int i = 1; i < P.nnodes-1 ; i++){

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

  //adiciona o target na solucao
  Sol[Sol.size()-1] = P.target; 

  // Atualiza o UB (Upper Bound) que eh o valor da solucao
  UB = 0.0;
  for (int i=1;i<P.nnodes;i++)
    for (OutArcIt a(P.g,Sol[i-1]);a!=INVALID;++a) {
      if(P.g.target(a)==Sol[i]){
        // cout << "Sol[i]: " << P.vname[P.g.source(a)] << "->" << P.vname[Sol[i]] << " weight: " << P.weight[a] << endl;
        UB += P.weight[a];
        break;
      }
    }
	    
  cout << UB << endl;
  return(1);
}

/***********************************************************/



bool Lab1(Pickup_Delivery_Instance &P,int time_limit,double &LB,double &UB,DNodeVector &Sol) {
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;
  vector<vector<double>> adj_matrix(P.nnodes, vector<double>(P.nnodes));

  int x, y;
  for (ArcIt e(P.g); e != INVALID; ++e) {
    x = (stoi(P.vname[P.g.source(e)]) - 1);
    y = (stoi(P.vname[P.g.target(e)]) - 1);
    adj_matrix[x][y] = P.weight[e];
    
    // cout<<P.vname[P.g.source(e)]<<"\t"<<P.vname[P.g.target(e)]<<"\t"<<P.weight[e]<<"\t"<<adj_matrix[x][y]<<endl;
  }  

  for (int i = 0; i < P.nnodes; ++i) {
    adj_matrix[i][i] = INF;
  }

  int source = stoi(P.vname[P.source]);
  int target = stoi(P.vname[P.target]);
  vector<int> pickup(P.npairs);
  vector<int> delivery(P.npairs);
  vector<int> solution(P.nnodes);

  for (int i = 0; i < P.npairs; i++) {
    pickup[i] = stoi(P.vname[P.pickup[i]]) - 1;
    delivery[i] = stoi(P.vname[P.delivery[i]]) - 1;
  }

  auto t1 = high_resolution_clock::now();
  double my_UB = UB;
  double cost = solve(adj_matrix, source, target, pickup, delivery, solution, my_UB, my_UB, time_limit);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  cout << "cost: " << cost << " time: " << ms_double.count()/1000 << endl;
  Sol.resize(solution.size());
  my_UB = 0.0;
  for (int i = 1; i < P.nnodes; i++) {
    for (OutArcIt e(P.g, Sol[i-1]); e != INVALID; ++e) {
      if (P.vname[P.g.target(e)] == to_string(solution[i] + 1) && P.vname[P.g.source(e)] != to_string(solution[i] + 1)) {
        Sol[i] = P.g.target(e);
        my_UB += P.weight[e];
        // cout << "Sol[i]: " << P.vname[P.g.source(e)] << "->" << P.vname[Sol[i]] << " weight: " << P.weight[e] << endl;
        break;
      }
    }
  }
  cout << cost << endl;
  cout << my_UB << endl;
  UB = cost;
  // Apague a chamada abaixo e escreva a codificacao da sua rotina relativa ao Laboratorio 1.
  // return(HeuristicaConstrutivaBoba(P,time_limit,LB,UB,Sol));
  return 1;
}





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
  set_pdfreader("open");    // pdf reader for Mac OS X
  //set_pdfreader("xpdf");    // pdf reader for Linux
  //set_pdfreader("evince");  // pdf reader for Linux
  //set_pdfreader("open -a Skim.app");
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
  PrintInstanceInfo(P);
  
  double LB = 0, UB = MY_INF; // considere MY_INF como infinito.
  DNodeVector Solucao;
  
  // heuristica gulosa para definir um UB inicial
  Lab2(P, maxtime, LB, UB, Solucao);

  bool melhorou = Lab1(P,maxtime,LB,UB,Solucao);

  if (melhorou) {
    ViewPickupDeliverySolution(P,LB,UB,Solucao,"Solucao do Lab.");
    PrintSolution(P,Solucao,"Solucao do Lab1.");
  }
  return 0;
}

