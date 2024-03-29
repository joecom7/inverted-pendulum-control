#include "CsvLogger.hpp"

//#define LOGGING_DISABLED TRUE

CsvLogger::CsvLogger(const std::string filename) : FILENAME(filename.c_str())
{
    file.open(filename);
    if (!file.is_open())
    {
        printf("impossibile aprire il file di log. esco...");
        exit(1);
    }
    file.precision(16);
    file << "Time,Encoder,Ctrl_vel,x,EncoderPunto,PosPunto\n";
}

CsvLogger::~CsvLogger()
{
    flush();
    file.close();
}

void CsvLogger::flush()
{
    file.flush();
}

CsvLogger &CsvLogger::operator<<(const double new_val)
{
#ifndef LOGGING_DISABLED
    file << std::scientific << new_val << ',';
#endif
    return *this;
}

void CsvLogger::end_row()
{
#ifndef LOGGING_DISABLED
    file << '\n';
#endif
}

void CsvLogger::close()
{
    flush();
    file.close();
}