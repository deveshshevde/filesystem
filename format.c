#include "fs.h"


status format_filesystem(FILE* file) {
    superblock_t superblock = {
        .total_number_of_inodes = NUM_INODES,
        .number_of_data_block_present = NUMBER_OF_SECTOR - DATA_BLOCK_START
    };
    fseek(file, 0, SEEK_SET);
    fwrite(&superblock, sizeof(superblock_t), 1, file);
    inode_t inode = {
        .location_on_the_disk_from_where_os_will_read = -1,
        .size_of_file_in_bytes = 0,
        .reference_count_of_the_file = 0,
        .name_of_the_file = ""
    };
    fseek(file, SECTOR_SIZE, SEEK_SET);
    for (int i = 0; i < NUM_INODES; i++) {
        fwrite(&inode, sizeof(inode), 1, file);
    }
    return OK;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <image file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r+b");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    if (format_filesystem(file) == OK) {
        puts("File system formatted successfully");
    } else {
        puts("Failed to format file system");
    }

    fclose(file);
    return EXIT_SUCCESS;
}