//
//  formfactor.cpp
//  spin_wave_genie
//
//  Created by Hahn, Steven E. on 11/20/13.
//
//
#include <string>
#include "MagneticFormFactor.h"

MagneticFormFactor::MagneticFormFactor()
{
    this->initializeMap();
}

MagneticFormFactor::MagneticFormFactor(std::string type)
{
    this->initializeMap();
    this->setType(type);
}

void MagneticFormFactor::initializeMap()
{
    coefficients =
    {
        {"CO0",std::vector<double>{ 0.413900, 16.161600,  0.601300,  4.780500, -0.151800,  0.021000,  0.134500}},
        {"CO1",std::vector<double>{ 0.099000, 33.125198,  0.364500, 15.176800,  0.547000,  5.008100, -0.010900}},
        {"CO2",std::vector<double>{ 0.433200, 14.355300,  0.585700,  4.607700, -0.038200,  0.133800,  0.017900}},
        {"CO3",std::vector<double>{ 0.390200, 12.507800,  0.632400,  4.457400, -0.150000,  0.034300,  0.127200}},
        {"CO4",std::vector<double>{ 0.351500, 10.778500,  0.677800,  4.234300, -0.038900,  0.240900,  0.009800}},
        {"FE0",std::vector<double>{ 0.070600, 35.008499,  0.358900, 15.358300,  0.581900,  5.560600, -0.011400}},
        {"FE1",std::vector<double>{ 0.125100, 34.963299,  0.362900, 15.514400,  0.522300,  5.591400, -0.010500}},
        {"FE2",std::vector<double>{ 0.026300, 34.959702,  0.366800, 15.943500,  0.618800,  5.593500, -0.011900}},
        {"FE3",std::vector<double>{ 0.397200, 13.244200,  0.629500,  4.903400, -0.031400,  0.349600,  0.004400}},
        {"FE4",std::vector<double>{ 0.378200, 11.380000,  0.655600,  4.592000, -0.034600,  0.483300,  0.000500}},
        {"MN0",std::vector<double>{ 0.243800, 24.962900,  0.147200, 15.672800,  0.618900,  6.540300, -0.010500}},
        {"MN1",std::vector<double>{-0.013800,  0.421300,  0.423100, 24.667999,  0.590500,  6.654500, -0.001000}},
        {"MN2",std::vector<double>{ 0.422000, 17.684000,  0.594800,  6.005000,  0.004300, -0.609000, -0.021900}},
        {"MN3",std::vector<double>{ 0.419800, 14.282900,  0.605400,  5.468900,  0.924100, -0.008800, -0.949800}},
        {"MN4",std::vector<double>{ 0.376000, 12.566100,  0.660200,  5.132900, -0.037200,  0.563000,  0.001100}},
        {"V0", std::vector<double>{ 0.408600, 28.810900,  0.607700,  8.543700, -0.029500,  0.276800,  0.012300}},
        {"V1", std::vector<double>{ 0.444400, 32.647900,  0.568300,  9.097100, -0.228500,  0.021800,  0.215000}},
        {"V2", std::vector<double>{ 0.408500, 23.852600,  0.609100,  8.245600, -0.167600,  0.041500,  0.149600}},
        {"V3", std::vector<double>{ 0.359800, 19.336399,  0.663200,  7.617200, -0.306400,  0.029600,  0.283500}},
        {"V4", std::vector<double>{ 0.310600, 16.816000,  0.719800,  7.048700, -0.052100,  0.302000,  0.022100}}
    };
}

void MagneticFormFactor::setType(std::string type)
{
    F = coefficients[type];
}

double MagneticFormFactor::getFormFactor(double x, double y, double z)
{
    double s2 = (pow(x,2) + pow(y,2) + pow(z,2))/(16.0*M_PI*M_PI);
    double f_Q = F[6];
    for(int k=0;k<3;k++)
    {
        f_Q += F[2*k]*exp(-1.0*F[2*k+1]*s2);
    }
    return f_Q;
}