#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SECTOR_SIZE   512
#define NUMBER_OF_SECTOR 20480
#define NUM_INODES 128
#define INODE_SIZE 128
#define DATA_BLOCK_START (NUM_INODES * INODE_SIZE / SECTOR_SIZE + 1)

typedef struct {
    int total_number_of_inodes;
    int number_of_data_block_present;
} superblock_t;

typedef struct {
    int location_on_the_disk_from_where_os_will_read;
    uint16_t size_of_file_in_bytes;
    uint16_t reference_count_of_the_file;
    char name_of_the_file[16];
} inode_t;

typedef enum {
    OK,
    NOT_OK,
} status;

