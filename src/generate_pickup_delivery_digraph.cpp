#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "mygraphlib.h"
using namespace std;
int main(int argc, char *argv[]) 
{
  int n,k,i;
  srand48(clock());
  if (argc!=2) {cout<<"Usage: "<< argv[0]<<" <number_of_pairs>"<<endl; exit(0);} 
  k = atoi(argv[1]);
  n = 2*k+2;
  Digraph g;
  DNodeStringMap vname(g);
  DNodeIntMap pair(g);
  DNodePosMap px(g);
  DNodePosMap py(g);
  ArcValueMap weight(g);
  DNode R[n];
  GenerateRandomEuclideanDigraph(g,vname,px,py,weight,n,1000,1000);
  cout << "nnodes narcs type" << endl;
  cout << n << " " << n*(n-1) << " " << " digraph " << endl;
  cout << "nodename posx posy" << endl;
  i=0;
  for (DNodeIt v(g);v!=INVALID;++v) {
    cout<<vname[v]<<" "<<px[v]<<" "<<py[v]<<endl;
    // R[i] = v;
    // i++;
  }
  cout << "tail head weight" << endl;
  for (ArcIt e(g);e!=INVALID;++e)
    cout<<vname[g.source(e)]<<" "<<vname[g.target(e)]<<" "<<weight[e]<<endl;
  // cout << "source target" << endl;
  // cout << vname[R[n-1]] << " " << vname[R[0]] << endl;
  // cout << "pick delivery" << endl;
  // for (int i=0;i<k;i++)
  //   cout << vname[R[2*i+2]] << " " << vname[R[2*i+1]] << endl;
}
