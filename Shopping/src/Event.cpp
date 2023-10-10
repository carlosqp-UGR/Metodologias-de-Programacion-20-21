/**
 * @file Event.cpp
 * @author DECSAI
 * @note To be implemented by students 
 * 
 */

#include <string>
#include <fstream>
#include "Event.h"
using namespace std;

/**
 * @brief split the first field in 6 components of the data time.
 * Please consider using string::substr(int, int)[https://en.cppreference.com/w/cpp/string/basic_string/substr] to cut the line
 * into the appropriate substrings and then convert it into integer values with 
 * the function stoi(string) [https://en.cppreference.com/w/cpp/string/basic_string/stol]
 * ~~~~
 *      YYYY-MM-dd hh:mm:ss UTC 
 *      +----+----+----+----+----+ 
 *      |    |    |    |    |    |
 *      0    5    10   15   20   25
 * ~~~~
 *  * @param line input string
 * @param y output int
 * @param m output int
 * @param d output int
 * @param h output int
 * @param mn output int
 * @param s output int
 */
/// void split(const string &line);

Event::Event() 
{
    initDefault();
}

void Event::initDefault() 
{
    set(EVENT_DEFAULT);
}

Event::Event(const string &line) 
{
    set(line);
}

DateTime Event::getDateTime()  const
{
    return(_dateTime);
}

string Event::getType() const
{
    return (_type);
}

string Event::getProductID() const
{
    return (_prod_id);
}

string Event::getCategoryID() const 
{
    return (_cat_id);
}

string Event::getCategoryCode() const
{
    return (_cat_cod);
}

string Event::getBrand() const
{
    return (_brand);
}

double Event::getPrice() const
{
    return (_price);
}

string Event::getUserID() const
{
    return (_user_id);
}

string Event::getSession() const
{
    return (_session);
}

void Event::setDateTime(const string &time)
{
    _dateTime.set(time);
}

void Event::setType(const string &type) 
{
    if (type == VALID_TYPES[1])
        _type = VALID_TYPES[1];
    else
        if (type == VALID_TYPES[2])
            _type = VALID_TYPES[2];
        else
            if (type == VALID_TYPES[3])
                _type = VALID_TYPES[3];
            else    // Si no es ninguno de los 3 últimos tipos, por defecto--> _type = "view"
                _type = VALID_TYPES[0];

}

void Event::setProductID(const string &prod_id) // NOTNULL
{
    if (prod_id.length()<=0)
        _prod_id = EMPTY_FIELD;
    else
        _prod_id = prod_id;
}

void Event::setCategoryID(const string &cat_id) 
{
    _cat_id = cat_id;
}

void Event::setCategoryCode(const string &cat_cod) 
{
    _cat_cod = cat_cod;
}

void Event::setBrand(const string &brand) 
{
    _brand = brand;
}

void Event::setPrice(double price) 
{
    if (0<=price)
        _price = price;
    else 
        _price = -1.0;
}

void Event::setUserID(const string &user_id) //NOTNULL
{
    if (user_id.length()<=0)
        _user_id = EMPTY_FIELD;
    else
        _user_id = user_id;
}

void Event::setSession(const string &session) //NOTNULL
{
    if (session.length()<=0)
        _session = EMPTY_FIELD;
    else
        _session = session; 
}

void Event::set(const string &line) 
{
    /// Separacion de la linea en los diferentes campos
    // Cada uno de ellos se almacena por orden de aparicion en 'line_fields'
    
    string line_fields [9];
    
    int pos_ini=0, pos_fin, num_caract;
    
    for (int i=0; i<9; i++)
    {
        pos_fin = line.find(",",pos_ini);
        num_caract = pos_fin - pos_ini;
        line_fields[i] = line.substr(pos_ini, num_caract);
        
        pos_ini = pos_fin + 1;
    }
    
    double p;
    
    if (line_fields[6].length()<=0)
        p = -1.0;
    else
        p = stod(line_fields[6]);
    
    setDateTime     (line_fields[0]);
    setType         (line_fields[1]);
    setProductID    (line_fields[2]);
    setCategoryID   (line_fields[3]);
    setCategoryCode (line_fields[4]);
    setBrand        (line_fields[5]);
    setPrice        (p);
    setUserID       (line_fields[7]);
    setSession      (line_fields[8]);
}

bool Event::isEmpty() const
{
    bool is_empty = false;
    
    Event def; // Evento por defecto, constructor sin argumentos
    
    if  ((_prod_id == def._prod_id) || (_user_id==def._user_id) || (_session==def._session))
        is_empty = true;
    
    return (is_empty);
}

string Event::to_string() const {
    string salida;
    salida += _dateTime.to_string();
    salida += "," + _type;
    salida += "," + _prod_id;
    salida += "," + _cat_id;
    salida += "," + _cat_cod;
    salida += "," + _brand;
    salida += "," + std::to_string(_price);
    salida += "," + _user_id;
    salida += "," + _session;
    return salida;
}

string Event::getField(const string &field) const
{
    string salida;
    
    if(field=="DateTime")
        salida = _dateTime.to_string();
    else {
        if (field=="Type")
            salida = _type;
        else {
            if (field=="ProductID")
                salida = _prod_id;
            else {
                if (field=="CategoryID")
                    salida = _cat_id;
                else {
                    if(field=="CategoryCode")
                        salida = _cat_cod;
                    else {
                        if(field=="Brand")
                            salida = _brand;
                        else {
                            if (field=="Price")
                                salida = std::to_string(_price);
                            else {
                                if (field=="UserID")
                                    salida = _user_id;
                                else {
                                    if (field=="Session")
                                        salida = _session;
                                }
                            }
                        }
                    }
                }        
            }
        }
    }
    
    return (salida);
}

void Event::write(std::ostream &os) const  {
    os << to_string();
}


void Event::read(std::istream &is) {
    string line;
    
    getline(is,line);
    set(line);
}