#pragma once

#include <ostream>

#include "Vector/Vector.h"
#include "Array/Array.h"

// The size which defines the number of characters in string for which to use static string
#define _STRING_OPTIMIZATION_SIZE_ 60

class String
{
public:
    using Iterator = _Iterator_Base<char>;

public:
    String() {}
    String(const char *_Str) { Append(_Str); }
    ~String() {}

    // Getters
    // Const
    inline const char *C_Str() const;
    inline const int Size() const { return _Size; }
    inline const int Capacity() const { return (_String.Capacity() + _STRING_OPTIMIZATION_SIZE_); }

    inline void Clear() { _Del(); }

    inline Iterator begin()
    {
        if (_Size > 60)
            return _String.begin();
        return _SmallString.begin();
    }

    inline Iterator end()
    {
        if (_Size > 60)
            return _String.end();
        return (_SmallString.begin() + _Size);
    }

    inline void Push_Back(const char _C)
    {
        if (_Size < 60)
            _SmallString.Push_Back(_C);
        else
            _String.Push_Back(_C);
        _Size++;
    }

    void Append(const char *_C);

    String SubStr(int _ipos, int _fpos);

    // Operators
    String &operator=(const char *_C);

    inline const char operator[](int i) const
    {
        if (_Size < 60)
            return _SmallString[i];
        return _String[i];
    }

    inline char operator[](int i)
    {
        if (_Size < 60)
            return _SmallString[i];
        return _String[i];
    }

    // For Printing
    friend std::ostream &operator<<(std::ostream &os, String &_String)
    {
        for (char &i : _String)
            os << i;

        return os;
    }

private:
    // The main Vector which stores the string
    Vector<char> _String;

    // The String optimization
    Array<char, _STRING_OPTIMIZATION_SIZE_> _SmallString;

    // The size of characters
    int _Size = 0;

    // Deletes the array
    void _Del();

    // Allows getting the Len of given char
    int _String_Len(const char *_Str);
};

inline const char *String::C_Str() const
{
    if (_Size < 60)
        return _SmallString.Data();
    return _String.Data();
}

inline void String::Append(const char *_C)
{
    // Checks if size is enough
    _Size = _String_Len(_C) + _Size;

    if (_Size > 60)
    {
        _String.Reserve(_Size);
        // Adds the characters
        for (int i = 0; i < _String_Len(_C); i++)
            _String.Push_Back(_C[i]);
    }

    for (int i = 0; i < _String_Len(_C); i++)
        _SmallString.Push_Back(_C[i]);
}

inline String String::SubStr(int _ipos, int _fpos)
{
    String _RString;

    for (int i = _ipos; i < _fpos; i++)
        _RString.Push_Back(this->operator[](i));

    return _RString;
}

inline String &String::operator=(const char *_C)
{
    _Size = _String_Len(_C);

    if (_Size > 60)
    {
        _String.Reserve(_Size);

        for (int i = 0; i < _Size; i++)
        {
            _String.Emplace_Back(_C[i]);
        }

        return *this;
    }

    for (int i = 0; i < _Size; i++)
    {
        _SmallString[i] = _C[i];
    }

    return *this;
}

inline void String::_Del()
{
    if (_Size > 60)
        _String.Flush();
    else
        _SmallString._Flush_();
    _Size = 0;
}

inline int String::_String_Len(const char *_Str)
{
    int Size = 0;
    while (_Str[Size] != '\0')
        Size++;
    return Size;
}