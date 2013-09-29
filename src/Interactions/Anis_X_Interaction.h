#ifndef __AnisX_Interaction_H__
#define __AnisX_Interaction_H__ 1

#include <string>
#include <vector>
#include <Eigen/Dense>
#include <boost/shared_ptr.hpp>
#include "Cell.h"
#include "Interaction.h"

//!
/*!
 */
class Anis_X_Interaction: public Interaction
{
public:
    //!
    Anis_X_Interaction(double value_in, std::string sl_r_in);
    //!
    void Update_Interaction(double value_in, std::string sl_r_in);
    //!
    void Update_Matrix(Eigen::Vector3d K, boost::shared_ptr<Cell> cell, Eigen::MatrixXcd &LN, int quadrant);
    std::vector<std::string> sublattices() const;
private:
    std::string sl_r;
    double value;
};

#endif