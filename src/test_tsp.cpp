//----------------------------------------------------------------------
// Program to test if the solution obtained by the program ex_tsp_gurobi
// matches a solution cost available in
// http://elib.zib.de/pub/mp-testdata/tsp/tsplib/tsp/
// Differences may occur, because edge costs in the graphs available in the same directory
// do not have the same costs computed by TSPLIB (edge costs are a bit different)
//
// Send comments/corrections to Flavio K. Miyazawa.
//----------------------------------------------------------------------
#include <assert.h>
#include <gurobi_c++.h>
#include <float.h>
#include <math.h>
#include <set>
#include <lemon/list_graph.h>
#include <lemon/unionfind.h>
#include <lemon/gomory_hu.h>
#include <lemon/adaptors.h>
#include <lemon/connectivity.h>
#include "mygraphlib.h"
#include "myutils.h"

// This is the type used to obtain the pointer to the problem data. This pointer
// is stored in the branch and cut tree. And when we define separation routines,
// we can recover the pointer and access the problem data again.
class TSP_Data {
public:
  TSP_Data(Graph &graph,
	   NodeStringMap &nodename,
	   NodePosMap &posicaox,
	   NodePosMap &posy,
	   EdgeValueMap &eweight);
  Graph &g;
  int NNodes,NEdges;
  int max_perturb2opt_it; // maximum number of iterations for heuristic Perturb2OPT
  NodeStringMap &vname;
  EdgeStringMap ename;
  NodeColorMap vcolor;
  EdgeColorMap ecolor;
  EdgeValueMap &weight;
  NodePosMap &posx;
  NodePosMap &posy;
  AdjacencyMatrix AdjMat; // adjacency matrix
  vector<Node> BestCircuit; // vector containing the best circuit found
  double BestCircuitValue;
};

TSP_Data::TSP_Data(Graph &graph,
		   NodeStringMap &nodename,
		   NodePosMap &posicaox,
		   NodePosMap &posicaoy,
		   EdgeValueMap &eweight):
  g(graph),
  vname(nodename),
  ename(graph),
  vcolor(graph),
  ecolor(graph),
  weight(eweight),
  posx(posicaox),
  posy(posicaoy),
  AdjMat(graph,eweight,MY_INF), //
  BestCircuit(countEdges(graph)) 
{
  NNodes=countNodes(this->g);
  NEdges=countEdges(this->g);
  BestCircuitValue = DBL_MAX;
  max_perturb2opt_it = 3000; // default value
}



int main(int argc, char *argv[]) 
{
  int time_limit;
  Graph g;
  EdgeValueMap weight(g);
  NodeStringMap vname(g);
  NodePosMap   posx(g),posy(g);
  string filename,circuitfilename;

  int seed=1;


  // uncomment one of these lines to change default pdf reader, or insert new one
  //set_pdfreader("open");    // pdf reader for Mac OS X
  //set_pdfreader("xpdf");    // pdf reader for Linux
  //set_pdfreader("evince");  // pdf reader for Linux

  srand48(seed);
  time_limit = 3600; // solution must be obtained within time_limit seconds
  if (argc!=3) {
    cout << endl<<
      "This program only presents the cost of a given circuit in the format given by TSPLIB."<<endl;
    cout<< endl << "Usage: "<< argv[0]<<" <graph_filename> <tsplib_tour_format>"<<endl << endl <<
      "Example:" << endl <<
      "\t" << argv[0] << " ./instances/k_a280.gr ./instances/a280.opt.tour" << endl <<endl;
 exit(0);}
  
  else if (!FileExists(argv[1])) {cout<<"File "<<argv[1]<<" does not exist."<<endl; exit(0);}
  filename = argv[1];
  circuitfilename = argv[2];
  
  // Read the graph
  if (!ReadGraph(filename,g,vname,posx,posy,weight)) 
    {cout<<"Error reading graph file "<<argv[1]<<"."<<endl;exit(0);}

  TSP_Data tsp(g,vname,posx,posy,weight); 

  {
      ifstream in;
      string linha,substring,name;
      int nnodes=0,t;
      in.open(circuitfilename);
      if (!in.is_open()) {
	cout << "ERROR: Problem to open file " << circuitfilename << "\n";
	return(false);
      }
      while (!in.eof()) {
	getline(in,linha);
	// cout << "getline: " << linha << endl;
	t = linha.find("DIMENSION");
	if (t==string::npos) continue;
	else {
	  t = linha.find(":");
	  substring = linha.substr(t+1);
	  sscanf(substring.c_str(),"%d",&nnodes);
	  break;
	}
      }
      cout << "Number of nodes: " << nnodes << endl;
      if (nnodes>100000) {cout<<"Number of vertices is too large (>100000)\n";exit(0);};
      Node seq[nnodes];
      double soma=0.0;
      while (!in.eof()) {
	getline(in,linha);
	// cout << "getline: " << linha << endl;
	t = linha.find("TOUR_SECTION");
	if (t==string::npos) continue;
	else break;
      }
      for (int i=0;i<nnodes;i++) {
	getline(in,linha);
	seq[i] = GetNodeByName(g,vname,linha);
      }
      for (int i=0;i<nnodes;i++) {
	Edge e;
	e = findEdge(g,seq[i],seq[(i+1)%nnodes]);
	soma += weight[e];
      }
      cout << "Cost of the circuit in file \"" << circuitfilename << "\" is " << soma << endl;
      in.close();
    }
}
