#include "Anis_Z_Interaction.h"

using namespace std;
using namespace Eigen;


Anis_Z_Interaction::Anis_Z_Interaction(double value_in, string sl_r_in)
{
    this->Update_Interaction(value_in, sl_r_in);
}

void Anis_Z_Interaction::Update_Interaction(double value_in, string sl_r_in)
{
    value = value_in;
    sl_r = sl_r_in;
}

void Anis_Z_Interaction::Update_Matrix(Vector3d K, boost::shared_ptr<Cell> cell, MatrixXcd &LN)
{
        //find location of r
    int r= -1;
    int M = 0;
    for (SublatticeIterator sl=cell->begin(); sl!=cell->end(); ++sl)
    {
        if ( sl_r == (*sl)->getName())
            r = M;
        M++;
    }
    assert(r!=-1);
        
    double S = (*cell->getSublattice(sl_r)->getMoment())[0];
    double theta = (*cell->getSublattice(sl_r)->getMoment())[1];
    double X = value;
    
    LN(r,r) -= 0.5*X*S*(1.0-3.0*pow(cos(theta),2));
    LN(r,r+M) -= 0.2*X*S*pow(sin(theta),2);
    LN(r+M,r) -= 0.2*X*S*pow(sin(theta),2);
    LN(r+M,r+M) -= 0.5*X*S*(1.0-3.0*pow(cos(theta),2));
}