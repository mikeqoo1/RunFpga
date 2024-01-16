#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
const int MAX_TAGS = 50;         // 設定 TAG 最大數量
const int MAX_TAG_LENGTH = 200;   // 設定 TAG 最大內容長度
const int MAX_VALUE_LENGTH = 200; // 設定 VALUE 最大內容長度

size_t my_strlen(const char* str);

void my_strncpy(char* dest, const char* src, size_t count);

void parsemsg(char *input,
              char tags[MAX_TAGS][MAX_TAG_LENGTH],
              char values[MAX_TAGS][MAX_VALUE_LENGTH],
              int &numTags);
