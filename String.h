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
    String(const char *_Str) { this->operator=(_Str); }
    ~String() {}

    // Getters
    // Const
    inline const char *C_Str() const;
    inline const int Size() const { return _Size; }
    inline const int Capacity() const { return (_String.Capacity() + _STRING_OPTIMIZATION_SIZE_); }

    inline void Clear() { _Del(); }

    // Range based loops
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

    // Pushes a single char to end of string
    inline void Push_Back(const char _C)
    {
        // Limit of _SmallString reached so append to _String
        if (_Size == 60)
            _Copy_Dynamic_String_(true);

        if (_Size < 60)
            _SmallString.Push_Back(_C);
        else
            _String.Push_Back(_C);

        _Size++;
    }

    // Find Funcs

    // Returns the pos of given _C from _Pos if found else -1
    int Find(char _C, int _IPos = 0) { return Find(_C, _IPos, _Size); }
    // Returns the pos of given _C from _IPos to _FPos if found else -1
    int Find(char _C, int _IPos, int _FPos);
    // returns the pos of _C first occurence
    int Find_First_Of(char _C, int _Pos = 0) { return Find(_C, _Pos); }
    // returns the pos of _C Second occurence
    int Find_Not_First_Of(char _C, int _Pos = 0) { return Find(_C, Find(_C, _Pos) + 1); }
    // returns the pos of _C last occurence
    int Find_Last_Of(char _C, int _Pos = 0) { return _Find_Last(_C, _Size - 1, _Pos); }
    // returns the pos of _C Second last occurence
    int Find_Not_Last_Of(char _C, int _Pos = 0) { return _Find_Last(_C, Find_Last_Of(_C, _Pos) - 1, 0); }

    // Appends the _Str to _String
    void Append(const char *_C);

    // Returns a string which has char from _ipos to _fpos
    String SubStr(int _ipos, int _fpos);

    // Operators
    String &operator=(const char *_C);

    // Index operators
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
    // For Printing
    friend std::ostream &operator<<(std::ostream &os, String _String)
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

    // Returns the pos of _C from last
    int _Find_Last(char _C, int _Ipos, int _FPos);

    // If _Cond == true then will copy the contents os _SmallString to the dynamic _String
    void _Copy_Dynamic_String_(bool _Cond);
};

inline const char *String::C_Str() const
{
    if (_Size < 60)
    {
        return _SmallString.Data();
    }
    return _String.Data();
}

inline void String::Append(const char *_C)
{
    // Checks if size is enough
    int _O_Size = _Size;
    _Size = _String_Len(_C) + _O_Size;

    if (_Size < 60)
    {
        for (int i = 0; i < _String_Len(_C); i++)
            _SmallString.Push_Back(_C[i]);
        return;
    }

    _String.Reserve(_Size);

    if (_STRING_OPTIMIZATION_SIZE_ > _O_Size)
    {
        for (int i = 0; i < _STRING_OPTIMIZATION_SIZE_; i++)
            _String.Push_Back(_SmallString[i]);
    }

    for (int i = _O_Size; i < _Size; i++)
        _String.Push_Back(_SmallString[i]);
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
    Clear();
    Append(_C);

    return *this;
}

inline int String::Find(char _C, int _IPos, int _FPos)
{
    if (_IPos > _Size || _FPos > _Size)
    {
        _PRINT_("[STRING]: _Pos: " << _IPos << " to " << _FPos << " is grater than size!!")
        exit(EXIT_FAILURE);
    }

    for (int i = _IPos; i < _FPos; i++)
    {
        if (i < _STRING_OPTIMIZATION_SIZE_ - 1)
            if (_SmallString[i] == _C)
                return i;
        if (i > _STRING_OPTIMIZATION_SIZE_ - 1)
            if (_String[i] == _C)
                return i;
    }

    return -1;
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

int String::_Find_Last(char _C, int _Ipos, int _FPos)
{
    if (_Ipos > _Size || _FPos > _Size)
    {
        _PRINT_("[STRING]: _Pos: " << _Ipos << " to " << _FPos << " is grater than size!!")
        exit(EXIT_FAILURE);
    }

    for (int i = _Ipos; i > _FPos; i--)
    {
        if (i < _STRING_OPTIMIZATION_SIZE_ - 1)
            if (_SmallString[i] == _C)
                return i;
        if (i > _STRING_OPTIMIZATION_SIZE_ - 1)
            if (_String[i] == _C)
                return i;
    }

    return -1;
}

inline void String::_Copy_Dynamic_String_(bool _Cond)
{
    if (_Cond)
    {
        _String.Reserve(_Size);

        for (int i = 0; i < _Size; i++)
            _String.Push_Back(_SmallString[i]);

        _SmallString._Flush_();
    }
}
