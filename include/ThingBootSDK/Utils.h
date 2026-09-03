#ifndef THINGBOOT_SDK_UTILS_H
#define THINGBOOT_SDK_UTILS_H

#include <Arduino.h>
#include "JSON.h"

// 将 JSONVar 转换为 String
// Convert JSONVar to String
inline String CS(JSONVar var)
{
    String t = JSON.typeof(var);
    if (t == "null")        return "";
    if (t == "boolean")     return var ? "1" : "0";
    if (t == "string")      return (const char*)var;
    return JSON.stringify(var);
}

// 将 JSONVar 转换为 const char*
// Convert JSONVar to const char*
#define C(var) (CS(var).c_str())

// 将 JSONVar 转换为 int
// Convert JSONVar to int
inline int CN(JSONVar var)
{
    return CS(var).toInt();
}

// 将 JSONVar 转换为 long
// Convert JSONVar to long
inline long CL(JSONVar var)
{
    return strtol(C(var), NULL, 10);
}

// 拼接多个值为 String（每个参数会先经 CS() 转换）
// Concatenate multiple values into a String (each argument is converted via CS())
#define _CAT_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME

#define _CAT_1(a)                               CAT_impl(CS(a))
#define _CAT_2(a, b)                            CAT_impl(CS(a), CS(b))
#define _CAT_3(a, b, c)                         CAT_impl(CS(a), CS(b), CS(c))
#define _CAT_4(a, b, c, d)                      CAT_impl(CS(a), CS(b), CS(c), CS(d))
#define _CAT_5(a, b, c, d, e)                   CAT_impl(CS(a), CS(b), CS(c), CS(d), CS(e))
#define _CAT_6(a, b, c, d, e, f)                CAT_impl(CS(a), CS(b), CS(c), CS(d), CS(e), CS(f))
#define _CAT_7(a, b, c, d, e, f, g)             CAT_impl(CS(a), CS(b), CS(c), CS(d), CS(e), CS(f), CS(g))
#define _CAT_8(a, b, c, d, e, f, g, h)          CAT_impl(CS(a), CS(b), CS(c), CS(d), CS(e), CS(f), CS(g), CS(h))
#define _CAT_9(a, b, c, d, e, f, g, h, i)       CAT_impl(CS(a), CS(b), CS(c), CS(d), CS(e), CS(f), CS(g), CS(h), CS(i))
#define _CAT_10(a, b, c, d, e, f, g, h, i, j)   CAT_impl(CS(a), CS(b), CS(c), CS(d), CS(e), CS(f), CS(g), CS(h), CS(i), CS(j))

#define CAT(...) \
    _CAT_GET_MACRO(__VA_ARGS__, _CAT_10, _CAT_9, _CAT_8, _CAT_7, _CAT_6, _CAT_5, _CAT_4, _CAT_3, _CAT_2, _CAT_1)(__VA_ARGS__)

template<typename... Args>
String CAT_impl(Args&&... args)
{
    String s = "";
    ((s.concat(args)), ...);
    return s;
}

// 重复字符串 times 次
// Repeat a string times
inline String str_repeat(String str, uint8_t times)
{
    String result;
    result.reserve(str.length() * times);
    for (uint8_t i = 0; i < times; i++)
    {
        result += str;
    }
    return result;
}

// 按指定长度填充字符串
// left=true 时在左侧填充（默认），left=false 时在右侧填充
inline String str_pad(String str, uint8_t len, const char* fill = " ", bool left = false)
{
    uint8_t str_len = str.length();

    if (str_len >= len || fill == nullptr || fill[0] == '\0')
    {
        return String(str);
    }

    String pad;
    uint8_t need = len - str_len;
    pad.reserve(need);

    uint8_t fill_len = strlen(fill);

    // 循环填充，避免最后一次超出目标长度
    while (pad.length() + fill_len <= need)
    {
        pad += fill;
    }

    // 补齐剩余长度
    uint8_t remain = need - pad.length();
    if (remain > 0)
    {
        pad += String(fill).substring(0, remain);
    }

    if (left)
    {
        return pad + str;
    }
    else
    {
        return str + pad;
    }
}

// 在字符串中提取两个子串之间的内容
// Extract content between two substrings
inline String str_find(String str, String begin, String end)
{
    int p1 = str.indexOf(begin);
    int p2 = 0;

    if (p1 >= 0)
    {
        p2 = str.indexOf(end, p1 + begin.length());

        if (p2 >= 0)
        {
            return str.substring(p1 + begin.length(), p2);
        }
    }

    return "";
}

// 判断字符串是否以指定后缀结尾
// Check whether string ends with the specified suffix
inline bool str_ends_with(const char* str, const char* suffix)
{
    if (str == NULL || suffix == NULL) return false;

    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    if (str_len < suffix_len) return false;

    const char* p = str + str_len - suffix_len;
    while (*suffix != '\0')
    {
        if (*p != *suffix) return false;
        p++;
        suffix++;
    }

    return true;
}

// 按分隔符拆分字符串
// Split string by separator
inline uint8_t str_split(const String &temp, const char* sep, String *out, uint8_t max_count)
{
    uint8_t cnt = 0;
    int start = 0;
    uint8_t sep_len = strlen(sep);

    while (cnt < max_count)
    {
        int idx = temp.indexOf(sep, start);

        if (idx == -1)
        {
            out[cnt++] = temp.substring(start);
            break;
        }

        out[cnt++] = temp.substring(start, idx);
        start = idx + sep_len;
    }

    return cnt;
}

// 字符串转十六进制字符串
// Convert string to hex string
inline String str2hex(String str)
{
    String ret = "";

    for (uint16_t i = 0; i < str.length(); i++)
    {
        ret += String(str[i], HEX);
    }

    return ret;
}

// 十六进制字符串转字符串
// Convert hex string to string
inline String hex2str(String hex)
{
    char str[hex.length() / 2 + 1];
    int idx = 0;

    for (uint16_t i = 0; i < hex.length(); i += 2)
    {
        String hexByte = hex.substring(i, i + 2);
        char byte = (char) strtol(hexByte.c_str(), NULL, 16);
        str[idx++] = byte;
    }

    str[idx] = '\0';

    if (str2hex(str) != hex)
    {
        return hex;
    }
    else
    {
        return String(str);
    }
}

// 计算从某个 millis() 时间点到当前时刻经过的毫秒数（自动处理溢出）
// Calculate elapsed milliseconds since a given millis() timestamp (handles overflow)
inline uint32_t mill_elapsed(uint32_t time)
{
    if (millis() >= time)
    {
        return millis() - time;
    }
    else
    {
        return 4294967295 - time + millis() + 1;
    }
}

// 生成指定范围内的随机数
// Generate a random number in the specified range
inline int rand_num(int begin, int end)
{
    srand((unsigned int)time(NULL));
    return rand() % end + begin;
}

// MD5 加密
// MD5 hash
inline String md5(String data, uint8_t len = 32)
{
    MD5Builder md5;

    md5.begin();
    md5.add(data.c_str());
    md5.calculate();

    uint8_t begin = (32 - len) / 2;
    return md5.toString().substring(begin, begin + len);
}

// 复制字符串到目标缓冲区（目标缓冲区需有足够容量；str2 为 nullptr 时目标置空串）
// Copy string to destination buffer (destination must have enough capacity;
// a nullptr str2 clears the destination to an empty string)
void str_copy(char* str1, String str2);
void str_copy(char* str1, const char* str2);
void str_copy(char* str1, JSONVar str2);

#endif
