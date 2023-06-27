//TODO
#ifndef MDSIMULATION_H
#define MDSIMULATION_H

#include "MDParameters.h"
#include "MDRunOutput.h"
#include "Timer.h"
#include "AveragedRadialDistribution.h"
#include <string>
#include <vector>
#include <iostream>

/*!
 * This class launches a MD simulation starting from parameters and, optionally, coordinates.
 */
class MDSimulation {
  public:
    /*! Constructor; the output of the MD simulation will be redirected to outputStream. */
    explicit MDSimulation(std::ostream& outputStream);

    /*! Perform a simulation based on a parameter file and an (optional) coordinate file. */
    void performSimulation(const std::string& parFile, const std::pair<std::vector<int>,
      std::vector<double>> ElemAndPos, double velo);
    /*! Perform a simulation based parameters and an (optional) coordinate file. */
    void performSimulation(const MDParameters& par, const std::pair<std::vector<int>,
      std::vector<double>> ElemAndPos, double velo);

  private:
    void prepareRun();
    void checkParameterValidity();
    void initializeCoordinatesAndVelocities(std::vector<double>& posis, double velo);
    void executeMDIterations();
    void printRadialDistribution(const AveragedRadialDistribution& radialDistribution);
    void finalizeRun();

    MDRunOutput output;
    Timer timer;
    MDParameters parameters;
    std::vector<double> _positions, _velocities;
    std::vector<int> _elements;
};

#endif // MDSIMULATION_H
