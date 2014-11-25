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
    .AddAttribute ("Type",
                   "Data type",
                   UintegerValue (),
                   MakeUintegerAccessor (&Solve::type),
                   MakeUintegerChecker<uint32_t> ())    
  ;

  return tid;
}

Solve::Solve()
{
}

Solve::~Solve()
{
}

void 
Solve::Initialize(uint32_t rows, uint32_t cols, uint32_t type)  
{
  this->rows = rows;
  this->cols = cols;
  this->type = type;

  if(type == TYPE_DOUBLE)
  {
    data = new mat(cols, rows);
    cx_data = NULL;
  } else if(type == TYPE_COMPLEX)
  {
    data = NULL;
    cx_data = new cx_mat(cols, rows);
  }
}

void Solve::FillR()
{
  if(type == TYPE_DOUBLE)
  {
    data->randu(data->n_rows, data->n_rows);
  } else if(type == TYPE_COMPLEX)
  {
    cx_data->randu(data->n_rows, data->n_rows);
  }
}

void Solve::SetValue(int row, int col, double value)
{
  (data->at(row, col)) = value;
}

void Solve::SetValue(int row, int col, cx_double value)
{
  (cx_data->at(row, col)) = value;
}


void Solve::Zeros()
{
  if(type == TYPE_DOUBLE)
  {
    data->zeros(rows, cols);
  } else if(type == TYPE_COMPLEX)
  {
    cx_data->zeros(rows, cols);
  }
}

void Solve::Clear()
{
  if(type == TYPE_DOUBLE)
  {
    data->reset();
    delete data;
  } else if(type == TYPE_COMPLEX)
  {
    cx_data->reset();
    delete cx_data;
  }
}

void Solve::Print()
{
  if(type == TYPE_DOUBLE)
  {
    cout << *(data) << endl;
  } else if(type == TYPE_COMPLEX)
  {
    cout << *(cx_data) << endl;
  }
}

double Solve::GetValue(int row, int col)
{
  return data->at(row, col);
}

cx_double Solve::GetCxValue(int col, int row)
{
  return cx_data->at(row, col);
}

mat Solve::Product(vec error)
{
  mat m = mat(((inv(*data)) * -error));

  return m;
}

cx_mat Solve::ProductCx(vec error)
{
  cx_mat m = cx_mat(((inv(*cx_data)) * -error));

  return m;
}

mat Solve::Inverse()
{
  return inv(*(data));
}

cx_mat Solve::InverseCx()
{
  return inv(*(cx_data));
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

uint32_t
Solve::GetType() const {
  return type;
}

void Solve::SetType(uint32_t type) {
  this->type = type;
}

}
