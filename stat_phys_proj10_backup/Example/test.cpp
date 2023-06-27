// Copyright ETH Zurich, Laboratory for Physical Chemistry, Reiher Group.
// See LICENSE.txt for details.

#include <StatPhysQC.hpp>
#include <iostream>

using namespace std;

void example3();

int main(int argc, char* argv[]) {
  example3();
}
/*
 * Calculate the forces acting on the atoms of a molecule,
 * have the atoms move in the corresponding direction (1 step) and save the new structure file.
 */
void example3() {
  cout << "\nExample 3\n"
          "---------\n\n";

  std::string filename = "2-HAtom.xyz";
  auto structure = readStructureFromFile(filename);
  vector<int> elements = structure.first;
  vector<double> positions = structure.second;

  QuantumChemicalCalculation dftbCalculation{Method::dftb};
  dftbCalculation.initialize(elements);
  dftbCalculation.setPositionsInAngstrom(positions);
  dftbCalculation.calculate();

  vector<double> forces = dftbCalculation.getForcesInEvPerAngstrom();

  double factor = 0.001;
  for (unsigned i = 0; i < forces.size(); ++i)
    positions[i] += forces[i] * factor;

  writeStructureToFile(elements, positions, "2-HAtom_new.xyz");
  cout << "Wrote new positions to file 2-HAtom_new.xyz" << endl;
}
