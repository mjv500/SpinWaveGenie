#include "SW_Builder.h"

using namespace std;

SW_Builder::SW_Builder(boost::shared_ptr<Cell>& cell_in)
{
    cell = cell_in;
    SpinWave temp(cell);
    SW = temp;
}

void SW_Builder::Add_Interaction(Interaction * in)
{
    interactions.push_back(in);
}

SpinWave SW_Builder::Create_Element(double KX,double KY,double KZ)
{
    Eigen::Vector3d K;
    Eigen::Matrix3d recip;
    recip = cell->getReciprocalVectors();
    K << KX,KY,KZ;
    //cout << "K before " << K.transpose() << endl;
    K = recip*K;
    //cout << "K after " << K.transpose() << endl;
    SW.Set_Kpoint(K[0],K[1],K[2]);
    SW.Clear_Matrix();
    //sort(interactions.begin(),interactions.end());
    interactions.sort();
    boost::ptr_vector<Interaction>::iterator iter;
    int quad = 0;
    for (iter = interactions.begin(); iter != interactions.end(); iter++)
    {
        iter->Update_Matrix(K,SW.cell,SW.LN,quad);
        iter->Update_Matrix(K,SW.cell,SW.LN,quad+1);
    }
    quad = 2;
    for (iter = interactions.begin(); iter != interactions.end(); iter++)
    {
        iter->Update_Matrix(K,SW.cell,SW.LN,quad);
        iter->Update_Matrix(K,SW.cell,SW.LN,quad+1);
    }
    return SW;
}