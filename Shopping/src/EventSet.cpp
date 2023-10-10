/* 
 * File:   EventSet.cpp
 * @author MP-Team DECSAI
 * @note To be implemented by students 
 */


#include <unordered_map>
#include <fstream>
#include "EventSet.h"

using namespace std;

Event NULL_EVENT=Event();

/**
 * @brief Give string[] and a string, it searches for string within string[]
 * @param array     The set of strings to look for
 * @param n_array   The size of string[]
 * @param name      The string that is being seeked
 * @return          The position of the first occurrence of string within string[]
 *  -1 if the string does not belong to string[]
 */
int findString(const string array[], int n_array, const string &name) 
{
    int pos=-1;
    bool encontrado=false;
    for (int i = 0; i < n_array && !encontrado; i++) {
        if (array[i] == name) {
            pos = i;
            encontrado = true;
        }
    }
    return (pos);
}

void EventSet::reallocate(unsigned size) {
    if (size>_capacity) {
        Event *_aux = nullptr;
        _aux = new Event [size];
        
        this->copy(_aux);
        
        delete [] _events;
        _events = _aux;
        _capacity = size;
        _aux = nullptr;
    }
}

void EventSet::deallocate() {
    if (_events != nullptr) {
        delete [] _events;
        _events = nullptr;
    }
}

void EventSet::copy(Event *dest) const {
    for (int i=0; i<size(); i++)
        dest[i] = _events[i];
}

void EventSet::copy(const EventSet &orig) {
    orig.copy(this->_events);
}

EventSet::EventSet() {
    _events = nullptr;
    _nEvents = 0;
    _capacity = 0;
}

EventSet::EventSet(const EventSet& orig) {
    EventSet();
    reallocate(orig._capacity);
    this->copy(orig);
    this->_nEvents = orig._nEvents;
}

int EventSet::size() const {
    return(_nEvents);
}

std::string EventSet::to_string() const {
    string result;
    result = std::to_string(_nEvents) + " ";
    for (int i = 0; i < _nEvents; i++)
        result += _events[i].to_string() + "\n";
    return result;
}

//REVISAR MÉTODO CLEAR
void EventSet::clear() {
    deallocate();
    _nEvents = 0;
    _capacity = 0;
}

//REVISAR MÉTODO ADD (MAX_EVENT??)
int EventSet::add(const Event &e) {
    int n=0;
    
    if (size()==_capacity)
        reallocate(2*_capacity+1);

    _events[_nEvents] = e;
    _nEvents++;
    n=1;
    
    return (n);
}

int EventSet::add(const std::string& line) {
    Event e(line);
    return(add(e));
}

const Event & EventSet::at(int pos) const {
    if (0<=pos && pos<_nEvents) 
        return (_events[pos]);
    else
        return (NULL_EVENT);
}

void EventSet::write(std::ofstream &os) const   {
    for (int i=0; i<_nEvents; i++) {
        _events[i].write(os);
        os << endl;
    }
}

bool EventSet::read(std::ifstream &is, int nelements)   {
    bool _readOk = true;
    Event e;
    clear();
    for (int i = 0; i < nelements && _readOk; i++) {
        e.read(is);
        if (!is)
            _readOk = false;
        else {
            if (!e.isEmpty()) 
                _readOk = add(e);
        }
    }

    return _readOk;
}

EventSet& EventSet::operator=(const EventSet &one) {
    // Comprobar que no es el mismo objeto
    if (one._events != this->_events) { 
        // Si *this tiene memoria dinámica: LIBERARLA
        this->clear();
        // Copiar one en *this
        reallocate(one._capacity); // Reserva la misma capacidad que one
        this->copy(one);   // Copia el vector de eventos de one en *this
        this->_nEvents = one._nEvents; // Copia el numero de Eventos  
    }
    // Devolver referencia a *this
    return *this;   
}

// REVISAR MÉTODO ~EventSet()
EventSet::~EventSet() {
    clear();
}

float sumPrice(const EventSet &evSet) {
    float sum=0;
    
    for(int i=0; i<evSet.size(); i++) {
        if (evSet.at(i).getPrice()>0) 
            sum+=evSet.at(i).getPrice();
    }
        
    return (sum);
}

void findUnique(const EventSet &es, const string &field, string values[], int &n_values) {
    string value;
    n_values=0;
    
    for (int i=0; i<es.size(); i++) {
        value = es.at(i).getField(field);
        
        if (field!="DateTime") {
            if (findString(values, n_values, value) == -1) {
                values[n_values] = value;
                n_values++;
            }
        } else {
            DateTime DTvalue(value);
            bool encontrado = false;
            for (int j=0; j<n_values && !encontrado; j++) {
                DateTime DTaux;
                DTaux.set(values[j]);
                if (DTvalue.sameDay(DTaux))
                    encontrado = true;
            }
            
            if (!encontrado) {
                value[11] = value[12] = value[14] = value[15] = value[17] = value[18] = '0';
                values[n_values] = value;
                n_values++;
            }
        }
        
    }
}

EventSet rawFilterEvents(const EventSet &es, const string &field, const string &value) {
    EventSet salida;
    
    for (int i=0; i<es.size(); i++) {
        if (es.at(i).getField(field)==value)
            salida.add(es.at(i));
    }
        
    return (salida);
}
