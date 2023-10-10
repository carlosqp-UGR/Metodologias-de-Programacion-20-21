/**
 * @file DateTime.cpp
 * @author DECSAI
 * @note To be implemented by students 
 * 
 */

#include <iostream>
#include <sstream>
#include <string>
#include "DateTime.h"
using namespace std;

/**
 * @brief auxiliary function to check if the date and the time are in correct 
 * format, taking into account, leap year etc.
 * @param year input
 * @param month input
 * @param day input
 * @param hour input
 * @param min input
 * @param sec input
 * @return true if is correct, false if not
 */
bool isCorrect(int year, int month, int day, int hour, int min, int sec) ;
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
void split(const std::string &line, int &y, int &m, int &d, int &h, int &mn, int &s);

// Implementación de los métodos y las funciones

DateTime::DateTime()
{
    initDefault();
}

void DateTime::initDefault() 
{
    set(DATETIME_DEFAULT);
}

bool isCorrect(int year, int month, int day, int hour, int min, int sec) 
{
    bool year_ok = false;    
    if (0<=year)
        year_ok = true;
    
    bool month_ok = false;
    if (1<=month && month<=12)
        month_ok = true;
    
    bool day_ok = false;
    switch (month) 
    {
        case (2):   // Febrero
            if ( (year%4==0 && year%100!=0) || year%400==0 ) { // Año Bisiesto
                if(1<=day && day<=29)
                    day_ok = true; 
            }
            else { // No Bisiesto
                if(1<=day && day<=28)
                    day_ok = true;
            }
                
            break;
        
        case (1):   // Enero
        case (3):   // Marzo
        case (5):   // Mayo
        case (7):   // Julio
        case (8):   // Agosto
        case (10):  // Octubre
        case (12):  // Diciembre
            if (1<=day && day<=31)
                day_ok = true;
                
            break;
            
        case (4):   // Abril
        case (6):   // Junio
        case (9):   // Septiembre
        case (11):  // Noviembre
            if (1<=day && day<=30)
                day_ok = true;
            
            break;   
    }
    
    
    bool hour_ok = false;
    if (0<=hour && hour<=23)
        hour_ok = true;
    
    bool min_ok = false;
    if (0<=min && min<=59)
        min_ok = true;
    
    bool sec_ok = false;
    if (0<=sec && sec<=59)
        sec_ok = true;
    
    return ( year_ok && month_ok && day_ok && hour_ok && min_ok && sec_ok );
}

void split (const string &line, int &y, int &m, int &d, int &h, int &mn, int &s)
{
    /// YYYY-MM-dd hh:mm:ss UTC 
    /// ----+--+--+--+--+--+---  length()=22
    ///     |  |  |  |  |  |
    ///     4  7  10 13 16 19
    if (line.length()==23) {
        y = stoi ( line.substr(0,4) );
        m = stoi ( line.substr(5,2) );
        d = stoi ( line.substr(8,2) );
        h = stoi ( line.substr(11,2));
        mn= stoi ( line.substr(14,2));
        s = stoi ( line.substr(17,2));  
    } else 
        split (DATETIME_DEFAULT, y, m, d, h, mn, s);
    
}

void DateTime::set(const string  &line)
{
    int y,m,d,h,mn,s;
    
    split (line, y, m, d, h, mn, s);
    
    if (isCorrect(y, m, d, h, mn, s)) {
        _year   = y;
        _month  = m;
        _day    = d;
        _hour   = h;
        _min    = mn;
        _sec    = s; 
    }
    else
        initDefault();
}

DateTime::DateTime(const string &line) 
{
    set(line);
}

int DateTime::year() const 
{
    return (_year);
}

int DateTime::month() const 
{
    return (_month);
}

int DateTime::day() const 
{
    return (_day);
}

int DateTime::hour() const 
{
    return (_hour);
}

int DateTime::min() const 
{
    return (_min);
}

int DateTime::sec() const 
{
    return (_sec);
}

bool DateTime::isBefore(const DateTime &one) const
{
    bool es_anterior = false;
    
    string this_d = to_string();
    string one_d = one.to_string();
    
    int i=0;
    
    bool sigo = true;
    
    while (sigo)
    {
        if (i<this_d.length()) {
            if (this_d[i]!=one_d[i]) {
                sigo = false;
                if (this_d[i]<one_d[i]) 
                    es_anterior=true;
            } 
        }
        else
            sigo = false;
        
        i++;
    }  
    return (es_anterior);
}

int DateTime::weekDay() const
{
    int a = (14 - _month)/12;
    int y = _year - a;
    int m = _month + 12*a - 2;
    
    int d = (_day + y + y/4 - y/100 + y/400 + (31*m)/12) % 7;
    
    return (d);
}

string DateTime::to_string() const 
{
    char local[64];
    sprintf(local, "%04i-%02i-%02i %02i:%02i:%02i UTC", _year, _month, _day, _hour, _min, _sec);
    return local;
}

bool DateTime::sameDay(const DateTime &other) const {
    bool _sameDay = false;
    
    if ( (_day==other._day) && (_month==other._month) && (_year==other._year))
        _sameDay = true;
    
    return _sameDay;
}
