/**
 * @file   CheckCastNoThrow.h
 * @author Boby Wolf <wangxiaobo@sogou-inc.com>
 * @date   Mon Jan 30 16:06:35 2012
 *
 * @brief  类型转换检查
 *
 *
 */
#ifndef _CHECKCASTNOTHROW_H_
#define _CHECKCASTNOTHROW_H_

#include <string>

namespace gsl
{
/**
 * @brief check_cast<>模板函数
 *
 * 提供对C/C++各原生类型提供带上下溢出检查功能模板函数。使用方式与static_cast<>完全相同。
 *
 * 目前支持所有有符号数值类型、无符号数值类型、
 * 浮点数类型中任意两类型的双向转换以及C风格字符串（const char *）
 * 到以上各类型的单向转换。不考虑浮点数转换成浮点数类型的数值溢出问题。
 *
 */
/**
 * @brief 类型检查
 *
 * @param value [in] 被转换的值
 * @param dest [in/out] 目标值
 *
 * @return 0-转换成功
 *         1-表示有错误但是可接受，如string结尾的无效字符转换为int
 *         -1-转换失败
 * @author Boby Wolf
 * @date   Mon Jan 30 16:08:44 2012
 */
template <typename DestType, typename SrcType> int check_cast(SrcType value, DestType & dest);

/**
 * @brief check_cast<>模板函数在DestType == SrcType时的偏特化版本
 *
 * @param value [in] 被转换值
 * @param dest [in/out] 转换目标
 *
 * @return 0
 * @author Boby Wolf
 * @date   Mon Jan 30 16:13:06 2012
 */
template <typename DestType> inline int check_cast(DestType value, DestType & dest)
{
    dest = value;
    return 0;
}

template <> inline int check_cast <signed char, long>(long value, signed char &dest)
{
    if (value < -128L)
    {
        return -1;
    }
    if (value > 127L)
    {
        return -1;
    }
    dest = static_cast <signed char>(value);
    return 0;
}

template <> inline int check_cast <unsigned char, long>(long value, unsigned char &dest)
{
    if (value < 0L)
    {
        return -1;
    }
    if (value > 255L)
    {
        return -1;
    }
    dest = static_cast <unsigned char>(value);
    return 0;
}

template <> inline int check_cast <short, long>(long value, short &dest)
{
    if (value < -32768L)
    {
        return -1;
    }
    if (value > 32767L)
    {
        return -1;
    }
    dest = static_cast <short>(value);
    return 0;
}

template <> inline int check_cast <int, long>(long value, int &dest)
{
    dest = static_cast <int>(value);
    return 0;
}

template <>
inline int check_cast <unsigned short, unsigned long>(unsigned long value,
                                                      unsigned short &dest)
{
    if (value > 65535UL)
    {
        return -1;
    }
    dest = static_cast <unsigned short >(value);
    return 0;
}

template <>
inline int check_cast <unsigned int, unsigned long>(unsigned long value,
                                                    unsigned int &dest)
{
    dest = static_cast <unsigned int>(value);
    return 0;
}

//from c-style string
template <> inline int check_cast <char, const char *>(const char *s, char &dest)
{
    if (!s)
    {
        return -1;
    }
    dest = s[0];
    return 0;
}

template <> inline int check_cast <long, const char *>(const char *s, long &dest)
{
    char* end = NULL;
    dest = strtol(s, &end, 10);
    return 0;
}

//signed char is used as int8
template <> inline int check_cast <signed char, const char *>(const char *s, signed char &dest)
{
    long v = 0;
    int err = check_cast <long>(s, v);
    if (err >= 0)
    {
        int e = check_cast <signed char>(v, dest);
        //forward the error number
        return e == 0 ? err : e;
    }
    return err;
}

//unsigned char is used as uint8
template <>
inline int check_cast <unsigned char, const char *>(const char *s, unsigned char &dest)
{
    long v = 0;
    int err = check_cast <long>(s, v);
    if (err >= 0)
    {
        int e = check_cast <unsigned char>(v, dest);
        return e == 0 ? err : e;
    }
    return err;
}

template <> inline int check_cast <short, const char *>(const char *s, short &dest)
{
    long v = 0;
    int err = check_cast <long>(s, v);
    if (err >= 0)
    {
        int e = check_cast <short>(v, dest);
        return e == 0 ? err : e;
    }
    return err;
}

template <> inline int check_cast <int, const char *>(const char *s, int &dest)
{
    long v = 0;
    int err = check_cast <long>(s, v);
    if (err >= 0)
    {
        int e = check_cast <int>(v, dest);
        return e == 0 ? err : e;
    }
    return err;
}


template <> inline int check_cast <long long, const char *>(const char *s, long long &dest)
{
    char* end = NULL;
    dest = strtoll(s, &end, 10);
    return 0;
}

template <> inline int check_cast <unsigned long, const char *>(const char *s, unsigned long &dest)
{
    char* end = NULL;
    dest = strtoul(s, &end, 10);
    return 0;
}

template <>
inline int check_cast <unsigned short, const char *>(const char *s, unsigned short &dest)
{
    unsigned long v = 0;
    int err = check_cast <unsigned long>(s, v);
    if (err >= 0)
    {
        int e = check_cast <unsigned short>(v, dest);
        return e == 0 ? err : e;
    }
    return err;
}

template <>
inline int check_cast <unsigned int, const char *>(const char *s, unsigned int &dest)
{
    unsigned long v = 0;
    int err = check_cast <unsigned long>(s, v);
    if (err >= 0)
    {
        int e = check_cast <unsigned int>(v, dest);
        return e == 0 ? err : e;
    }
    return err;
}


template <>
inline int check_cast <unsigned long long, const char *>(const char *s, unsigned long long &dest)
{
    char* end = NULL;
    dest = strtoull(s, &end, 10);
    return 0;
}

template <> inline int check_cast <float, const char *>(const char *s, float &dest)
{
    char* end = NULL;
    dest = strtof(s, &end);
    return 0;
}

template <> inline int check_cast <double, const char *>(const char *s, double &dest)
{
    char* end = NULL;
    dest = strtod(s, &end);
    return 0;
}

template <> int check_cast <long double, const char *>(const char *s, long double &dest);

template <> inline int check_cast <char, char *>(char *s, char &dest)
{
    if (!s)
    {
        return -1;
    }
    dest = s[0];
    return 0;
}

template <> inline int check_cast <long, char *>(char *s, long &dest)
{
    return check_cast <long, const char *>(s, dest);
}

template <> inline int check_cast <short, char *>(char *s, short &dest)
{
    long v = 0;
    int err = check_cast <long, const char *>(s, v);
    if (err >= 0)
    {
        int e = check_cast <short>(v, dest);
        return e == 0 ? err : e;
    }
    return err;
}

//signed char is used as int8
template <> inline int check_cast <signed char, char *>(char *s, signed char &dest)
{
    long v = 0;
    int err = check_cast <long, const char *>(s, v);
    if (err >= 0)
    {
        int e = check_cast <signed char>(v, dest);
        return e == 0 ? err : e;
    }
    return err;
}

//unsigned char is used as uint8
template <> inline int check_cast <unsigned char, char *>(char *s, unsigned char &dest)
{
    long v = 0;
    int err = check_cast <long, const char *>(s, v);
    if (err >= 0)
    {
        int e = check_cast <unsigned char>(v, dest);
        return e == 0 ? err : e;
    }
    return err;
}

template <> inline int check_cast <int, char *>(char *s, int &dest)
{
    long v = 0;
    int err = check_cast <long, const char *>(s, v);
    if (err >= 0)
    {
        int e = check_cast <int>(v, dest);
        return e == 0 ? err : e;
    }
    return err;
}


template <> inline int check_cast <long long, char *>(char *s, long long &dest)
{
    return check_cast <long long, const char *>(s, dest);
}

template <> inline int check_cast <unsigned long, char *>(char *s, unsigned long &dest)
{
    return check_cast <unsigned long, const char *>(s, dest);
}

template <> inline int check_cast <unsigned short, char *>(char *s, unsigned short &dest)
{
    unsigned long v = 0;
    int err = check_cast <unsigned long, const char *>(s, v);
    if (err >= 0)
    {
        int e = check_cast <unsigned short>(v, dest);
        return e == 0 ? err : e;
    }
    return err;
}

template <> inline int check_cast < unsigned int, char *>(char *s, unsigned int &dest)
{
    unsigned long v = 0;
    int err = check_cast <unsigned long, const char *>(s, v);
    if (err == 0)
    {
        int e = check_cast <unsigned int>(v, dest);
        return e == 0 ? err : e;
    }
    return err;
}


template <>
inline int check_cast <unsigned long long, char *>(char *s, unsigned long long &dest)
{
    return check_cast <unsigned long long, const char *>(s, dest);
}

template <> inline int check_cast <float, char *>(char *s, float &dest)
{
    return check_cast <float, const char *>(s, dest);
}

template <> inline int check_cast <double, char *>(char *s, double &dest)
{
    return check_cast <double, const char *>(s, dest);
}

template <> inline int check_cast <long double, char *>(char *s, long double &dest)
{
    return check_cast <long double, const char *>(s, dest);
}

template <> inline int check_cast <std::string, char *>(char *value, std::string & dest)
{
    dest = value;
    return 0;
}

template <>
inline int check_cast <std::string, const char *>(const char *value, std::string & dest)
{
    dest = value;
    return 0;
}
}

#endif /* _CHECKCASTNOTHROW_H_ */

