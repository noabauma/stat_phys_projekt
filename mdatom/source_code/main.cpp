#include <MDSimulation.h>
#include <iostream>
#include <StatPhysQC.hpp>

/*!
 * main function.
 * The program expects one argument for the input file (parameters), an one optional argument for
 * the  file with the initial coordinates (as .xyz file !!!!)
 */

int main(int argc, char* argv[]) {
    /*switch (argc) {
        case 2: break;
        case 3: break;
        default:
        std::cerr << "Usage: mdatom input_file [coordinate_file] > output \n";
            return 1;
    }*/

    std::string parameterFile = argv[1];
    std::string coordinatesFile; // NB: might be empty
    double velocities;

    if (argc > 2) {
        coordinatesFile = argv[2];
        velocities = std::stod(argv[3]);
      }

    auto structure = readStructureFromFile(coordinatesFile); //pair of vectors

    for (int num = 0; num < structure.second.size(); num++) {
      structure.second[num] =  structure.second[num]*0.1; //Transfer from Angstrom to nm
    }

    MDSimulation md(std::cout);
    try {
        md.performSimulation(parameterFile, structure, velocities);
    }
    catch (std::exception& e) {
        std::cerr << e.what();
        return 1;
    }
    return 0;
}
