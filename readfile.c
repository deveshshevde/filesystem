
#include "fs.h"

status read_file(FILE* file, const char* file_name) {
    inode_t node;
    fseek(file, SECTOR_SIZE, SEEK_SET);
    for (int i = 0; i < NUM_INODES; i++) {
        fread(&node, sizeof(node), 1, file);
        if (strncmp(node.name_of_the_file, file_name, 16) == 0) {
            char *data = malloc(node.size_of_file_in_bytes + 1);
            fseek(file, node.location_on_the_disk_from_where_os_will_read * SECTOR_SIZE, SEEK_SET);
            fread(data, sizeof(char), node.size_of_file_in_bytes, file);
            data[node.size_of_file_in_bytes] = '\0';
            puts(data);
            free(data);
            return OK;
        }
    }
    puts("File not found");
    return NOT_OK;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <image file> <file name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r+b");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    if (read_file(file, argv[2]) == OK) {
        puts("File read successfully");
    } else {
        puts("Failed to read file");
    }

    fclose(file);
    return EXIT_SUCCESS;
}
