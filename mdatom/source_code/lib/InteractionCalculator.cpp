#include "InteractionCalculator.h"
#include <cmath>
#include <StatPhysQC.hpp>

inline int nearestInteger(double x) {
    return x > 0 ? static_cast<int>(x + 0.5) : static_cast<int>(x - 0.5);
}

InteractionCalculator::InteractionCalculator(const MDParameters& parameters)
  : par(parameters),
    radialDistribution(parameters.numberRadialDistrPoints, parameters.radialDistrCutoffRadius) {
    initializeValues();
}

void InteractionCalculator::initializeValues() {

    sig6 = par.sigmaLJ * par.sigmaLJ;
    sig6 = sig6 * sig6 * sig6;
    c6 = 4. * par.epsilonLJ * sig6;
    c12 = c6 * sig6;
    rcutf2 = par.interactionCutoffRadius * par.interactionCutoffRadius;
    for (int m = 0; m < 3; m++)
        inverseBoxLength[m] = 1.0 / par.boxSize[m];

}
//transform units back to angstrom, calculate stuff and retransfo
void InteractionCalculator::calculate(const std::vector<double>& positions, std::vector<double>& forces, std::vector<int>& elements) {
  //ok
    resetVariablesToZero(forces);
    std::vector<double> positions_old = positions;
    for (size_t i = 0; i < positions_old.size(); i++) {
      positions_old[i]*=10; //now positions are in Angstrom again
    }
    //dftb or pm6
    QuantumChemicalCalculation dftbCalculation(Method::dftb, "Parameters");
    dftbCalculation.initialize(elements);
    dftbCalculation.setPositionsInAngstrom(positions_old);
    dftbCalculation.calculate();
    //transform to kilojoule/mole
    potentialEnergy = dftbCalculation.getEnergyInEv()*96.485;
    //https://www.wolframalpha.com/input/?i=1+ev%2Fangstrom+to+nm*u%2F(ps%5E2), not sure if this is correct
    forces = dftbCalculation.getForcesInEvPerAngstrom();
    for (size_t i = 0; i < forces.size(); i++) {
      forces[i]*=964.8533;
    }

    virial /= 2.;
}
//relevant
void InteractionCalculator::resetVariablesToZero(std::vector<double>& forces) {
    radialDistribution.setZero();
    potentialEnergy = 0;
    virial = 0;
    int nat3 = 3 * par.numberAtoms;
    for (int j3 = 0; j3 < nat3; j3++)
        forces[j3] = 0;
}
//irrelevant
void InteractionCalculator::calculateInteraction(int i, int j, const std::vector<double>& positions,
                                                 std::vector<double>& forces) {
    applyPeriodicBoundaryConditions(i, j, positions);
    calculateSquaredDistance();
    if (rij2 < rcutf2) {
        calculatePotentialAndForceMagnitude();
        potentialEnergy += eij;
        calculateForceAndVirialContributions(i, j, forces);
    }
    radialDistribution.addPairAtSquaredDistance(rij2);
}

void InteractionCalculator::applyPeriodicBoundaryConditions(int i, int j, const std::vector<double>& positions) {
    int i3 = 3 * i;
    int j3 = 3 * j;
    for (int m = 0; m < 3; m++) {
        xij[m] = positions[i3 + m] - positions[j3 + m];
        xij[m] = xij[m] - nearestInteger(xij[m]*inverseBoxLength[m]) * par.boxSize[m];
    }
}

void InteractionCalculator::calculateSquaredDistance() {
    rij2 = 0;
    for (int m = 0; m < 3; m++)
        rij2 += xij[m] * xij[m];
}

void InteractionCalculator::calculatePotentialAndForceMagnitude() {
    double riji2 = 1.0 / rij2; // inverse inter-particle distance squared
    double riji6 = riji2 * riji2 * riji2; // inverse inter-particle distance (6th power)
    double crh = c12 * riji6;
    double crhh = crh - c6; //  L-J potential work variable
    eij= crhh * riji6;
    dij= 6. * (crh + crhh) * riji6 * riji2;
}

void InteractionCalculator::calculateForceAndVirialContributions(int i, int j, std::vector<double>& forces) {
    int i3 = 3 * i;
    int j3 = 3 * j;
    for (int m = 0; m < 3; m++) {
        // Force increment in direction of inter-particle vector
        //(note: xij[m]/rij is unit vector in inter-particle direction.)
        double df = xij[m] * dij;
        forces[i3 + m] += df;
        forces[j3 + m] -= df;
        virial -= xij[m] * df;
    }
}

double InteractionCalculator::getPotentialEnergy() const {
    return potentialEnergy;
}

double InteractionCalculator::getVirial() const {
    return virial;
}

const InstantaneousRadialDistribution& InteractionCalculator::getInstantaneousRadialDistribution() const {
    return radialDistribution;
}
