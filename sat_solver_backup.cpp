#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum State
{
  sat,
  unsat,
  unresolved,
  done
};

class Formula
{

public:
  vector<int> literals;
  vector<int> literal_num;
  vector<int> literal_sign;
  vector<vector<int>> clause;

  Formula() {}
  Formula(const Formula &f)
  {
    literals = f.literals;
    literal_num = f.literal_num;
    literal_sign = f.literal_sign;
    clause = f.clause;
  }
};

class Solver
{
private:
  Formula formula;
  int literal_count;
  int clause_count;
  int BPC(Formula &);
  int dpll(Formula);
  int transform(Formula &, int);
  void show_result(Formula &, int);

public:
  Solver() {}
  void initialize();
  void solve();
};

void Solver::initialize()
{
  char c;
  string s;

  while (true)
  {
    cin >> c;
    if (c == 'c')
    {
      getline(cin, s);
    }
    else
    {
      cin >> s;
      break;
    }
  }

  cin >> literal_count;
  cin >> clause_count;

  formula.literals.clear();
  formula.literals.resize(literal_count, -1);
  formula.clause.clear();
  formula.clause.resize(clause_count);
  formula.literal_num.clear();
  formula.literal_num.resize(literal_count, 0);
  formula.literal_sign.clear();
  formula.literal_sign.resize(literal_count, 0);

  int literal;
  for (int i = 0; i < clause_count; i++)
  {
    while (true)
    {
      cin >> literal; // Variable number

      if (literal > 0)
      {
        formula.clause[i].push_back(literal);
        formula.literal_num[literal - 1]++;
        formula.literal_sign[literal - 1]++;
      }
      else if (literal < 0)
      {
        formula.clause[i].push_back(literal);
        formula.literal_num[-1 - literal]++;
        formula.literal_sign[-1 - literal]--;
      }
      else
      {
        break;
      }
    }
  }
}

int Solver::BPC(Formula &f)
{
  bool unit_clause = false;

  // If there are no clauses it is satisfiable by default 
  if (f.clause.size() == 0)
  {
    return State::sat;
  }

  // Check and remove all unit clauses
  do
  {
    unit_clause = false;
    for (int i = 0; i < f.clause.size(); i++)
    {
      if (f.clause[i].size() == 1)   // Size of Unit clause will be 1
      {
        unit_clause = false;
        int result;
        if (f.clause[i][0] > 0)     // if literal is positive
        {
          f.literals[(f.clause[i][0]) - 1] = 1;
          f.literal_num[f.clause[i][0] - 1] = -1;
        }
        else                       // if literal is negative 
        {
          f.literals[-1 * (f.clause[i][0]) - 1] = 0;
          f.literal_num[-1 * f.clause[i][0] - 1] = -1;
        }
        
        result = transform(f, f.clause[i][0]);  // remove literal from all clauses 
        
        if (result == State::sat || result == State::unsat)
        {
          return result;
        }
        break;
      }
      else if (f.clause[i].size() == 0)   // if there is an empty clause it is unsatisfiable  
      {
        return State::unsat;
      }
    }
  } while (unit_clause);

  return State::unresolved;   // function is not only comprised of unit clauses have to do more 
}

int Solver::transform(Formula &f, int literal)
{
  for (int i = 0; i < f.clause.size(); i++)
  {
    for (int j = 0; j < f.clause[i].size(); j++)
    {
      if ((f.clause[i][j] == literal))    // if the literal in some arbitrary clause we are checking is same as the literal  
      {
        f.clause.erase(f.clause.begin() + i); // remove the clause ( or operation)
        i--;
        if (f.clause.size() == 0)  // if number of clauses becomes it is satisfied
        {
          return State::sat;
        }
        break;
      }
      else if (-1 * f.clause[i][j] == literal)  // if literal is present in complement form 
      {
        f.clause[i].erase(f.clause[i].begin() + j); // only remove the literal as is not going to help is satisfiability 
        j--;
        if (f.clause[i].size() == 0)
        {
          return State::unsat;
        }
        break;
      }
    }
  }
  return State::unresolved;
}

int Solver::dpll(Formula f)
{
    int result = BPC(f);  // first handle all unit clauses 
    if (result == State::sat)
    {
        show_result(f, result);
        return State::done;
    }
    else if (result == State::unsat)  // we are not sure it is satisfied, mark unresolved
    {
        return State::unresolved;
    }
    int i = distance(               // get position of literal with max position 
        f.literal_num.begin(),
        max_element(f.literal_num.begin(), f.literal_num.end()));
    
    // cout << "maximum distance is " << i << endl;
    for (int j = 0; j < 2; j++)
    {
        int literal;
        Formula new_f = f;
        if (new_f.literal_sign[i] > 0)
        {
            new_f.literals[i] = 1;
            literal = (i+1);
        }
        else
        {
            new_f.literals[i] = 0;
            literal = (i+1)*-1;

        }
        new_f.literal_num[i] = -1;
        int transform_result = transform(new_f, literal);
        if (transform_result == State::sat)
        {
            show_result(new_f, transform_result);
            return State::done;
        }
        else if (transform_result == State::unsat)
        {
            continue;
        }
        int dpll_result = dpll(new_f);
        if (dpll_result == State::done)
        {
            return dpll_result;
        }
    }
    return State::unresolved;
}

void Solver::show_result(Formula &f, int result)
{
  cout << "Result: ";
  switch (result)
  {
  case State::sat:
    cout << "SATISFIABLE" << endl;
    break;
  case State::unsat:
    cout << "UNSATISFIABLE" << endl;
    break;
  case State::unresolved:
    cout << "UNRESOLVED" << endl;
    break;
  default:
    break;
  }

  cout << "Literals: ";
  for (int i = 0; i < f.literals.size(); ++i)
  {
    cout << f.literals[i] << " ";
  }
  cout << endl;

  cout << "Literal Num: ";
  for (int i = 0; i < f.literal_num.size(); ++i)
  {
    cout << f.literal_num[i] << " ";
  }
  cout << endl;

  cout << "Literal Sign: ";
  for (int i = 0; i < f.literal_sign.size(); ++i)
  {
    cout << f.literal_sign[i] << " ";
  }
  cout << endl;

  cout << "Clause: ";
  for (int i = 0; i < f.clause.size(); ++i)
  {
    cout << "[ ";
    for (int j = 0; j < f.clause[i].size(); ++j)
    {
      cout << f.clause[i][j] << " ";
    }
    cout << "] ";
  }
  cout << endl;

  cout << "result: ";
  for (int i = 0; i < f.literals.size(); ++i)
  {
    if (f.literals[i] == 0){
      cout << (-1*(i+1)) << " ";
    }
    else if (f.literals[i] == 1){
      cout << ((i+1)) << " ";
    }
  }
  cout << endl;


}

void Solver::solve()
{
  int result = dpll(formula);
  if (result == State::unresolved)
    {
        show_result(formula, State::unsat);
    }
}

int main()
{
  Solver solver;
  solver.initialize();
  solver.solve();
  return 0;
}
