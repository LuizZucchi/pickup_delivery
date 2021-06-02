// =============================================================
//
//  Some useful general routines used with types given by solvers
//
//  Send comments/corrections to Flavio K. Miyazawa.
//
// =============================================================
#ifndef SOLVER_DEFINE
#define SOLVER_DEFINE
#include "mygraphlib.h"
#include "myutils.h"
#include <gurobi_c++.h>

typedef ListGraph::NodeMap<GRBVar> NodeSolverMap;
typedef ListGraph::EdgeMap<GRBVar> EdgeSolverMap;
typedef Digraph::NodeMap<GRBVar> DNodeSolverMap;
typedef Digraph::ArcMap<GRBVar> ArcSolverMap;

// Get solution from the solver variable 
inline void GetSolverValue(ListGraph &g, NodeValueMap &vec, NodeSolverMap &x)
{for (NodeIt v(g); v!=INVALID; ++v) vec[v] = x[v].get(GRB_DoubleAttr_X);}
inline void GetSolverValue(ListGraph &g, EdgeValueMap &vec, EdgeSolverMap &x)
{for (EdgeIt e(g); e!=INVALID; ++e) vec[e] = x[e].get(GRB_DoubleAttr_X);}
inline void GetSolverValue(Digraph &g, DNodeValueMap &vec, DNodeSolverMap &x)
{for (DNodeIt v(g); v!=INVALID; ++v) vec[v] = x[v].get(GRB_DoubleAttr_X);}
inline void GetSolverValue(Digraph &g, ArcValueMap &vec, ArcSolverMap &x)
{for (ArcIt e(g); e!=INVALID; ++e) vec[e] = x[e].get(GRB_DoubleAttr_X);}

// Values converted to int
inline void GetSolverValue(ListGraph &g, NodeIntMap &vec, NodeSolverMap &x)
{for (NodeIt v(g); v!=INVALID; ++v) vec[v] = (int) (x[v].get(GRB_DoubleAttr_X)+MY_EPS);}
inline void GetSolverValue(ListGraph &g, EdgeIntMap &vec, EdgeSolverMap &x)
{for (EdgeIt e(g); e!=INVALID; ++e) vec[e] = (int) (x[e].get(GRB_DoubleAttr_X)+MY_EPS);}
inline void GetSolverValue(Digraph &g, DNodeIntMap &vec, DNodeSolverMap &x)
{for (DNodeIt v(g); v!=INVALID; ++v) vec[v] = (int) (x[v].get(GRB_DoubleAttr_X)+MY_EPS);}
inline void GetSolverValue(Digraph &g, ArcIntMap &vec, ArcSolverMap &x)
{for (ArcIt e(g); e!=INVALID; ++e) vec[e] = (int) (x[e].get(GRB_DoubleAttr_X)+MY_EPS);}

// Values converted to string
inline void GetSolverValue(ListGraph &g, NodeStringMap &vec, NodeSolverMap &x)
{for (NodeIt v(g);v!=INVALID;++v) vec[v] = DoubleToString(x[v].get(GRB_DoubleAttr_X));}
inline void GetSolverValue(ListGraph &g, EdgeStringMap &vec, EdgeSolverMap &x)
{for (EdgeIt e(g);e!=INVALID;++e) vec[e] = DoubleToString(x[e].get(GRB_DoubleAttr_X));}
inline void GetSolverValue(Digraph &g, DNodeStringMap &vec, DNodeSolverMap &x)
{for (DNodeIt v(g);v!=INVALID;++v) vec[v] = DoubleToString(x[v].get(GRB_DoubleAttr_X));}
inline void GetSolverValue(Digraph &g, ArcStringMap &vec, ArcSolverMap &x)
{for (ArcIt e(g);e!=INVALID;++e) vec[e] = DoubleToString(x[e].get(GRB_DoubleAttr_X));}



inline double GetModelValue(GRBModel &model) {return(model.get(GRB_DoubleAttr_ObjVal));}



// FKM: Mudar o codigo para usar template e juntar as rotinas.

inline void SetColorByValue(GraphAttributes &G, double value, NodeSolverMap &x, string cor){
  for (NodeIt v(G.g);v!=INVALID;++v) if(IsEqual(value,x[v].get(GRB_DoubleAttr_X))) G.SetColor(v,cor);}

inline void SetColorByInterval(GraphAttributes &G, double lb,double ub,NodeSolverMap &x, string cor){
  for (NodeIt v(G.g);v!=INVALID;++v)
    if((x[v].get(GRB_DoubleAttr_X)>=lb) && (x[v].get(GRB_DoubleAttr_X)<=ub)) G.SetColor(v,cor);}

inline void SetColorByValue(GraphAttributes &G, double value,
					  EdgeSolverMap &x, string cor){
  for (EdgeIt e(G.g);e!=INVALID;++e) if(IsEqual(value,x[e].get(GRB_DoubleAttr_X))) G.SetColor(e,cor);}

inline void SetColorByInterval(GraphAttributes &G, double lb,double ub,
					  EdgeSolverMap &x, string cor){
  for (EdgeIt e(G.g);e!=INVALID;++e)
    if((x[e].get(GRB_DoubleAttr_X)>lb-MY_EPS) && (x[e].get(GRB_DoubleAttr_X)<ub+MY_EPS)) G.SetColor(e,cor);}

/// --- 
inline void SetColorByValue(DigraphAttributes &G, double value, DNodeSolverMap &x, string cor){
  for (DNodeIt v(G.g);v!=INVALID;++v) if(IsEqual(value,x[v].get(GRB_DoubleAttr_X))) G.SetColor(v,cor);}

inline void SetColorByInterval(DigraphAttributes &G, double lb,double ub,DNodeSolverMap &x, string cor){
  for (DNodeIt v(G.g);v!=INVALID;++v)
    if((x[v].get(GRB_DoubleAttr_X)>lb-MY_EPS) && (x[v].get(GRB_DoubleAttr_X)<ub+MY_EPS)) G.SetColor(v,cor);}

inline void SetColorByValue(DigraphAttributes &G, double value,
					  ArcSolverMap &x, string cor){
  for (ArcIt e(G.g);e!=INVALID;++e) if(IsEqual(value,x[e].get(GRB_DoubleAttr_X))) G.SetColor(e,cor);}

inline void SetColorByInterval(DigraphAttributes &G, double lb,double ub,
					  ArcSolverMap &x, string cor){
  for (ArcIt e(G.g);e!=INVALID;++e)
    if((x[e].get(GRB_DoubleAttr_X)>lb-MY_EPS) && (x[e].get(GRB_DoubleAttr_X)<ub+MY_EPS)) G.SetColor(e,cor);}


inline void SetLabelNonZero(GraphAttributes &G, EdgeSolverMap &x)
{ for (EdgeIt e(G.g);e!=INVALID;++e) if(!IsEqual(0,x[e].get(GRB_DoubleAttr_X))) G.SetLabel(e,x[e].get(GRB_DoubleAttr_X));}
inline void SetLabelNonZero(GraphAttributes &G, NodeSolverMap &x)
{ for (NodeIt v(G.g);v!=INVALID;++v) if(!IsEqual(0,x[v].get(GRB_DoubleAttr_X))) G.SetLabel(v,x[v].get(GRB_DoubleAttr_X));}
inline void SetLabelNonZero(DigraphAttributes &G, ArcSolverMap &x)
{ for (ArcIt a(G.g);a!=INVALID;++a) if(!IsEqual(0,x[a].get(GRB_DoubleAttr_X))) G.SetLabel(a,x[a].get(GRB_DoubleAttr_X));}
inline void SetLabelNonZero(DigraphAttributes &G, DNodeSolverMap &x)
{ for (DNodeIt v(G.g);v!=INVALID;++v) if(!IsEqual(0,x[v].get(GRB_DoubleAttr_X))) G.SetLabel(v,x[v].get(GRB_DoubleAttr_X));}

#endif
