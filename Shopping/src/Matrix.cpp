/* 
 * File:   matrix.cpp
 * Author: MP-team 
 * @warning To be implemented by students. Follow the instructions given in the headers
 */


#include "Matrix.h"
#include <fstream>
#include <assert.h>

using namespace std;

/**
 * @brief Copies the values in a 2D matrix org with nrows and ncols to an also 2D matrix dest.
 * It is assumed that org and dest have the memory properly allocated 
 * @param dest destination matrix
 * @param org  source matrix
 * @param nrows number of rows
 * @param ncols number of cols
 */
void copy(double **dest, double **org, size_t nrows, size_t ncols) {
    for (int r=0; r<nrows; r++) {
        for (int c=0; c<ncols; c++)
            dest[r][c]=org[r][c];
    }
}

/** 
 * @brief this method reserve memory to allocate a 2D matrix of size r x c.
 * @param r number of rows
 * @param c number of cols
 * @return the pointer to the memory block containing the data
 */
double ** allocate(size_t r, size_t c) {
    double **block = nullptr;
    // Representación: Matriz 2D usando array 1D de punteros a arrays de 1D
    // allocate memory into block
    block = new double *[r];
    for (int i=0; i<r; i++)
        block[i]= new double [c];

    return block;
}

/**
 * @brief In this method, given an index idx it returns the key in the position i, 
 * such that i is the first position in the index such that idx.at(i).getPos()==p
 * @param idx the input index
 * @param p number of 
 * @return the key for first position i in the index such that idx.at(i).getPos()==p
 */
string labelWithValuePosInIndex(const Index & idx, size_t p) {
    string salida;

    bool enc = false;
    for (int i = 0; i < idx.size() && !enc; i++) {
        if (idx.at(i).getPos() == p) {
            salida = idx.at(i).getKey();
            enc = true;
        }
    }
    return salida;
}

void Matrix::deallocate() {
    if (_data != nullptr) {
        for (int i=0; i<_nrows; i++)
            delete [] _data[i];
            
        delete [] _data;
        _data = nullptr;
    }
}

Matrix::Matrix() {
    _data = nullptr;
    _nrows = 0;
    _ncols = 0;
}

void Matrix::setValues(double value) {
    for (int i=0; i<_nrows; i++) {
        for (int j=0; j<_ncols; j++)
            _data[i][j] = value;
    }
}

Matrix::Matrix(size_t f, size_t c, double defValue) {
    Matrix();
    _data = allocate(f,c);
    _nrows = f;
    _ncols = c;
    setValues(defValue);
}

Matrix::Matrix(const Matrix& orig): Matrix(orig.rows(), orig.columns()) {
    copy(this->_data, orig._data, _nrows,  _ncols);
    // Copia de las etiquetas (si es necesario)
    if (orig._rowLabels.size()>0 && orig._colLabels.size()>0) {
        this->_rowLabels = orig._rowLabels;
        this ->_colLabels = orig._colLabels;
    }
}

Matrix::Matrix(const std::string *rowLabels, size_t nrows, const std::string * colsLabels, size_t ncols, double value): Matrix(nrows, ncols, value) {
    this->setLabels(_rowLabels, rowLabels, nrows);
    this->setLabels(_colLabels, colsLabels, ncols);
}

std::string Matrix::labelAtRow(size_t row) const {
//    return labelWithValuePosInIndex(_rowLabels, row);
    assert (0<=row && row<_rowLabels.size());
    return _rowLabels.at(row).getKey();
}

std::string Matrix::labelAtCol(size_t col) const {
//    return labelWithValuePosInIndex(_colLabels, col);
    assert (0<=col && col<_colLabels.size());
    return _colLabels.at(col).getKey();
}

void Matrix::resize(size_t nrows, size_t ncols, double value) {
    clear();
    _data = allocate(nrows, ncols);
    _nrows = nrows;
    _ncols = ncols;
    setValues(value);
}

void Matrix::resize(const std::string * rowLabels, size_t nrows, const std::string * colLabels, size_t ncols, double value) {
    if (nrows>=1 && ncols>=1) {
        resize(nrows, ncols, value);
        setLabels(_rowLabels, rowLabels, nrows);
        setLabels(_colLabels, colLabels, ncols);
    } else
        clear();
}

void Matrix::clear() {
    deallocate();  // Free the memory allocated for the matrix and _data = nullptr
    _nrows = 0;
    _ncols = 0;
    _rowLabels.clear();
    _colLabels.clear();
}

Matrix::~Matrix() {
    clear();
}

double & Matrix::at(size_t f, size_t c) {
    assert(0<=f&&f<rows()); assert(0<=c&&c<columns());    
    return _data[f][c];
}

const double & Matrix::at(size_t f, size_t c) const {
    assert(0<=f&&f<rows()); assert(0<=c&&c<columns());
    return _data[f][c];   
}

double & Matrix::operator()(size_t f, size_t c) {
    return this->at(f,c);
}

const double & Matrix::operator()(size_t f, size_t c) const {
    return this->at(f,c);
}

int Matrix::rowIndexOf(const std::string & label) const {
    int row = _rowLabels.lower_bound(label);
    if (_rowLabels.at(row).getKey() != label)
        row = Matrix::npos;
    return row;
}

int Matrix::colIndexOf(const std::string & label) const {
    int col = _colLabels.lower_bound(label);
    if (_colLabels.at(col).getKey() != label)
        col = Matrix::npos;
    return col;
}

double & Matrix::at(const std::string & rowLabel, const std::string & colLabel) {
    int f = rowIndexOf(rowLabel);
    int c = colIndexOf(colLabel);
    
    assert(0<=f&&f<rows()); assert(0<=c&&c<columns());    
    return _data[f][c];
}

double & Matrix::operator()(const std::string & rowLabel, const std::string & colLabel) {
    return this->at(rowLabel, colLabel);
}

const double & Matrix::at(const std::string & rowLabel, const std::string & colLabel)const {
    int f = rowIndexOf(rowLabel);
    int c = colIndexOf(colLabel);
    
    assert(0<=f&&f<rows()); assert(0<=c&&c<columns());    
    return _data[f][c];
}

const double & Matrix::operator()(const std::string & rowLabel, const std::string & colLabel)const {
    return this->at(rowLabel, colLabel);
}

ostream & operator<<(ostream & flujo, const Matrix& m) {
    // number_of_rows number_of_columns
    flujo << m.rows() << " " << m.columns() << endl;
    // [labelr1 labelr2 label_nrows]
    if (m._rowLabels.size()>0) {
        for (int i=0; i<m._rowLabels.size(); i++)
            flujo << m._rowLabels.at(i).getKey() << " ";
        flujo << endl;
    }
    // [labelc1 labelc2 label_ncols]
    if (m._colLabels.size()>0) {
        for (int i=0; i<m._colLabels.size(); i++)
            flujo << m._colLabels.at(i).getKey() << " ";
        flujo << endl;
    }
    // m[0][0]....m[0][ncols]
    // ...
    // m[nrows][0] ...m[nrows][ncols]
    for (int f=0; f<m.rows(); f++) {
        for (int c=0; c<m.columns(); c++)
            flujo << m.at(f,c) << " ";
        flujo << endl;
    }
    return flujo;
}

Matrix & Matrix::operator=(const Matrix& m) {
    // Comprueba que no es el mismo objeto (tienen direcciones diferentes)
    if(&m != this) {
        // Libera la memoria dinámica de *this
        this->clear();
        // Reservar memoria y redimensionar
        this->resize(m._nrows, m._ncols, 0);
        // Copiar la matriz y sus etiquetas
        copy(this->_data, m._data, this->_nrows,  this->_ncols);
        if (m._rowLabels.size()>0 && m._colLabels.size()>0) {
            this->_rowLabels = m._rowLabels;
            this ->_colLabels = m._colLabels;
        }
    }
    return *this; // Devuelve referencia a *this
}

void Matrix::setLabels(Index & idx, const string * labels, int nlabels){
    idx.clear();
    int lb,ub;
    bool correct = true;
    for (int i=0; i< nlabels && correct; i++){
        lb = idx.lower_bound(labels[i]);
        ub = idx.upper_bound(labels[i]);
        if (lb==ub) { // it is NOT in the set
            idx.add(Pair(labels[i],i));
        } else {
            correct = false;
         }
    }
    if (!correct) idx.clear();
 }

int Matrix::getLabels(const Index & idx, string * labels) const {
    if (idx.size() > 0) {
        for (int i = 0; i < idx.size(); i++) {
            labels[i] = idx.at(i).getKey();
        }
    }
    return idx.size();
}

int Matrix::getRowLabels(string * labels) const {
    return getLabels(_rowLabels,labels);
}

int Matrix::getColLabels(string *labels) const {
    return getLabels(_colLabels,labels);
}

size_t Matrix::columns() const {
    return _ncols;
}

size_t Matrix::rows() const {
    return _nrows;
}

void Matrix::save(const string & nf) const{
    ofstream fsal(nf);
    if (fsal.is_open()) {
        fsal << "#matrix "; 
        if (_rowLabels.size()==0)
            fsal << "unlabeled"<< endl;
        else fsal << "labeled" << endl;
        fsal << *this;
    fsal.close();
    } else cerr << "Fail creating " << nf << " file" <<endl;
}

void Matrix::load(const string & nf) {
    ifstream fent(nf);
    string labeled, cad;
    clear();
    if (fent.is_open()) {
         fent >> cad >> labeled;
         fent >> _nrows >> _ncols;
         if (labeled == "labeled"){
            for (int i=0;i<_nrows;i++){
                fent >> cad;
                _rowLabels.add(Pair(cad,i));
            }
             for (int i=0;i<_ncols;i++){
                fent >> cad;
                _colLabels.add(Pair(cad,i));
            }
        }
        _data = allocate(_nrows,_ncols); 
        for (int i = 0; i < _nrows; i++) {
            for (int j = 0; j < _ncols; j++) {
                fent >> _data[i][j];
            }
        }
        fent.close();
    } else cerr << "Fail reading " << nf << " file" <<endl;
}

Matrix & Matrix::operator*=(double val) {
    for (int f=0; f<rows(); f++) {
        for (int c=0; c<columns(); c++)
            _data[f][c] = val*_data[f][c];
    }
    return *this;
}

Matrix & Matrix::operator+=(double val) {
    for (int f=0; f<rows(); f++) {
        for (int c=0; c<columns(); c++)
            _data[f][c] = val+_data[f][c];
    }
    return *this;
}

// (*this) x M
Matrix Matrix::operator*(const Matrix & m) const {
    assert(this->_nrows> 0 && this->_ncols>0);
    assert(m._nrows>0 && m._ncols>0);
    assert(this->_ncols == m._nrows);
    Matrix product (this->_nrows, m._ncols);
    product._rowLabels = this->_rowLabels;
    product._colLabels = m._colLabels;
    
    for (int r=0; r<product.rows(); r++) {
        for (int c=0; c<product.columns(); c++) {
            for (int i=0; i< m._nrows; i++)
                product._data[r][c] += this->_data[r][i] * m._data[i][c];
        }
    }
    
    return product;
}

Matrix Matrix::operator+(double val) const {
    Matrix add (*this);
    add += val;
    return add;
}

Matrix Matrix::operator*(double val) const {
    Matrix product (*this);
    product *= val;
    return product;
}

// val + M
Matrix operator+(double ival, const Matrix & dch) {
    Matrix add (dch);
    add += ival;
    return add;
}

Matrix operator*(double ival, const Matrix & dch) {
    Matrix product (dch);
    product *= ival;
    return product;
}

Matrix & Matrix::normalize(){
    double totalSum=0;
    
    for (int f=0; f<rows(); f++) {
        totalSum=0;
        // Primera iteración: calcula la suma total de la fila f
        for (int c=0; c<columns(); c++)
            totalSum += _data[f][c];
        // Segunda iteración: normaliza los valores de la fila f
        if (totalSum>0) {
            for (int c=0; c<columns(); c++)
                _data[f][c] = _data[f][c]/totalSum;
        } else { // totalSum==0
            for (int c=0; c<columns(); c++)
                _data[f][c] = 1.0/columns();
        }
    }
    // Devolver referencia a *this
    return *this;
}
