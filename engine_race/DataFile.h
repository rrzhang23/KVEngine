#ifndef _ENGINE_RACE_DATAFILE_H_
#define _ENGINE_RACE_DATAFILE_H_

#include "util.h"
#include "config.h"

namespace polar_race {


extern string engineDir;

class DataFile {
public:
    DataFile(){}
    int id() {return _id;}
    unsigned long size() {return _size;}
    RetCode open(int n);
    RetCode write(const char* key, const char* value, const unsigned long stamp, unsigned long &offset);
    RetCode read(const unsigned long &offset, DataEntry *entry);
    RetCode readValue(const unsigned long &offset, char *value);
    RetCode last(int n, DataEntry *entry);
private:
    int _id = -1;
    int _fd = 0;
    unsigned long _size = 0;
    std::mutex _write_lock;
};
extern DataFile dataFile[DATA_FILE_NUM];

} // namespace polar_race


#endif