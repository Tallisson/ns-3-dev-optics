/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef SOLVE_H_
#define SOLVE_H_

#include <armadillo>

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

using namespace arma;
using namespace std;

namespace ns3
{

#define TYPE_DOUBLE 100
#define TYPE_COMPLEX 200

class Solve: public Object
{
private:
  mat* data;
  cx_mat* cx_data;
  uint32_t rows;
  uint32_t cols;
  uint32_t type;
public:
  Solve();
  virtual ~Solve();

  static TypeId GetTypeId (void);

  void Initialize(uint32_t cols, uint32_t rows, uint32_t type);
  void FillR();
  void SetValue(int col, int row, double value);
  void SetValue(int col, int row, cx_double value);
  void Zeros();
  void Clear();
  void Print();

  double GetValue(int col, int row);
  cx_double GetCxValue(int col, int row);

  mat Product(vec error);
  cx_mat ProductCx(vec error);
  mat Inverse();
  cx_mat InverseCx();

  uint32_t GetCols() const ;

  void SetCols(uint32_t cols);

  uint32_t GetRows() const;

  void SetRows(uint32_t rows);

  uint32_t GetType() const;

  void SetType(uint32_t type);
};
}

#endif /* SOLVE_H_ */
