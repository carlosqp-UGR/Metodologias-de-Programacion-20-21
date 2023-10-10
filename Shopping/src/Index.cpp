/* 
 * File:   Index.cpp
 * @author MP-Team DECSAI
 * @warning To be implemented by students 
 */
#include <string>
#include <iostream>
#include <fstream>
#include "Index.h"

using namespace std;

void Index::setIOnWhich(int val) {
    _onBrand=0;
    
    if (val==1)
        _onBrand=1;
    else if (val==2)
        _onBrand=2;
}

void Index::copy(Pair* dest) const{
    for (int i=0; i<size(); i++)
        dest[i] = _entries[i];
}

void Index::copy(const Index& orig) {
    orig.copy(this->_entries);
}

void Index::reallocate(int size) {
    if (size>_capacity) {
        Pair *_aux = nullptr;
        _aux = new Pair [size];
        this->copy(_aux);
        delete [] _entries;
        _entries = _aux;
        _capacity = size;
        _aux = nullptr;
    }
}

void Index::deallocate() {
    if (_entries != nullptr) {
        delete [] _entries;
        _entries = nullptr;
    }
}

Index::Index(int onBrand) {
    _entries = nullptr;
    _nEntries = 0;
    _capacity = 0;
    setIOnWhich(onBrand);
}

Index::Index(const Index & orig) {
    Index(orig.getIOnWhich());
    reallocate(orig._capacity);
    this->copy(orig);
    this->_nEntries = orig._nEntries;
}

int Index::size() const {
    return _nEntries;
}

void Index::build(const EventSet & evSet, int onBrand) {
    Pair local;
    clear();
    setIOnWhich(onBrand);
    string key, field = "UserID";
    
    if (_onBrand==1)
        field = "Brand";
    else if (_onBrand==2)
        field = "Session";
    
    for (int i=0; i<evSet.size(); i++) {
        key = evSet.at(i).getField(field);
        if (key.length()>0) {
            local.set(key, i);
            add(local);
        }
    }
}

int Index::getIOnWhich() const {
    return _onBrand;
}

const Pair & Index::at(int pos) const {
    return _entries[pos];
}

void Index::clear() {
    deallocate();
    _nEntries = 0;
    _capacity = 0;
    _onBrand = 0;
}

Pair & Index::at(int pos) {
    if (0<=pos && pos<_nEntries) 
        return (_entries[pos]);
    else
      return (NULL_PAIR);
}

int Index::lower_bound(const string & key) const {
    int _lowerBound = 0;
    
    while (_lowerBound<_nEntries && key>_entries[_lowerBound].getKey())
        _lowerBound++;
    
    return _lowerBound;
}

int Index::upper_bound(const string & key) const {
    int _upperBound = lower_bound(key);
    
    while (_upperBound<_nEntries && key==_entries[_upperBound].getKey())
        _upperBound++;

    return _upperBound;
}

// REVISAR MÉTODO ADD
int Index::add(const Pair & pair) {
    int output = 1;

    // NO se incluyen en el índice:
    // 	- Cuando el índice está completo ¿?
    //	- Cuando la pareja está vacía (NULL_PAIR: _key=EMPTY_FIELD, _pos=-1)
    //	- Cuando _key = ""; este caso si es posible en pair pero no se admite en el índice 	
    if ( (pair.getKey().length()<=0) || (pair.isEmpty()) ) // || (_nEntries>=MAXEVENT) )
        output = 0; //Error al añadir
    else {
        if (size()==_capacity)
            reallocate(2*_capacity+1);
        
        // Se busca la posición donde debe de ser insertado 'pair'
        int pos = upper_bound(pair.getKey());
        // Se recorre todo el vector desde 'pos' (incluida) a la derecha en una unidad
        for (int i=_nEntries; i>pos; i--)
            _entries[i] = _entries[i-1];
        // Se inserta 'pair'
        _entries[pos] = pair;
        _nEntries++;
    }
    
    return output;
}

void Index::print()const {
    cout << to_string() << endl;
}

Index& Index::operator=(const Index &one) {
    // Comprobar que no es el mismo objeto
    if (_entries != one._entries) {
        // Si *this tiene memoria dinámica: LIBERARLA
        this->clear();
        // Copiar one en *this
        reallocate(one._capacity);
        this->copy(one);
        this->_nEntries = one._nEntries;
        this->setIOnWhich(one.getIOnWhich());
    }
    // Devolver referencia a *this
    return *this; 
}

Index::~Index() {
    clear();
}

Index rawFilterIndex(const EventSet & evSet, const Index & indx, const std::string &field, const std::string &value) {
    Index rawIndx(indx.getIOnWhich());
        
    if (field != "DateTime") {
        for (int i=0; i<indx.size(); i++) {
            if (getEvent(evSet, indx, i).getField(field) == value) {        
                Pair local (indx.at(i));
                rawIndx.add(local);
            }
        }  
    } else { // field == DateTime
        DateTime valueDT(value);
        for (int i=0; i<indx.size(); i++) {
            if (valueDT.sameDay(getEvent(evSet, indx, i).getDateTime())) {
                Pair local (indx.at(i));
                rawIndx.add(local);        
            }
        }
    }
    
    return rawIndx;
}

Event getEvent(const EventSet & evSet, const Index & indx, int pos) {
    Event e;
    int pos_event=0;
    if (0<=pos && pos<indx.size()) {
        pos_event=indx.at(pos).getPos();
        if (0<=pos_event && pos_event<evSet.size())
            e = evSet.at(indx.at(pos).getPos());
    } // else e = EVENT_DEFAULT;
    
    return e;   
}

void write(ofstream &os, const EventSet & evSet, const Index & indx) {
    Event e;
    for(int i=0; i<indx.size(); i++) {
        e=getEvent(evSet, indx, i);
        e.write(os);
        os << endl;
    }
}

float sumPrice(const EventSet & evSet, const Index & indx) {
    float price = 0.0;
    double aux = 0;
    
    for (int i=0; i<indx.size(); i++) {
        aux = getEvent(evSet, indx, i).getPrice();
        if (aux>0)
            price += aux;
    }
    
    return price;
}
