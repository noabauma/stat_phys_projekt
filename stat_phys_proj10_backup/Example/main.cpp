// Copyright ETH Zurich, Laboratory for Physical Chemistry, Reiher Group.
// See LICENSE.txt for details.

#include <StatPhysQC.hpp>
#include <iostream>

using namespace std;

void example1_dftb();
void example1_pm6();
void example2();
void example3();

int main(int argc, char* argv[]) {
  example1_dftb();
  example1_pm6();
  example2();
  example3();
}

/*
 * Specify a molecular system and calculate its energy and the forces acting on the different atoms with dftb.
 */
void example1_dftb() {
  cout << "Example 1 (DFTB)\n"
          "---------\n\n";
  int nAtoms = 5;
  vector<int> elements = {6, 1, 1, 1, 1};
  vector<double> positions = {0.00000, 0.00000, 0.00000,
                              0.00000, 0.00000, 1.08900,
                              1.02672, 0.00000, -0.36300,
                              -0.51336, -0.88916, -0.36300,
                              -0.51336, 0.88916, -0.36300};

  QuantumChemicalCalculation dftbCalculation{Method::dftb, "Parameters"};
  dftbCalculation.initialize(elements);
  dftbCalculation.setPositionsInAngstrom(positions);
  dftbCalculation.calculate();

  double energy = dftbCalculation.getEnergyInEv();
  vector<double> forces = dftbCalculation.getForcesInEvPerAngstrom();

  cout << "The energy is " << energy << " eV." << endl;
  for (int i = 0; i < nAtoms; ++i)
    cout << "The force for atom " << i + 1 << " is {" << forces[3 * i] << ", " << forces[3 * i + 1] << ", "
         << forces[3 * i + 2] << "} eV/Angstrom." << endl;

  for (int i = 0; i < nAtoms; ++i)
    for (int j = i + 1; j < nAtoms; ++j)
      cout << "The bond order between atoms " << i + 1 << " and " << j + 1 << " is "
           << dftbCalculation.getBondOrderBetweenAtoms(i, j) << "." << endl;
}

/*
 * Same as example1_dftb, but with the PM6 method.
 */
void example1_pm6() {
  cout << "\nExample 1 (PM6)\n"
          "---------\n\n";
  int nAtoms = 5;
  vector<int> elements = {6, 1, 1, 1, 1};
  vector<double> positions = {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 1.08900, 1.02672, 0.00000,
                              -0.36300, -0.51336, -0.88916, -0.36300, -0.51336, 0.88916, -0.36300};

  QuantumChemicalCalculation pm6Calculation{Method::pm6, "Parameters"};
  pm6Calculation.initialize(elements);
  pm6Calculation.setPositionsInAngstrom(positions);
  pm6Calculation.calculate();

  double energy = pm6Calculation.getEnergyInEv();
  vector<double> forces = pm6Calculation.getForcesInEvPerAngstrom();

  cout << "The energy is " << energy << " eV." << endl;
  for (int i = 0; i < nAtoms; ++i)
    cout << "The force for atom " << i + 1 << " is {" << forces[3 * i] << ", " << forces[3 * i + 1] << ", "
         << forces[3 * i + 2] << "} eV/Angstrom." << endl;

  for (int i = 0; i < nAtoms; ++i)
    for (int j = i + 1; j < nAtoms; ++j)
      cout << "The bond order between atoms " << i + 1 << " and " << j + 1 << " is "
           << pm6Calculation.getBondOrderBetweenAtoms(i, j) << "." << endl;
}

/*
 * Same as example1(), but read the positions and element types from a file instead of specifying them manually.
 */
void example2() {
  cout << "\nExample 2\n"
          "---------\n\n";
  string filename = "methane.xyz";
  auto structure = readStructureFromFile(filename);
  vector<int> elements = structure.first;
  vector<double> positions = structure.second;

  QuantumChemicalCalculation dftbCalculation{Method::dftb};
  dftbCalculation.initialize(elements);
  dftbCalculation.setPositionsInAngstrom(positions);
  dftbCalculation.calculate();

  double energy = dftbCalculation.getEnergyInEv();
  cout << "The energy is " << energy << " eV." << endl;
  /*
   * ... and so on (same as example1()).
   */
}

/*
 * Calculate the forces acting on the atoms of a molecule,
 * have the atoms move in the corresponding direction (1 step) and save the new structure file.
 */
void example3() {
  cout << "\nExample 3\n"
          "---------\n\n";

  std::string filename = "methane.xyz";
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

  writeStructureToFile(elements, positions, "methane_new.xyz");
  cout << "Wrote new positions to file methane_new.xyz" << endl;
}
