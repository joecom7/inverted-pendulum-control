#ifndef CSV_LOGGER_H
#define CSV_LOGGER_H

#include <iostream>
#include <fstream>

class CsvLogger {
    private:
        const char* FILENAME;
        std::ofstream file;
    public:
        CsvLogger(const std::string filename);
        ~CsvLogger();
        void flush();
        CsvLogger& operator << (const double new_val);
        void end_row();
        void close();
};

#endif