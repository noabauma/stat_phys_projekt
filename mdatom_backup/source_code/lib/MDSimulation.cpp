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

void MDSimulation::performSimulation(const std::string& parameterFile, const std::string& coordinateFile) {
    MDParameters par = ParameterIO::readParameters(parameterFile);
    performSimulation(par, coordinateFile);
}

void MDSimulation::performSimulation(const MDParameters& par, const std::string& coordinateFile) {
    parameters = par;
    prepareRun();
    checkParameterValidity();
    initializeCoordinatesAndVelocities(coordinateFile);
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

void MDSimulation::initializeCoordinatesAndVelocities(const std::string& coordinateFile) {
    CoordinatesAndVelocitiesInitializer xvInitializer(output, parameters, coordinateFile);
    xvInitializer.initialize(positions, velocities);
}

void MDSimulation::executeMDIterations() {
    TrajectoryFileWriter trajectoryWriter(parameters, "coords.final", "coords.traj");
    trajectoryWriter.writeBeforeRun();

    timer.mdStart();
    MDRun mdRun(parameters, output, trajectoryWriter);
    mdRun.run(positions, velocities);
    timer.mdEnd();

    printRadialDistribution(mdRun.getRadialDistribution());
    trajectoryWriter.writeFinalCoordinates(positions, velocities);
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
