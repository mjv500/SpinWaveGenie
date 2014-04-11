#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "nlopt.hpp"
#include "tbb/tbb.h"
#include "Genie/SpinWave.h"
#include "Initializer.h"
#include "Cell/Cell.h"
#include "Cell/Neighbors.h"
#include "Interactions/ExchangeInteraction.h"
#include "Interactions/AnisotropyInteraction.h"
#include "TwoDimensionCut.h"
#include "OneDimensionalFactory.h"
#include "OneDimensionalShapes.h"
#include "PointsAlongLine.h"
#include "CalculateAngles.h"

using namespace std;
using namespace Eigen;
using namespace tbb;

void myfunc(const std::vector<double> &x)
{

    /*cout << "x[i] = ";
    for (size_t i = 0; i< x.size();i++)
    {
        cout << x[i] << " ";
    }
    cout << endl;
    */
    
    double SA = 1.5;
    double SB = 1.0;
    
    CalculateAngles angles(SA,SB,x[0],x[1],x[1],x[2]);
    
    double minf = 0.0;
    std::vector<double> ub = {M_PI,2.0*M_PI,M_PI,2.0*M_PI,M_PI,2.0*M_PI,M_PI,2.0*M_PI};
    std::vector<double> lb = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    std::vector<double> thetaphi = {2.44,3.0*M_PI/4.0,2.44,7.0*M_PI/4.0,2.44,M_PI/4.0,2.44,5.0*M_PI/4.0};
    
    nlopt::opt opt(nlopt::LN_COBYLA,8);
    opt.set_upper_bounds(ub);
    opt.set_lower_bounds(lb);
    
    opt.set_ftol_abs(1.0e-13);
    opt.set_maxeval(5000);
    
    opt.set_min_objective(CalculateAngles::calc,&angles);
    
    try
    {
        opt.optimize(thetaphi,minf);
    } catch (...)
    {
        cout << "exception occurred" << endl;
    }
    
    if ( abs(thetaphi[0] + thetaphi[2] - thetaphi[4] - thetaphi[6]) < 0.01)
    {
    
    
    //for(auto it = thetaphi.begin();it!=thetaphi.end();it++)
    //{
    //    cout << (*it) << " ";
    //}
    //cout << endl;
    double theta = thetaphi[0];
    //cout << "theta= " << theta*180.0/M_PI << endl;
    
    Cell cell;
    cell.setBasisVectors(8.5,8.5,8.5,90.0,90.0,90.0);
    
    Sublattice Mn0;
    string name = "Mn0";
    Mn0.setName(name);
    Mn0.setType("MN2");
    Mn0.setMoment(SA,0.0,0.0);
    cell.addSublattice(Mn0);
    cell.addAtom(name,0.0,0.0,0.0);
    cell.addAtom(name,0.0,0.5,0.5);
    cell.addAtom(name,0.5,0.0,0.5);
    cell.addAtom(name,0.5,0.5,0.0);
    
    Sublattice Mn1;
    name = "Mn1";
    Mn1.setName(name);
    Mn1.setType("MN2");
    Mn1.setMoment(SA,0.0,0.0);
    cell.addSublattice(Mn1);
    cell.addAtom(name,0.75,0.25,0.75);
    cell.addAtom(name,0.75,0.75,0.25);
    cell.addAtom(name,0.25,0.25,0.25);
    cell.addAtom(name,0.25,0.75,0.75);
    
    Sublattice V0;
    name = "V0";
    V0.setName(name);
    V0.setType("V3");
    V0.setMoment(SB,theta,3.0*M_PI/4.0);
    cell.addSublattice(V0);
    cell.addAtom(name,0.875,0.125,0.375);
    cell.addAtom(name,0.875,0.625,0.875);
    cell.addAtom(name,0.375,0.125,0.875);
    cell.addAtom(name,0.375,0.625,0.375);
    
    Sublattice V1;
    name = "V1";
    V1.setName(name);
    V1.setType("V3");
    V1.setMoment(SB,theta,7.0*M_PI/4.0);
    cell.addSublattice(V1);
    cell.addAtom(name,0.125,0.375,0.875);
    cell.addAtom(name,0.125,0.875,0.375);
    cell.addAtom(name,0.625,0.375,0.375);
    cell.addAtom(name,0.625,0.875,0.875);
    
    Sublattice V2;
    name = "V2";
    V2.setName(name);
    V2.setType("V3");
    V2.setMoment(SB,theta,M_PI/4.0);
    cell.addSublattice(V2);
    cell.addAtom(name,0.375,0.875,0.125);
    cell.addAtom(name,0.375,0.375,0.625);
    cell.addAtom(name,0.875,0.875,0.625);
    cell.addAtom(name,0.875,0.375,0.125);
    
    Sublattice V3;
    name = "V3";
    V3.setName(name);
    V3.setType("V3");
    V3.setMoment(SB,theta,5.0*M_PI/4.0);
    cell.addSublattice(V3);
    cell.addAtom(name,0.625,0.625,0.625);
    cell.addAtom(name,0.625,0.125,0.125);
    cell.addAtom(name,0.125,0.625,0.125);
    cell.addAtom(name,0.125,0.125,0.625);
    
    SW_Builder builder(cell);
    
    builder.Add_Interaction(new ExchangeInteraction("Jbb",x[1],"V0","V1",2.975,3.06));
    builder.Add_Interaction(new ExchangeInteraction("Jbb",x[1],"V2","V3",2.975,3.06));

    builder.Add_Interaction(new ExchangeInteraction("Jbbp",x[1],"V0","V2",2.975,3.06));
    builder.Add_Interaction(new ExchangeInteraction("Jbbp",x[1],"V0","V3",2.975,3.06));
    builder.Add_Interaction(new ExchangeInteraction("Jbbp",x[1],"V1","V2",2.975,3.06));
    builder.Add_Interaction(new ExchangeInteraction("Jbbp",x[1],"V1","V3",2.975,3.06));
       
    Vector3 direction(-1.0,1.0,-1.0);
    builder.Add_Interaction(new AnisotropyInteraction("Dby",x[2],direction,"V0"));
    direction = Vector3(1.0,-1.0,-1.0);
    builder.Add_Interaction(new AnisotropyInteraction("Dbx",x[2],direction,"V1"));
    direction = Vector3(1.0,1.0,-1.0);
    builder.Add_Interaction(new AnisotropyInteraction("Dby",x[2],direction,"V2"));
    direction = Vector3(-1.0,-1.0,-1.0);
    builder.Add_Interaction(new AnisotropyInteraction("Dbx",x[2],direction,"V3"));
    
    //double JBB = x[0];
    //double JBBP = x[1];
    //double DB = x[2];
    //double JAB = SB*((6.0*JBB+6.0*JBBP+DB)*cos(theta)*sin(theta) -sqrt(2.0)*DB*(2.0*pow(cos(theta),2)-1.0))/(-9.0*SA*sin(theta));
    
   // cout << "JAB= " << JAB << endl;
    
    builder.Add_Interaction(new ExchangeInteraction("Jab",x[0],"Mn0","V0",3.48,3.57));
    builder.Add_Interaction(new ExchangeInteraction("Jab",x[0],"Mn0","V1",3.48,3.57));
    builder.Add_Interaction(new ExchangeInteraction("Jab",x[0],"Mn0","V2",3.48,3.57));
    builder.Add_Interaction(new ExchangeInteraction("Jab",x[0],"Mn0","V3",3.48,3.57));
    builder.Add_Interaction(new ExchangeInteraction("Jab",x[0],"Mn1","V0",3.48,3.57));
    builder.Add_Interaction(new ExchangeInteraction("Jab",x[0],"Mn1","V1",3.48,3.57));
    builder.Add_Interaction(new ExchangeInteraction("Jab",x[0],"Mn1","V2",3.48,3.57));
    builder.Add_Interaction(new ExchangeInteraction("Jab",x[0],"Mn1","V3",3.48,3.57));
    
    SpinWave SW = builder.Create_Element();
    
    TwoDimensionCut Cut;
    Cut.setSpinWave(SW);

    string filename = "MnV2O4_"+boost::lexical_cast<string>(x[0])+"_"+boost::lexical_cast<string>(x[1])+"_"+boost::lexical_cast<string>(x[2])+"_"+boost::lexical_cast<string>(theta);

    Cut.setEnergyPoints(0.0,25.0,201);
    
    OneDimensionalFactory factory;
    auto resinfo = factory.getGaussian(1.0,0.0001);
    Cut.setConvolutionObject(move(resinfo));
    
    PointsAlongLine Line;
    Line.setFirstPoint(1.0,1.0,0.0);
    Line.setFinalPoint(3.0,3.0,0.0);
    Line.setNumberPoints(201);
    Cut.setPoints(Line.getPoints());
    
    //Cut.setFilename(filename+"_x.txt");
    Cut.setSpinWave(SW);
    //Cut.save();
    
    MatrixXd figure = Cut.getMatrix();
    
    Line.setFirstPoint(0.0,1.0,1.0);
    Line.setFinalPoint(0.0,3.0,3.0);
    Line.setNumberPoints(201);
    Cut.setPoints(Line.getPoints());
    
    figure += Cut.getMatrix();
    
    //Cut.setFilename(filename+"_y.txt");
    
    //Cut.save();
    
    Line.setFirstPoint(1.0,0.0,1.0);
    Line.setFinalPoint(3.0,0.0,3.0);
    Line.setNumberPoints(201);
    Cut.setPoints(Line.getPoints());
    
    figure += Cut.getMatrix();
    
    if ( figure.maxCoeff() < 100.0 && figure(16,100) > 0.4 && figure(80,1) > 0.4)
    {
        double woof = figure.block(40,100,80,1).sum();
        if ( woof < 0.1 )
        {
            cout << filename << endl;
            std::ofstream file(filename+"_"+boost::lexical_cast<string>(woof)+".txt");
            if (file.is_open())
            {
                file << figure;
            }
            file << endl;
            file.close();
        }
    }
   
    //Cut.setFilename(filename+"_z.txt");
    //Cut.setSpinWave(SW);
    
    //Cut.save();
    }
}

double findValue(int i, int numberSteps, double lowerBound, double upperBound)
{
    if(numberSteps==1)
    {
        return lowerBound;
    }
    else
    {
        return lowerBound + (upperBound-lowerBound)*(double)i/(numberSteps-1);
    }
}

class ApplyFoo
{
    vector<vector<double> > my_a;
public:
    void operator()( const blocked_range<size_t>& r ) const

    {
        for( size_t i=r.begin(); i!=r.end(); ++i )
            myfunc(my_a[i]);
    }
    
    ApplyFoo( vector< vector<double> > a ) : my_a(a) {}
};

void ParallelApplyFoo( vector<vector<double> > a, size_t n ) {
    parallel_for(blocked_range<size_t>(0,n), ApplyFoo(a));
}

int main()
{
    cout << "#MinValue JAB JBB JBBP DAZ DBZ DB111 " << endl;

    std::vector<double> ub(4),lb(4);
    std::vector<size_t> ns(4);
    ub[0] =  -1.0;
    ub[1] =  -20.0;
    ub[2] =  -1.0;

    lb[0] =  -4.0;
    lb[1] =  -40.0;
    lb[2] =  -4.0;
    
    ns[0] =  12;
    ns[1] =  41;
    ns[2] =  12;
    
    vector<vector<double> > parameters;
    
    for (int h=0;h<ns[0];h++)
    {
        double jab = findValue(h,ns[0],lb[0],ub[0]);
        for (int i=0;i<ns[1];i++)
        {
            double jbb = findValue(i,ns[1],lb[1],ub[1]);
            for (int k=0;k<ns[2];k++)
            {
                double db = findValue(k,ns[2],lb[2],ub[2]);
                vector<double> xtemp = {jab,jbb,db};
                parameters.push_back(xtemp);
                //cout << jab << " " << jbb << " " << jbbp << " " << db << endl;
                //myfunc(xtemp);
            }
        }
    }
    ParallelApplyFoo(parameters, parameters.size());
    return 0;
}
