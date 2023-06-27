#include "MDSimulation.h"
#include "TrajectoryFileWriter.h"
#include "MDRun.h"
#include "CoordinatesAndVelocitiesInitializer.h"
#include "ParameterIO.h"
#include "ParameterValidityChecker.h"
#include "RandomNumberGenerator.h"

MDSimulation::MDSimulation(std::ostream& outputStream)
  : output(outputStream) {
}

void MDSimulation::performSimulation(const std::string& parameterFile,
  const std::pair<std::vector<int>, std::vector<double>> ElemAndPos, double velo) { //instead of coordinateFile we give elementTypes and Positions
    MDParameters par = ParameterIO::readParameters(parameterFile);
    performSimulation(par, ElemAndPos, velo);
}

void MDSimulation::performSimulation(const MDParameters& par,
  const std::pair<std::vector<int>, std::vector<double>> ElemAndPos, double velo)
  { //instead of coordinateFile we give elementTypes and Positions)
    parameters = par;
    _elements = ElemAndPos.first; //assigns elementtypes to a vector
    _positions = ElemAndPos.second; // assigns elementpositions to a vector (x1,y1,z1, x2,y2,z2...)
    prepareRun();
    checkParameterValidity();
    initializeCoordinatesAndVelocities(_positions, velo);
    executeMDIterations();
    finalizeRun();
}

void MDSimulation::prepareRun() {
    RandomNumberGenerator::setSeed(parameters.randomSeed);
    timer.programStart();
    output.printHeader();
    output.printParameters(parameters);
}

void MDSimulation::checkParameterValidity() {
    ParameterValidityChecker validityChecker(parameters);
    if (!validityChecker.valid()) {
        throw std::runtime_error("Invalid parameters: " + validityChecker.getErrorMessage());
    }
}
//TODO
void MDSimulation::initializeCoordinatesAndVelocities(std::vector<double>& posis, double velos) {
    CoordinatesAndVelocitiesInitializer xvInitializer(output, parameters, posis);
    xvInitializer.initialize(_positions, _velocities, velos);
}

void MDSimulation::executeMDIterations() {
    TrajectoryFileWriter trajectoryWriter(parameters, "coords.final", "coords.traj");
    trajectoryWriter.writeBeforeRun();

    timer.mdStart();
    MDRun mdRun(parameters, output, trajectoryWriter);
    mdRun.run(_positions, _velocities, _elements);
    timer.mdEnd();

    printRadialDistribution(mdRun.getRadialDistribution());
    trajectoryWriter.writeFinalCoordinates(_positions, _velocities);
}

void MDSimulation::printRadialDistribution(const AveragedRadialDistribution &radialDistribution) {
    if (parameters.radialDistrCutoffRadius > 0 && parameters.numberAtoms > 1) {
        int ngr = radialDistribution.getNumberBins();
        double dgr = radialDistribution.getBinSize();
        std::vector<double> gr = radialDistribution.calculateNormalizedDistribution(parameters.numberAtoms,
                                                                                    parameters.boxSize[0] *
                                                                                    parameters.boxSize[1] *
                                                                                    parameters.boxSize[2]);
        output.printRadialDistribution(ngr, dgr, gr.data());
    }
}

void MDSimulation::finalizeRun() {
    timer.programEnd();
    output.printTiming(timer);
}
