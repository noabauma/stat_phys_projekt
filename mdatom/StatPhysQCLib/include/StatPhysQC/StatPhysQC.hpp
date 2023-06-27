#ifndef STATPHYSQC_HPP
#define STATPHYSQC_HPP

#include "statphysqc_export.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

/*!
 * Functions to read a molecular structure from a given file.
 */
STATPHYSQC_EXPORT int getNumberAtomsInFile(const char* xyzFileName);
STATPHYSQC_EXPORT void readStructureFromFile(int* elements, double* positions, const char* xyzFileName);
inline std::pair<std::vector<int>, std::vector<double>> readStructureFromFile(const std::string& xyzFileName) {
  int numberAtoms = getNumberAtomsInFile(xyzFileName.c_str());
  std::vector<int> elements(numberAtoms);
  std::vector<double> positions(3 * numberAtoms);
  readStructureFromFile(elements.data(), positions.data(), xyzFileName.c_str());
  return std::make_pair(elements, positions);
}

/*!
 * Functions to save a molecular structure to a given file.
 */
STATPHYSQC_EXPORT void writeStructureToFile(int nAtoms, int const* elements, double const* positions, const char* xyzFileName);
inline void writeStructureToFile(const std::vector<int>& elements, const std::vector<double>& positions,
                                 const std::string& xyzFileName) {
  int nAtoms = static_cast<int>(elements.size());
  writeStructureToFile(nAtoms, elements.data(), positions.data(), xyzFileName.c_str());
}

/*!
 * Quantum chemical method
 */
enum class Method { dftb, pm6 };

/*!
 * This class allows to do single-point calculations with the StatPhysQCLib library.
 * Since the c++ ABI is compiler-specific, non-inline functions must use POD types.
 * Still, to allow using std::vector, inline functions are defined in this header.
 */
class STATPHYSQC_EXPORT QuantumChemicalCalculation {
 public:
  explicit QuantumChemicalCalculation(Method method, const std::string& parameterDirectory = "Parameters");
  explicit QuantumChemicalCalculation(Method method, const char* parameterDirectory);
  ~QuantumChemicalCalculation();

  void initialize(const std::vector<int>& elementTypes);
  void initialize(int nAtoms, int const* elementTypes);

  void setPositionsInAngstrom(const std::vector<double>& positions);
  void setPositionsInAngstrom(double const* positions);

  void calculate();

  double getEnergyInEv() const;

  std::vector<double> getForcesInEvPerAngstrom() const;
  void getForcesInEvPerAngstrom(double* forces) const;

  double getBondOrderBetweenAtoms(int atom1, int atom2) const;

 private:
  struct Impl;
  std::unique_ptr<Impl> pImpl_;
  int atomCount() const;
};

inline QuantumChemicalCalculation::QuantumChemicalCalculation(Method method, const std::string& parameterDirectory)
  : QuantumChemicalCalculation(method, parameterDirectory.c_str()) {
}

inline void QuantumChemicalCalculation::initialize(const std::vector<int>& elementTypes) {
  initialize(static_cast<int>(elementTypes.size()), elementTypes.data());
}

inline void QuantumChemicalCalculation::setPositionsInAngstrom(const std::vector<double>& positions) {
  setPositionsInAngstrom(positions.data());
}

inline std::vector<double> QuantumChemicalCalculation::getForcesInEvPerAngstrom() const {
  std::vector<double> forces(3 * atomCount());
  getForcesInEvPerAngstrom(forces.data());
  return forces;
}

#endif // STATPHYSQC_HPP