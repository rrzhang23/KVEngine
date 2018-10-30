// config.h
#ifndef _ENGINE_RACE_CONFIG_H_
#define _ENGINE_RACE_CONFIG_H_

#define MAX_ENTRY 67108864

// max memory in use (byte)
// 1.5GB
#define MAX_MEM 1610612736

// block in a memtable
// 64MB
#define MEM_BLOCK_SIZE 67108864

// memtable max size
// 500MB
#define MEMTABLE_MAX_SIZE 512000000


#define LOG_FILE_NAME "DBLOG"

#endif // _ENGINE_RACE_CONFIG_H_