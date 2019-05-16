#!/usr/bin/python
import sys
import cplex
from cplex.exceptions import CplexError

def readInstance(filePath):
    f = file(filePath, "r")

    n = int(f.readline())
    c = int(f.readline())

    pesos = []
    for i in range(n):
       p = int(f.readline())
       pesos.append(p)

    f.close()
    return n,c,pesos

def createProblem(n, c, pesos):

    prob = cplex.Cplex()
    prob.objective.set_sense(prob.objective.sense.minimize)

    for k in range(n):
       prob.variables.add(obj=[1], lb=[0], ub=[1], types="I", names=["y_" + str(k+1)])

    for j in range(n):
       for k in range(n):
          prob.variables.add(obj=[0], lb=[0], ub=[1], types="I", names=["x_" + str(j+1) + "_" + str(k+1)])

    for j in range(n):
       var_list = []
       coeff_list = []
       for k in range(n):
          var_list.append("x_" + str(j+1) + "_" + str(k+1))
          coeff_list.append(1)
       prob.linear_constraints.add(lin_expr=[[var_list, coeff_list]], senses="E", rhs=[1])

    for j in range(n):
       for k in range(n):
          var_list = ["x_" + str(j+1) + "_" + str(k+1), "y_" + str(k+1)]
          coeff_list = [1, -1]
          prob.linear_constraints.add(lin_expr=[[var_list, coeff_list]], senses="L", rhs=[0])

    for k in range(n):
       var_list = []
       coeff_list = []
       for j in range(n):
          var_list.append("x_" + str(j+1) + "_" + str(k+1))
          coeff_list.append(pesos[j])
       prob.linear_constraints.add(lin_expr=[[var_list, coeff_list]], senses="L", rhs=[c])

    return prob


def main():

    try:
        n,c,pesos = readInstance(sys.argv[1])
        prob = createProblem(n,c,pesos)
        prob.solve()
    except CplexError as exc:
        print(exc)
        return

    # solution.get_status() returns an integer code
    print "Solution status = ", prob.solution.get_status(), ":",
    # the following line prints the corresponding string
    print prob.solution.status[prob.solution.get_status()]
    print "Solution value  = ", prob.solution.get_objective_value()

if __name__ == "__main__":
   main()
