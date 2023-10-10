/* 
 * File:   Pair.cpp
 * @author MP-Team DECSAI
 * @warning To be implemented by students
 */
#include <string>
#include "Pair.h"

using namespace std;

Pair NULL_PAIR = Pair();

Pair::Pair()  {
    initDefault();
}

bool Pair::isEmpty() const {
    bool _isEmpty = false;
    
    if (_key==EMPTY_FIELD && _pos<0) {
        _isEmpty = true;
    }
    
    return _isEmpty;
}

Pair::Pair(const std::string & key, int pos) {
    if (key.length()>0) {
        _key = key;
        _pos = pos;
    } else
        initDefault();
}

const std::string & Pair::getKey() const {
    return (_key);
}

const int Pair::getPos() const {
    return (_pos);
}

void Pair::setKey(const std::string & key) {
    if (key.length()>0)
        _key = key;
    else
        initDefault();
}

void Pair::setPos(int pos) {
    if (pos>=0)
        _pos = pos;
    else
        initDefault();     
}

void Pair::set(const std::string & key, int pos) {
    setKey(key);
    setPos(pos);
}

std::string Pair::to_string() const { /** necessary ?*/
    string result;
    result = _key + " " + std::to_string(_pos);
    return result;
}

void Pair::initDefault() {
    _key = EMPTY_FIELD;
    _pos = -1;
}

bool equalKey(const Pair & left, const Pair & right) {
    bool _equalKey = false;
    
    if (left.getKey()==right.getKey())
        _equalKey = true;
    
    return _equalKey;
}