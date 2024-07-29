#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    * Tutorial for all the concepts for revision purpose :
    * https://www.youtube.com/watch?v=NgUYYC6lTws -> fread , fwrite
*/

/*
    TODO :  Superblock (Block 0) -> Inodes (Block 1 - Block n) -> Data Blocks (Block n+1 - End) 
*/


typedef enum {
    OK,
    NOT_OK,
}status;
 



#define SECTOR_SIZE   512 // This is a chunck of block to store data 
#define NUMBER_OF_SECTOR 20480 // I am assuming i have 10 MB of x.img !!

//https://www.youtube.com/watch?v=6KjMlm8hhFA
/*
    * You can see the inode using ls -i command 
    * each file have their own inode number so ya i have to do the same!!
*/
#define NUM_INODES 128
#define INODE_SIZE 128  // Each inode stores the attributes and disk block locations of the object's data


#define DATA_BLOCK_START (NUM_INODES * INODE_SIZE / SECTOR_SIZE + 1)


/*
    * In unix based system ```stat``` command is used to get information from inode , btw is a system call
    * This is how it looks
    * 16777230 33307578 -rw-r--r-- 1 deveshshevde staff 0 762 "Jul 29 12:32:33 2024" "Jul 29 12:32:33 2024" "Jul 29 12:32:33 2024" "Jul 29 12:16:43 2024" 4096 8 0 filesystem.c
    * ```dh -i``` gives the information about inode usage

*/
typedef struct
{
    int location_on_the_disk_from_where_os_will_read;
    uint16_t size_of_file_in_bytes;
    uint16_t reference_count_of_the_file;
    char name_of_the_file[16];

}inode_t;


/*
    * For superblock i referred this lecture its in hindi so..
    * https://www.youtube.com/watch?v=6uA9zdKEIbA
*/


/*
* your C program should take this file as input and then format it to be a file system. 
* the design of this filesystem is upto you. 
* Next the C program should allow you to name a file and then write content to it.
* this file should be saved in the filesystem u created on x.img
* the program should also allow reading this file from x.img
*/

typedef struct {
    int total_number_of_inodes;
    int number_of_data_block_present;
} superblock_t;

status format_filesystem(FILE* file)
{

    // Create a instance of the super block and fill the appropioate content
    // in some other fs like zfs content and other allocation is dynamic in nature
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

/*
    * Before making a new file we have to check whether we have free inodes or not
    * if we have free inodes we can create a new file
    * else we have to delete some file so that corresponding inode will get delete!
*/

int find_free_inode(FILE* file){
    inode_t base_inode;

    // Move to the start of the inode section (block 1)
    fseek(file, SECTOR_SIZE, SEEK_SET);

    for(int i = 0; i < NUM_INODES; i++){

        fread(&base_inode, sizeof(inode_t), 1, file);
        if(base_inode.location_on_the_disk_from_where_os_will_read == -1){
            return i;
        }
    }
    return -1;
}


status create_file(FILE* file , char* file_name , char* file_content){
    // first we need to find whether we can create file or not so lets check inode 

    if(find_free_inode(file) == -1){
        puts("First delete some files then try again");
        return NOT_OK;
    }

    inode_t node;
    // initialize inode for the new file -> gives info about the file
    strcpy(node.name_of_the_file, file_name);
    node.size_of_file_in_bytes = strlen(file_content);
    node.location_on_the_disk_from_where_os_will_read = find_free_inode(file) + DATA_BLOCK_START;
    // cursor will the specific point and start writing the content
    // Writing Inode Metadata
    fseek(file,(find_free_inode(file)*INODE_SIZE)+SECTOR_SIZE,SEEK_SET); 
    fwrite(&node,sizeof(inode_t),1,file);
    // Writing Data to data block
    fseek(file,node.location_on_the_disk_from_where_os_will_read*SECTOR_SIZE,SEEK_SET);
    fwrite(file_content,sizeof(char),node.size_of_file_in_bytes,file);
    return OK;  
}

status read_file(FILE* file , char* file_name){

    inode_t node;
    fseek(file, SECTOR_SIZE, SEEK_SET);
    for(int i = 0; i < NUM_INODES; i++){
        fread(&node, sizeof(node), 1, file);
        if(strncmp(node.name_of_the_file, file_name, 16) == 0){
            // LOL i never thought these things are used here !! great level of confdence after doing this
            char *data = malloc(node.size_of_file_in_bytes + 1);
            fseek(file, node.location_on_the_disk_from_where_os_will_read*SECTOR_SIZE, SEEK_SET);
            fread(data, sizeof(char), node.size_of_file_in_bytes, file);
            puts(data);
            free(data);
            return OK;
        }
    }
    return OK;
}

int main()
{

    // b is needed to avoid extra character , this thing i read somewhere
    FILE *file = fopen("x.img", "r+b");
    if(file == NULL){
        puts("cant open file");
    }
    else{
        format_filesystem(file);
        puts("File system formatted successfully");
    }
    create_file(file, "file1", "hello world from file1");
    read_file(file, "file1");
    return 0;
}

