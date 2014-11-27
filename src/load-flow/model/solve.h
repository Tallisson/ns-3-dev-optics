#ifndef SOLVE_H_
#define SOLVE_H_

#include <armadillo>

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

using namespace arma;

namespace ns3
{
class Solve : public Object
{
private:
  mat* data;
  uint32_t rows;
  uint32_t cols;
public:
  Solve();
  virtual ~Solve();

  void FillR();
  void SetValue(uint32_t col, uint32_t row, double value);
  void Zeros();
  void Clear();
  void Print();

  double GetValue(uint32_t col, uint32_t row);

  mat Product(vec error);
  mat Inverse();

  static TypeId GetTypeId (void);

  uint32_t GetCols() const;
  void SetCols(uint32_t cols);
  uint32_t GetRows() const;
  void SetRows(uint32_t rows);

  void Initialize(uint32_t cols, uint32_t rows);
};
}

#endif /* SOLVE_H_ */
