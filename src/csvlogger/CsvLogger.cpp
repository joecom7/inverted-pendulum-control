#include "CsvLogger.hpp"

CsvLogger::CsvLogger(const std::string filename) :FILENAME(filename.c_str()){
    file.open(filename);
    if (!file.is_open()) {
        printf("impossibile aprire il file di log. esco...");
        exit(1);
    }
    file.precision(16);
    file << "Time,Encoder,Ctrl_vel,Ctrl_vel_robot_timestamp,Ctr_vel_robot,Vel_timestamp,Vel,Pos_timestamp,Pos,\n";
}

CsvLogger::~CsvLogger() {
    flush();
    file.close();
}

void CsvLogger::flush() {
    file.flush();
}

CsvLogger& CsvLogger::operator << (const double new_val) {
    file << std::scientific << new_val << ',';
    return *this;
}

void CsvLogger::end_row() {
    file << '\n';
}

void CsvLogger::close() {
    flush();
    file.close();
}