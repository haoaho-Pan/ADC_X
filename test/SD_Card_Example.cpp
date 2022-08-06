/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"

/* 列出指定目录下的文件 */
void listDir(FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);    //打开指定路径

    /* 判断是否成功打开目录 */
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }

    /* 判断此File对象代表的路径表示是不是文件夹，只有File对象代表路径存在且是一个目录时才返回true，否则返回false。*/
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }
    
    /* openNextFile() 返回当前目录下的下一个文件或者文件夹对象 */
    File file = root.openNextFile();

    while(file)
    {
        if(file.isDirectory())  //判断当前路径是目录？ 
        {
          Serial.print("  DIR : ");
          Serial.println(file.name());  //系统卷标文件
          if(levels)
          {
            listDir(fs, file.path(), levels -1);
          }
        } 
        else 
        {
          Serial.print("  FILE: ");
          Serial.print(file.name());
          Serial.print("  SIZE: ");
          Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    
    /*要检测是否存在的文件的名称*/
    if(fs.exists(path)) 
    {
        Serial.println("This directory has been!");
        return;
    }

    /* mkdir()  创建目录，如果目录创建成功，则返回true；否则，返回false*/
    if(fs.mkdir(path))
    {
      Serial.println(" Dir created");
    } 
    else 
    {
      Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);

     /*要检测是否存在的文件的名称*/
    if(!fs.exists(path)) 
    {
      Serial.println(" directory  does not exist!");
      return;
    }

    if(fs.rmdir(path))
    {
      Serial.println("Dir removed");
    } 
    else 
    {
      Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available())
    {
      Serial.write(file.read());   //Serial.print() 发送的是字符，Serial.write() 发送的是字节
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    /*open(filename，mode):filename：要打开的文件的名称，其中可以包含目录（以正斜杠/分隔）*/
    /* FILE_READ: 从文件的开头开始打开文件进行读取。*/
    /* FILE_WRITE: 从文件末尾开始打开文件并进行读写。*/
    File file = fs.open(path, FILE_WRITE); 

    if(!file)
    {
      Serial.println("Failed to open file for writing");
      return;
    }
    if(file.print(message))
    {
        Serial.println("File written");
    }
    else 
    {
        Serial.println("Write failed");
    }
    file.close();  //关闭文件
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file)
    {
      Serial.println("Failed to open file for appending");
      return;
    }
    if(file.print(message))
    {
      Serial.println("Message appended");
    } 
    else 
    {
      Serial.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) 
    {
      Serial.println("File renamed");
    } 
    else 
    {
      Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path))
    {
      Serial.println("File deleted");
    } else 
    {
      Serial.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

void setup(){
    Serial.begin(115200);
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();  //原始的标准容量（SDSC），大容量（SDHC），扩展容量（SDXC），超容量（SDUC）和SDIO

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    /*判断SD卡类型*/
    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }



    

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);  //获取SD卡容量
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

        // createDir(SD, "/mydir");
    // listDir(SD, "/", 0);
    // removeDir(SD, "/mydir");
    // listDir(SD, "/", 2);
    // writeFile(SD, "/hello.txt", "Hello ");
    // appendFile(SD, "/hello.txt", "World!\n");
    // readFile(SD, "/hello.txt");
    // deleteFile(SD, "/foo.txt");
    // renameFile(SD, "/hello.txt", "/foo.txt");
    // readFile(SD, "/foo.txt");
    // testFileIO(SD, "/test.txt");
    // Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    // Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop(){

}