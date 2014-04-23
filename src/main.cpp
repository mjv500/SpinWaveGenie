#include <vector>
#include <iostream>
#include <fstream>
#include "Initializer.h"
#include "Genie/SW_Builder.h"
#include "OneDimensionalFactory.h"
#include "interactions/InteractionFactory.h"
#include "IntegrateThetaPhi.h"
#include "Cell/Neighbors.h"
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;
int main()
{
    Matrix3d basisVectors;
    //basisVectors << 0.5,0.5,-0.5,
    //-0.5,0.5,0.5,
    //0.5,-0.5,0.5;
    
    //basisVectors << 0.5,0.5,0.0,
    //0.0,0.5,0.5,
    //0.5,0.0,0.5;
    
    //basisVectors << 3.0000000000,0.0000000000,0.0000000000,
    //-1.5000000000, 2.5980762114, 0.0000000000,
    //0.0000000000, 0.0000000000, 7.0000000000;
    
    basisVectors << 1.0,0.0,0.0,
    0.0,10.0,0.0,
    0.0,0.0,10.0;
    
    
    cout << basisVectors << endl << endl;
    cout << basisVectors.inverse() << endl;
    
    double MinimumEnergy = 0.0;
    double MaximumEnergy = 5.0;
    int EnergyPoints = 101;
    
    OneDimensionalFactory factory;
    InteractionFactory intFactory;
    
    std::unique_ptr<OneDimensionalShapes> InstrumentResolution = factory.getGaussian(0.5,1.0e-5);
    
    //Init inputfile = Init("/Users/svh/Documents/spin_wave_genie/build/2FMChain.xml");
    //SW_Builder builder = inputfile.get_builder();
    
    Cell cell;
    cell.setBasisVectors(1.0,10.0,10.0,90.0,90.0,90.0);
    
    Sublattice Spin0;
    string name = "Spin0";
    Spin0.setName(name);
    Spin0.setType("NONE");
    Spin0.setMoment(1.0,0.0,0.0);
    cell.addSublattice(Spin0);
    cell.addAtom(name,0.0,0.0,0.0);
    
    //Neighbors neighbor;
    //string spin0 = "Spin0";
    //neighbor.findNeighbors(cell,spin0,spin0,0.99,1.01);
    
    cout << "cell check(main):" << cell.begin()->getName() << endl;

    SW_Builder builder(cell);
    
    Vector3 zhat(0.0,0.0,1.0);
    builder.Add_Interaction(intFactory.getExchange("J",1.0,name,name,0.99,1.01).release());
    builder.Add_Interaction(intFactory.getAnisotropy("D",-0.000000001, zhat, name).release());

    SpinWave SW = builder.Create_Element();
    
    /*SW.createMatrix(1.0,0.0,0.0);
    SW.Calc();
    vector<point> points = SW.getPoints();
    
    for (auto it = points.begin(); it!=points.end(); it++)
    {
        cout << it->frequency << " " << it->intensity << endl;
    }*/
    
    EnergyResolutionFunction scan(move(InstrumentResolution),SW,MinimumEnergy,MaximumEnergy,EnergyPoints);
    //vector<double> values = scan.getCut(0.5,0.0,0.0);
    
    //for (auto it = values.begin(); it!=values.end(); it++)
    //{
    //cout << (*it) << endl;
    //}
    
    //values = scan2.getCut(0.5,0.0,0.0);
    
    //for (auto it = values.begin(); it!=values.end(); it++)
    //{
    //    cout << (*it) << endl;
    //}
    
    IntegrateThetaPhi func(MinimumEnergy,MaximumEnergy,EnergyPoints,basisVectors);
    func.setConvolutionObject(scan);
    
    std::string filename("output.txt");
    std::ofstream file(filename);
    for (int value = 1; value < 101; value++)
    {
        cout << value << endl;
        vector<double> result = func.getCut((double)value/50.0,0.0,0.0);
        for ( auto it = result.begin(); it != result.end(); it++)
        {
            file << (*it) << " ";
        }
        file << endl;
    }
    return 0;
}
