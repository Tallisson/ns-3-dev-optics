#include "solve.h"
#include <iostream>

#include "ns3/log.h"
#include "ns3/uinteger.h"

using namespace std;

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("Solve");
NS_OBJECT_ENSURE_REGISTERED (Solve);

TypeId
Solve::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Solve")
    .SetParent<Object> ()
    .AddConstructor<Solve> ()
    .AddAttribute ("Rows",
                   "Rows number",
                   UintegerValue (), /* TODO */
                   MakeUintegerAccessor (&Solve::rows),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Cols",
                   "Cols Number",
                   UintegerValue (),
                   MakeUintegerAccessor (&Solve::cols),
                   MakeUintegerChecker<uint32_t> ())
  ;

  return tid;
}

void
Solve::Initialize(uint32_t rows, uint32_t cols)
{
  this->rows = rows;
  this->cols = cols;
  data = new mat(rows, cols);
}

Solve::Solve():
  rows(0), cols(0)
{
}

Solve::~Solve()
{
  data->reset();
  delete data;
}

void Solve::FillR()
{
  data->randu(data->n_rows, data->n_rows);
  cout << *(data) << endl;
}

void Solve::SetValue(uint32_t row, uint32_t col, double value)
{
  (data->at(row, col)) = value;
}

void Solve::Zeros()
{
  data->zeros(rows, cols);
}

void Solve::Clear()
{
  data->reset();
  delete data;
}

void Solve::Print()
{
  cout << *(data) << endl;
}

double Solve::GetValue(uint32_t row, uint32_t col)
{
  return data->at(row, col);
}

mat Solve::Product(vec error)
{
  mat m = mat(((inv(*data)) * -error));

  return m;
}

mat Solve::Inverse()
{
  return inv(*(data));
}

uint32_t
Solve::GetCols() const {
  return cols;
}

void
Solve::SetCols(uint32_t cols) {
  this->cols = cols;
}

uint32_t
Solve::GetRows() const {
  return rows;
}

void
Solve::SetRows(uint32_t rows) {
  this->rows = rows;
}

}
