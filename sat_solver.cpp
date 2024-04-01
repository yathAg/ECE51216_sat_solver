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
  void result(Formula &, int);

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

  // Test Print literal count and clause count

  cout << "Literal count: " << literal_count << endl;
  cout << "Clause count: " << clause_count << endl;

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
  if (f.clause.size() == 0)
  {
    return State::sat;
  }
  do
  {
    unit_clause = false;
    for (int i = 0; i < f.clause.size(); i++)
    {
      if (f.clause[i].size() == 1)
      {
        unit_clause = false;
        int result;
        if (f.clause[i][0] > 0)
        {
          f.literals[(f.clause[i][0]) - 1] = 1;
          f.literal_num[f.clause[i][0] - 1] = -1;
          result = transform(f, f.clause[i][0]);
        }
        else
        {
          f.literals[-1 * (f.clause[i][0]) - 1] = 0;
          f.literal_num[-1 * f.clause[i][0] - 1] = -1;
          result = transform(f, f.clause[i][0]);
        }

        if (result == State::sat || result == State::unsat)
        {
          return result;
        }
        break;
      }
      else if (f.clause[i].size() == 0)
      {
        return State::unsat;
      }
    }
  } while (unit_clause);

  return State::unresolved;
}

int Solver::transform(Formula &f, int literal)
{

  for (int i = 0; i < f.clause.size(); i++)
  {
    for (int j = 0; j < f.clause[i].size(); j++)
    {
      if ((f.clause[i][j] == literal))
      {
        f.clause.erase(f.clause.begin() + i);
        i--;
        if (f.clause.size() == 0)
        {
          return State::sat;
        }
        break;
      }
      else if (-1 * f.clause[i][j] == literal)
      {
        f.clause[i].erase(f.clause[i].begin() + j);
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

void Solver::solve()
{
  int result = BPC(formula);
  Solver::result(formula, result);
}

void Solver::result(Formula &f, int result)
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
}

int main()
{
  Solver solver;
  solver.initialize();
  solver.solve();
  return 0;
}