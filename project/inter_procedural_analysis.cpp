#include "inter_procedural_analysis.h"

std::unordered_set<CallSite<M, N, A>> getCallers(Context<M,N,A> target);
Context<M, N, A> getContext(M method, A value);
std::vector<Context<M, N, A>> getContexts(M method);
ContextTransitionTable<M, N, A> getContextTransitionTable();
std::unordered_set<M> getMethods();
std::unordered_map<M, Context<M, N, A>> getTargets(CallSite<M, N, A> callSite);
