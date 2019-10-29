#ifndef _CONTEXT_H_
#define _CONTEXT_H_
#include <vector>
#include <functional>
#include <iterator>
#include <memory>
#include <iostream>

template<class M, class N, class A>
class Context {
  private:
    static int count;
    int id;
    M method;
    A entry_value;
    A exit_value;
    N node;
    bool analysed;

  public:
    bool is_null;
    Context(); // Just for the pair CallSite
    Context(M method);
    Context(M method, N node);
    bool operator==(const Context& c) const;

    int getId(void) const;
    M getMethod(void);
    N getNode(void);
    A getEntryValue(void);
    A getExitValue(void);
    A getValueBefore(N node);
    A getValueAfter(N node);
    bool isAnalysed(void);
    void markAnalysed(void);
    void unmarkAnalysed(void);
    void setEntryValue(A _entry_value);
    void setExitValue(A _exit_value);
};

namespace std {
	template<class M, class N, class A>
	struct hash<Context<M, N, A>> {
		size_t operator()(const Context<M, N, A>& c) const {
			return hash<int>()(c.getId());
		}
	};
}

#endif
