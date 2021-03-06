//
//  Anisotropy_Interaction.cpp
//  spin_wave_genie
//
//  Created by Hahn, Steven E. on 11/25/13.
//
//

#include "SpinWaveGenie/Interactions/AnisotropyInteraction.h"
using namespace std;

namespace SpinWaveGenie
{

AnisotropyInteraction::AnisotropyInteraction(string name_in, double value_in, Vector3 unitVectorIn, string sl_r_in)
    : name(name_in), r(0), M(0)
{
  this->updateInteraction(value_in, unitVectorIn, sl_r_in);
}

Interaction *AnisotropyInteraction::do_clone() const { return new AnisotropyInteraction(*this); }

void AnisotropyInteraction::updateInteraction(double value_in, Vector3 unitVectorIn, string sl_r_in)
{
  value = value_in;
  unitVectorIn.normalize();
  directions = unitVectorIn * unitVectorIn.transpose();
  sl_r = sl_r_in;
}

const string &AnisotropyInteraction::getName() { return name; }

void AnisotropyInteraction::updateValue(double value_in) { value = value_in; }

vector<string> AnisotropyInteraction::sublattices() const
{
  vector<string> sl;
  sl.push_back(sl_r);
  return sl;
}

void AnisotropyInteraction::calcConstantValues(Cell &cell)
{
  complex<double> XI(0.0, 1.0);
  // find location of r
  r = cell.getPosition(sl_r);
  M = cell.size();

  const double S = cell.getSublattice(sl_r).getMoment();
  const Matrix3 &inv = cell.getSublattice(sl_r).getInverseMatrix();

  LNrr = complex<double>(0.0, 0.0);
  LNrMr = complex<double>(0.0, 0.0);
  LNrrM = complex<double>(0.0, 0.0);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
    {
      // cout << i << " " << j << " " << directions(i,j) << endl;
      if (abs(directions(i, j)) > 1.0e-10)
      {
        double X = value * S * directions(i, j);
        double eta = 0.5 * (inv(i, 0) * inv(j, 0) - inv(i, 1) * inv(j, 1));
        complex<double> zeta = 0.5 * (inv(i, 0) * inv(j, 0) + inv(i, 1) * inv(j, 1) +
                                      XI * (inv(i, 1) * inv(j, 0) - inv(i, 0) * inv(j, 1)));
        // cout << "eta= " << eta << " zeta= " << zeta << endl;
        LNrr += X * (eta - inv(i, 2) * inv(j, 2));
        LNrMr += X * conj(zeta);
        LNrrM += X * zeta;
      }
    }
  // cout << "new implementation" << endl;
  // cout << LNrr << " "<< LNrMr << " " << LNrrM << " " << LNrMrM << endl;
}

void AnisotropyInteraction::calculateEnergy(Cell &cell, double &energy)
{
  if (r < 0)
  {
    r = cell.getPosition(sl_r);
  }
  double S = cell[r].getMoment();
  const Matrix3 &inv = cell[r].getInverseMatrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      // cout << i << " " << j << " " << directions(i,j) << endl;
      if (abs(directions(i, j)) > 1.0e-10)
      {
        energy += value * directions(i, j) * S * S * inv(i, 2) * inv(j, 2);
      }
    }
  }
}

void AnisotropyInteraction::calculateFirstOrderTerms(Cell &cell, Eigen::VectorXcd &elements)
{
  complex<double> XI(0.0, 1.0);
  double S = cell.getSublattice(sl_r).getMoment();
  const Matrix3 &inv = cell.getSublattice(sl_r).getInverseMatrix();
  r = cell.getPosition(sl_r);
  M = cell.size();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      // cout << i << " " << j << " " << directions(i,j) << endl;
      if (abs(directions(i, j)) > 1.0e-10)
      {
        double X = value * directions(i, j) * sqrt(pow(S, 3) / 2.0);
        complex<double> nu =
            inv(i, 2) * inv(j, 0) + inv(i, 0) * inv(j, 2) + XI * (inv(i, 2) * inv(j, 1) + inv(i, 1) * inv(j, 2));
        // cout << "nu= " << nu << endl;
        elements(r) += X * conj(nu);
        elements(r + M) += X * nu;
      }
    }
  }
}

void AnisotropyInteraction::updateMatrix(Vector3 /*K*/, Eigen::MatrixXcd &LN)
{
  LN(r, r) += LNrr;
  LN(r, r + M) += LNrrM;
  LN(r + M, r) += LNrMr;
  LN(r + M, r + M) += LNrr;
}
}
