#include "TrajectoryFileWriter.h"
#include "BinaryIO.h"
#include "CoordinatesAndVelocitiesInitializer.h" // For MAXTITLE value
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <utility>
#include <StatPhysQC.hpp>

using namespace std;

TrajectoryFileWriter::TrajectoryFileWriter(const MDParameters &parameters,
                                           std::string finalCoordFilename,
                                           std::string trajFilename)
  : par(parameters),
    finalCoordinatesFilename(std::move(finalCoordFilename)),
    trajectoryCoordinatesFilename(std::move(trajFilename)) {
}

void TrajectoryFileWriter::writeBeforeRun() {
    ofstream fout1; // trajectory output
    if (par.trajectoryOutput) {
        fout1.open(trajectoryCoordinatesFilename, ios::out);
        if (fout1.bad()) {
            throw std::runtime_error("can't open " + trajectoryCoordinatesFilename);
        }
        fout1 << par.title << endl;
    }
}

void TrajectoryFileWriter::writeFinalCoordinates(const std::vector<double>& positions,
                                                 const std::vector<double>& velocities) {
    if (par.finalXVOutput == FinalCoordinateFileFormat::ascii) {
        writeFinalCoordinatesInAsciiForm(positions, velocities);
    }
    else {
        writeFinalCoordinatesInBinaryForm(positions, velocities);
    }
}

void TrajectoryFileWriter::writeFinalCoordinatesInBinaryForm(const std::vector<double>& positions,
                                                             const std::vector<double>& velocities) {
    ofstream fout2;
    fout2.open(finalCoordinatesFilename, ios::out | ios::binary);
    if (fout2.bad()) {
        throw std::runtime_error("can't open " + finalCoordinatesFilename);
    }
    fout2.write(par.title.c_str(), MAXTITLE);
    BinaryIO::write(fout2, positions);
    BinaryIO::write(fout2, velocities);
}

void TrajectoryFileWriter::writeFinalCoordinatesInAsciiForm(const std::vector<double>& positions,
                                                            const std::vector<double>& velocities) {
    /*ofstream fout2;
    fout2.open(finalCoordinatesFilename, ios::out);
    if (fout2.bad()) {
        throw std::runtime_error("can't open " + finalCoordinatesFilename);
    }
    fout2 << par.title << "\n" ;
    fout2 << par.numberAtoms << "\n" ;
    for (int j = 0; j < par.numberAtoms; j++) {
        fout2 << setw(6) << j;
        for (int m = 0; m < 3; m++) {
            fout2 << setw(15) << positions[3 * j + m];
        }
        for (int m = 0; m < 3; m++) {
            fout2 << setw(15) << velocities[3 * j + m];
        }
        fout2 << "\n";
    }*/
    std::vector<double> pos_xyz = positions;
    std::vector<int> elements = {1,1,6,6,6,6,1,1,1,1,6,6,1,1,1,1};
    for (size_t i = 0; i < pos_xyz.size(); i++) {
      pos_xyz[i]*=10;
    }
    writeStructureToFile(elements,pos_xyz,"final.xyz");

}

void TrajectoryFileWriter::writeOutTrajectoryStep(const std::vector<double>& positions) {
    if (par.trajectoryOutput) {
        if (par.trajectoryOutputFormat == TrajectoryFileFormat::binary) {
            writeOutTrajectoryStepInBinaryForm(positions);
        } else if (par.trajectoryOutputFormat == TrajectoryFileFormat::ascii) {
            writeOutTrajectoryStepInAsciiForm(positions);
        }
    }
}

void TrajectoryFileWriter::writeOutTrajectoryStepInBinaryForm(const std::vector<double>& positions) {
    ofstream fileBW;
    fileBW.open(trajectoryCoordinatesFilename, ios::out | ios::app | ios::binary);
    if (fileBW.bad()) {
        throw runtime_error("I/O ERROR: cannot write to file: " + trajectoryCoordinatesFilename);
    }
    BinaryIO::write(fileBW, positions);
}

void TrajectoryFileWriter::writeOutTrajectoryStepInAsciiForm(const std::vector<double>& positions) {
    /*ofstream fileFW;
    fileFW.open(trajectoryCoordinatesFilename, ios::out | ios::app);
    if (fileFW.bad()) {
        throw runtime_error("I/O ERROR: cannot write to file: " + trajectoryCoordinatesFilename);
    }
    int ntot = 3 * par.numberAtoms;
    for (int i = 0; i < ntot; i += 10) {
        for (int j = i; (j < i + 10 && j < ntot); j++) {
            fileFW << setw(10) << positions[j];
        }
        fileFW << endl;
    }*/
    
    std::vector<double> pos_xyz = positions;
    std::vector<int> elements = {1,1,6,6,6,6,1,1,1,1,6,6,1,1,1,1};
    for (size_t i = 0; i < pos_xyz.size(); i++) {
      pos_xyz[i]*=10;
    }
    writeStructureToFile(elements,pos_xyz,"traj.xyz");

    //https://www.geeksforgeeks.org/cpp-program-to-append-content-of-one-text-file-to-another/
    fstream file;
     // Input stream class to 
    // operate on files. 
    ifstream ifile("traj.xyz", ios::in); 
  
    // Output stream class to 
    // operate on files. 
    ofstream ofile("movement.xyz", ios::out | ios::app); 
    
    // check if file exists 
    if (!ifile.is_open()) { 
  
        // file not found (i.e, not opened). 
        // Print an error message. 
        cout << "file not found"; 
    } 
    else { 
        // then add more lines to 
        // the file if need be 
        ofile << ifile.rdbuf(); 
    } 
    string word; 
  
    // opening file 
    file.open("file2.txt"); 
  
    // extracting words form the file 
    while (file >> word) { 
  
        // displaying content of 
        // destination file 
        cout << word << " "; 
    } 
}
