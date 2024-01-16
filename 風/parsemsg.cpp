#include "parsemsg.h"

size_t my_strlen(const char *str)
{
    size_t length = 0;
    while (str[length] != '\0')
    {
        ++length;
    }
    return length;
}

void my_strncpy(char *dest, const char *src, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        dest[i] = src[i];
    }
}

// 解析 KeyIn 電文
void parsemsg(char *input,
              char tags[MAX_TAGS][MAX_TAG_LENGTH],
              char values[MAX_TAGS][MAX_VALUE_LENGTH],
              int &numTags)
{
    numTags = 0;

    size_t start = 0;
    size_t i;
    for (i = 0; input[i] != '\0' && numTags < MAX_TAGS; ++i)
    {
        if (input[i] == '=')
        {
            // 遇到等號時，將 TAG 存入 tags 陣列
            size_t length = i - start;
            if (length < MAX_TAG_LENGTH)
            {
                //strncpy (tags [numTags], input + start, length); //HLS 不支援自己實作
                my_strncpy(tags[numTags], input + start, length);
                tags [numTags][length] = '\0'; // 確保以 null 結尾
            }
            start = i + 1;
        }
        else if (input[i] == '`')
        {
            // 遇到反引號時，將 VALUE 存入 values 陣列，並增加 numTags
            size_t length = i - start;
            if (length < MAX_VALUE_LENGTH)
            {
                //strncpy (values [numTags], input + start, length); //HLS 不支援自己實作
                my_strncpy(values[numTags], input + start, length);
                values [numTags][length] = '\0'; // 確保以 null 結尾
                numTags++;
            }
            start = i + 1;
        }
    }

    // 萬一最後一個 TAG 的值後面沒有帶到分隔符號就會少解析，所以針對最後一個值要做額外處理
    size_t length = my_strlen(input + start);
    if (length >= 0 && numTags < MAX_TAGS) {
        if (length < MAX_VALUE_LENGTH) {
            my_strncpy(values[numTags], input + start, length);
            values [numTags][length] = '\0'; // 確保以 null 結尾
            numTags++;
        }
    }
}

// 35=1`1=1300018`76=8450`117=V0001`55=1260  `38=1000`44=20`54=1`80024=20240112141234559`81001=0`81008=I`20000=192.168.103.90`50001=06`50003= `50002=110101`60005=N`20002=`