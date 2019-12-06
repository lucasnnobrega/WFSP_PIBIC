#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

class MyCallbackI : public IloCplex::SimplexCallbackI
{
    void main();
    IloCplex::CallbackI *duplicateCallback() const
    {
        return (new (getEnv()) MyCallbackI(*this));
    }
};

IloCplex::Callback MyCallback(IloEnv env)
{
    return (IloCplex::Callback(new (env) MyCallbackI()));
}

void MyCallbackI::main()
{
    cout << "Iteration " << getNiterations() << ": ";
    if (isFeasible())
    {
        cout << "Objective = " << getObjValue() << endl;
    }
    else
    {
        cout << "Infeasibility measure = " << getInfeasibility() << endl;
    }
}