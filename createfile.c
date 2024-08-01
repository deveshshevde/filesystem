#include "fs.h"


int find_free_inode(FILE* file) {
    inode_t base_inode;
    fseek(file, SECTOR_SIZE, SEEK_SET);
    for (int i = 0; i < NUM_INODES; i++) {
        fread(&base_inode, sizeof(inode_t), 1, file);
        if (base_inode.location_on_the_disk_from_where_os_will_read == -1) {
            return i;
        }
    }
    return -1;
}

status create_file(FILE* file, const char* file_name, const char* file_content) {
    int inode_index = find_free_inode(file);
    if (inode_index == -1) {
        puts("No free inodes available");
        return NOT_OK;
    }

    inode_t node;
    strcpy(node.name_of_the_file, file_name);
    node.size_of_file_in_bytes = strlen(file_content);
    node.location_on_the_disk_from_where_os_will_read = inode_index + DATA_BLOCK_START;

    fseek(file, SECTOR_SIZE + inode_index * INODE_SIZE, SEEK_SET);
    fwrite(&node, sizeof(inode_t), 1, file);

    fseek(file, node.location_on_the_disk_from_where_os_will_read * SECTOR_SIZE, SEEK_SET);
    fwrite(file_content, sizeof(char), node.size_of_file_in_bytes, file);

    return OK;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <image file> <file name> <file content>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r+b");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    if (create_file(file, argv[2], argv[3]) == OK) {
        puts("File created successfully");
    } else {
        puts("Failed to create file");
    }

    fclose(file);
    return EXIT_SUCCESS;
}
