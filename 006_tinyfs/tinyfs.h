//tinyfs.h
#ifndef TINYFS_H
#define TINYFS_H
 
 
#define MAXLEN 8         //文件名最多8个字节
#define MAX_FILES  32    //定义文件最大32个字节
#define MAX_BLOCKSIZE  512 //文件及目录文件一共的个数
 
//定义一个目录项格式
struct dir_entry{
 
    char filename[MAXLEN];
    uint8_t idx;
};
 
//文件块数据结构
struct file_blk{
    uint8_t busy;  
    mode_t mode;   
    uint8_t idx;  
 
    union 
    {
        /* data */
        uint8_t file_size;    //文件大小
        uint8_t dir_children;  //文件夹中文件个数
    };
    char data[0];  //保存数据所在位置的指针
 
};
 
//下面的block数组所占据的连续内存就是我的tinyfs.h的介质，每一个元素代表一个文件
//struct file_blk bolck[MAX_FILES+1];
#endif
