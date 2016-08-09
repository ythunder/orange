//
// Created by yunting on 16-8-9.
//

#include <iostream>
#include <string>

class MyString
{
private:
    char* _data;
    size_t _len;
    void _init_data( const char* s);

public:
    MyString();
    MyString( const char* p);
    MyString( const MyString& str);
    MyString&operator=( const MyString& str);
    virtual ~MyString();
};

void
MyString::_init_data(const char *s)
{
    _data = new char[_len+1];
    memcpy( _data, s, _len);
    _data[_len] = '\0';
}

MyString::MyString()
{
    _data = NULL;
    _len = 0;
}

MyString::MyString(const char *p)
{
    _len = strlen( p );
    _init_data( p );
}

MyString::MyString( const MyString& str)
{
    this->_len = str._len;
    _init_data( str._data );
    std::cout << "Copy Constructor is called! source: " << str._data << std::endl;
    return *this;
}

MyString& operator=(const MyString& str)
{
    if(this != &str)
    {
        _len = str.len;
        _init_data(str._data);
    }
    std::cout << "Copy Assigment is called! source: " << str._data << std::endl;
    return *this;
}
virtual MyString::~MyString()
{
    if( this->_data)
    {
        free(_data);
    }
}


int main(int argc, char *argv[])
{
    MyString a;
    a = MyString("Hello");
    std::vector<MyString> vec;
    vec.push_back(MyString("World"));
}