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
  // cout << "Literal count: " << literal_count << endl;
  // cout << "Clause count: " << clause_count << endl;

  int literal;
  for (int i = 0; i < clause_count; i++)
  {
    while (true)
    {
      cin >> literal; // Variable number

      if (literal > 0)
      {
        formula.clause[i].push_back(2 * (literal - 1));
        formula.literal_num[literal - 1]++;
        formula.literal_sign[literal - 1]++;
      }
      else if (literal < 0)
      {
        formula.clause[i].push_back(2 * ((-1) * literal - 1) + 1);
        formula.literal_num[-1 - literal]++;
        formula.literal_sign[-1 - literal]--;
      }
      else
      {
        break;
      }
    }
    // Test to print caluses

    // cout << "formula.clause[" << i << "]: ";
    //   for (auto value : formula.clause[i])
    //   {
    //       cout << value << " ";
    //   }
    //   cout << endl;
  }

  int main()
  {
    Solver solver;
    solver.initialize();
    // solver.solve();
    return 0;
  }