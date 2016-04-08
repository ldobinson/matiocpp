/***************************************************************************
 * Copyright (c) 2016 <Salva Ardid> All rights reserved.                   *
 * Based on previous version by Lars Johannesen                            *
 *                                                                         *
 * This file is part of MATIOCPP                                           *
 *                                                                         *
 * MATIOCPP is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * MATIOCPP is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with MATIOCPP.  If not, see <http://www.gnu.org/licenses/>.       *
 **************************************************************************/
#ifndef MATIOCPP_LJ_04_08_2016
#define MATIOCPP_LJ_04_08_2016 1

#include <string>
#include <matio.h>
#include <stdexcept>
#include <armadillo>

#include <vector>
#include <map>

namespace matiocpp {
  using namespace arma;
  using namespace std;

  class matiocpp_exception : public std::logic_error {
    public:
      matiocpp_exception(const char *what) : std::logic_error(what) {
      }
  };

  class matiocpp_castexception : public matiocpp_exception {
    public:
      matiocpp_castexception(const char *what) : matiocpp_exception(what) {
      }
  };

  // NOTE: Fwd declaractions to allow for cast operators for MatVar
  class MatVar;
  class Struct;
  class Cell;

  /**
  * @brief Matlab variable class
  */
  class MatVar {
    public:
      /**
       * @brief Create empty variable (zero-pointer)
       */
      MatVar() : _matvar(0) {
      }

      /**
       * @brief Create matlab variable from matio pointer
       *
       * @param mt
       */
      MatVar(matvar_t *mt) : _matvar(mt) {
      }

      /**
       * @brief Create matlab variable from armadillo vector
       *
       * @param v Armadillo vector
       */
      // SA: rename vec by colvec
      MatVar(const colvec &v) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly
        void* x = const_cast<void*>(reinterpret_cast<const void*>(v.memptr()));

        std::size_t dims[2];
        dims[0] = v.n_elem; // SA: fix for a column vector 1;
        dims[1] = 1; // v.n_elem;

        _matvar = Mat_VarCreate(NULL, MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, x, 0);

        if(!_matvar) {
          throw matiocpp_exception("can't create colvec");
        }
      }
      // SA: added icolvec
      MatVar(const icolvec &v) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly
        void* x = const_cast<void*>(reinterpret_cast<const void*>(v.memptr()));

        std::size_t dims[2];
        dims[0] = v.n_elem; // SA: fix for a column vector 1;
        dims[1] = 1; // v.n_elem;

        _matvar = Mat_VarCreate(NULL, MAT_C_INT64, MAT_T_INT64, 2, dims, x, 0);

        if(!_matvar) {
          throw matiocpp_exception("can't create icolvec");
        }
      }
      // SA: added rowvec
      MatVar(const rowvec &v) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly
        void* x = const_cast<void*>(reinterpret_cast<const void*>(v.memptr()));

        std::size_t dims[2];
        dims[0] = 1;
        dims[1] = v.n_elem;

        _matvar = Mat_VarCreate(NULL, MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, x, 0);

        if(!_matvar) {
          throw matiocpp_exception("can't create rowvec");
        }
      }
      // SA: added irowvec
      MatVar(const irowvec &v) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly
        void* x = const_cast<void*>(reinterpret_cast<const void*>(v.memptr()));

        std::size_t dims[2];
        dims[0] = 1;
        dims[1] = v.n_elem;

        _matvar = Mat_VarCreate(NULL, MAT_C_INT64, MAT_T_INT64, 2, dims, x, 0);

        if(!_matvar) {
          throw matiocpp_exception("can't create irowvec");
        }
      }
      /**
       * @brief Create matlab variable from matrix
       *
       * @param v Armadillo matrix
       */
      MatVar(const mat &v) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly
        void* x = const_cast<void*>(reinterpret_cast<const void*>(v.memptr()));

        std::size_t dims[2];
        dims[0] = v.n_rows;
        dims[1] = v.n_cols;

        _matvar = Mat_VarCreate(NULL, MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, x, 0);

        if(!_matvar) {
          throw matiocpp_exception("can't create mat");
        }
      }
      MatVar(const imat &v) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly
        void* x = const_cast<void*>(reinterpret_cast<const void*>(v.memptr()));

        std::size_t dims[2];
        dims[0] = v.n_rows;
        dims[1] = v.n_cols;

        _matvar = Mat_VarCreate(NULL, MAT_C_INT64, MAT_T_INT64, 2, dims, x, 0);

        if(!_matvar) {
          throw matiocpp_exception("can't create imat");
        }
      }

      /**
       * @brief Create matlab variable from string
       *
       * @param str String
       */
      MatVar(const std::string &str) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly
        void *x = const_cast<void*>(reinterpret_cast<const void*>(str.c_str()));

        std::size_t dims[2];
        dims[0] = 1;
        // dims[1] = str.size()+1;
        dims[1] = str.size(); // SA: fix for actual string size

        // FIXME: MAT_T_UINT8 is used even though they have MAT_T_STRING, but that is not accepted / used in the current version of matio
        _matvar = Mat_VarCreate(NULL, MAT_C_CHAR, MAT_T_UINT8, 2, dims, x, 0);

        if(!_matvar) {
          throw matiocpp_exception("can't create string");
        }
      }

      ~MatVar() {
        // NOTE: Potential double free, but should be fine as all the MatVar classes will be pointing to the same
        //     _matvar, so Mat_VarFree should check if it has been free'd
        if(!_matvar) {
          Mat_VarFree(_matvar);
        }
      }

    public:
      /**
       * @brief Cast to armadillo vector
       *
       * @return armadillo vector
       */
      // SA: renamed vec by colvec
      operator colvec() const {
        if(!_matvar) {
          throw matiocpp_castexception("Can't cast not-instantiate");
        }

        if(_matvar->data_type != MAT_T_DOUBLE && _matvar->class_type != MAT_C_DOUBLE) {
          throw matiocpp_castexception("Cannot be cast to colvec: wrong type");
        }

        if(_matvar->isComplex) {
          throw matiocpp_castexception("Cannot be cast to colvec: is complex");
        }
        // SA: fixed dimension checking
        if(_matvar->dims[1] != 1) {
          throw matiocpp_castexception("Cannot be cast to colvec: wrong dimension");
        }

        int nelem = _matvar->dims[0] * _matvar->dims[1];
        double *data = reinterpret_cast<double*>(_matvar->data);

        // FIXME: This means copying the data, but we have to as the _matvar data might not be alive for the same duration as the vec we are returning.
        colvec x(data,nelem,true);

        return x;
      }
      // SA: added icolvec
      operator icolvec() const {
        if(!_matvar) {
          throw matiocpp_castexception("Can't cast not-instantiate");
        }

        if(_matvar->data_type != MAT_T_INT64 && _matvar->class_type != MAT_C_INT64) {
          throw matiocpp_castexception("Cannot be cast to icolvec: wrong type");
        }

        if(_matvar->isComplex) {
          throw matiocpp_castexception("Cannot be cast to icolvec: is complex");
        }
        // SA: fixed dimension checking
        if(_matvar->dims[1] != 1) {
          throw matiocpp_castexception("Cannot be cast to icolvec: wrong dimension");
        }

        int nelem = _matvar->dims[0] * _matvar->dims[1];
        long long *data = reinterpret_cast<long long*>(_matvar->data);

        // FIXME: This means copying the data, but we have to as the _matvar data might not be alive for the same duration as the vec we are returning.
        icolvec x(data,nelem,true);

        return x;
      }
      // SA: added rowvec
      operator rowvec() const {
        if(!_matvar) {
          throw matiocpp_castexception("Can't cast not-instantiate");
        }

        if(_matvar->data_type != MAT_T_DOUBLE && _matvar->class_type != MAT_C_DOUBLE) {
          throw matiocpp_castexception("Cannot be cast to rowvec: wrong type");
        }

        if(_matvar->isComplex) {
          throw matiocpp_castexception("Cannot be cast to rowvec: is complex");
        }

        if (_matvar->dims[0] != 1) {
          throw matiocpp_castexception("Cannot be cast to rowvec: wrong dimension");
        }

        int nelem = _matvar->dims[0] * _matvar->dims[1];
        double *data = reinterpret_cast<double*>(_matvar->data);

        // FIXME: This means copying the data, but we have to as the _matvar data might not be alive for the same duration as the vec we are returning.
        rowvec x(data,nelem,true);

        return x;
      }
      // SA: added irowvec
      operator irowvec() const {
        if(!_matvar) {
          throw matiocpp_castexception("Can't cast not-instantiate");
        }

        if(_matvar->data_type != MAT_T_INT64 && _matvar->class_type != MAT_C_INT64) {
          throw matiocpp_castexception("Cannot be cast to irowvec: wrong type");
        }

        if(_matvar->isComplex) {
          throw matiocpp_castexception("Cannot be cast to irowvec: is complex");
        }
        // SA: fixed dimension checking
        if(_matvar->dims[1] != 1) {
          throw matiocpp_castexception("Cannot be cast to irowvec: wrong dimension");
        }

        int nelem = _matvar->dims[0] * _matvar->dims[1];
        long long *data = reinterpret_cast<long long*>(_matvar->data);

        // FIXME: This means copying the data, but we have to as the _matvar data might not be alive for the same duration as the vec we are returning.
        irowvec x(data,nelem,true);

        return x;
      }

      /**
       * @brief Cast to armadillo matrix
       *
       * @return armadillo matrix
       */
      operator mat() const {
        if(!_matvar) {
          throw matiocpp_castexception("Can't cast not-instantiate");
        }

        if(_matvar->data_type != MAT_T_DOUBLE && _matvar->class_type != MAT_C_DOUBLE) {
          throw matiocpp_castexception("Cannot be cast to mat: wrong type");
        }

        if(_matvar->isComplex) {
          throw matiocpp_castexception("Cannot be cast to mat: is complex");
        }

        if(_matvar->rank != 2) {
          throw matiocpp_castexception("Cannot be cast to mat: wrong rank");
        }

        int nrows = _matvar->dims[0];
        int ncols = _matvar->dims[1];

        double *data = reinterpret_cast<double*>(_matvar->data);

        // FIXME: This means copying the data, but we have to as the _matvar data might not be alive for the same duration as the vec we are returning.
        mat x(data,nrows,ncols,true);

        return x;
      }
      operator imat() const {
        if(!_matvar) {
          throw matiocpp_castexception("Can't cast not-instantiate");
        }

        if(_matvar->data_type != MAT_T_INT64 && _matvar->class_type != MAT_C_INT64) {
          throw matiocpp_castexception("Cannot be cast to mat: wrong type");
        }

        if(_matvar->isComplex) {
          throw matiocpp_castexception("Cannot be cast to mat: is complex");
        }

        if(_matvar->rank != 2) {
          throw matiocpp_castexception("Cannot be cast to mat: wrong rank");
        }

        int nrows = _matvar->dims[0];
        int ncols = _matvar->dims[1];

        long long *data = reinterpret_cast<long long*>(_matvar->data);

        // FIXME: This means copying the data, but we have to as the _matvar data might not be alive for the same duration as the vec we are returning.
        imat x(data,nrows,ncols,true);

        return x;
      }

      /**
       * @brief Cast to string
       *
       * @return string
       */
      operator std::string() const {
        if(!_matvar) {
          throw matiocpp_castexception("Can't cast not-instantiate");
        }

        if(_matvar->data_type != MAT_T_UINT8 && _matvar->class_type != MAT_C_CHAR) {
          throw matiocpp_castexception("Cannot be cast to string: wrong type");
        }

        if(_matvar->isComplex) {
          throw matiocpp_castexception("Cannot be cast to string: is complex");
        }

        if(_matvar->rank != 2 || (_matvar->dims[0] != 1 && _matvar->dims[1] != 1)) {
          throw matiocpp_castexception("Cannot be cast to string: wrong rank/dimension");
        }

        int nelem = _matvar->dims[0] * _matvar->dims[1];

        char *str = reinterpret_cast<char*>(_matvar->data);

        std::string s(str);

        return s;
      }

      /**
       * @brief Cast to Cell
       *
       * @return Cell
       */
      operator Cell() const;

      /**
       * @brief Cast to struct
       *
       * @return Struct
       */
      operator Struct() const;

    private:
      /**
       * @brief Access the _matvar pointer directly
       *
       * @return matvar_t pointer
       */
      matvar_t* operator->() const {
        if(!_matvar) {
          throw matiocpp_exception("Cannot dereference null pointer!");
        }

        return _matvar;
      }

      /**
       * @brief Convert to a matvar_t pointer
       *
       * @return matvar_t pointer
       */
      operator matvar_t*() const {
        return _matvar;
      }

      friend class Struct;
      friend class Cell;
      friend class Loader;
      friend class Saver;

    private:
      matvar_t *_matvar;
  };

  /**
   * @brief Cell array class
   */
  class Cell {
    public:
      /**
       * @brief Create cell array
       *
       * @param dims Dimensions
       * @param fill Fill with zero-matrix, as writing a not full cell array can corrupt file
       */
      Cell(const std::vector<std::size_t> &dims, bool fill=true) : _ndims(dims.size()), _dims(dims) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly
        std::size_t *d = const_cast<std::size_t*>(&dims[0]);

        // NOTE: Creating vector of dimensions with same content as
        //     dims as the API requires int* for subscript calc
        //     but std::size_t* for init.
        _idims = zeros<Col<int> >(_ndims);
        for(std::size_t i = 0; i < _ndims; ++i) {
          _idims[i] = dims[i];
        }
        _nelem = static_cast<int>(prod(_idims));

        matvar_t *mm = Mat_VarCreate(NULL, MAT_C_CELL, MAT_T_CELL, _ndims, d, NULL, 0);
        if(mm == NULL) {
          throw matiocpp_exception("Failed to create cell");
        }

        _mptr = MatVar(mm);

        // FIXME: Internals to make sure we dont save partially filled cell array
        if (fill) {
          mat zz = zeros<mat>(0,0);
          MatVar mv(zz);

          for(std::size_t i = 0; i < _nelem; ++i) {
            matvar_t *m = Mat_VarSetCell(_mptr, i, mv);

            // FIXME: Not checking return as it can either mean: 1) first element or error or 2) error
          }
        }
      }

      /**
       * @brief Convert from Matvar to cell
       *
       * @param ptr Matvar reference
       */
      Cell(MatVar &ptr) : _mptr(ptr) {
        if(ptr->data_type != MAT_T_CELL || ptr->class_type != MAT_C_CELL) {
          //FIXME: Should this be a regular exception?
          throw matiocpp_castexception("Cannot be cast to cell");
        }

        _ndims = _mptr->rank;

        _dims.resize(_ndims);
        _idims.set_size(_ndims);
        for(std::size_t i = 0; i < _ndims; ++i) {
          _dims[i] = _mptr->dims[i];
          _idims[i] = _mptr->dims[i];
        }
      }

    public:
      /**
       * @brief Convert to a MatVar
       *
       * @return MatVar
       */
      operator MatVar() const {
        return _mptr;
      }

      /**
       * @brief Set element of matvar
       *
       * @param i index
       * @param v value
       */
      void set(const std::size_t i, const MatVar &v) {
        if(i >= _nelem) {
          cout << "i = " << i << ", nelem = " << _nelem << endl;
          throw matiocpp_exception("Cell's set out of bounds");
        }

        MatVar mv(v);
        matvar_t *m = Mat_VarSetCell(_mptr, i, mv);

        if(m == NULL) {
          throw matiocpp_exception("could not set cell");
        }
      }

      /**
       * @brief Set element of matvar, using dimension vector
       *
       * @param idx Vector of indices
       * @param v value
       */
      void set(const Col<int> &idx, const MatVar &v) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly
        int* i = const_cast<int*>(_idims.memptr());
        int* ii = const_cast<int*>(idx.memptr());

        int iii = Mat_CalcSingleSubscript(_ndims, i,ii);

        set(idx, v);
      }

      /**
       * @brief Get element of matvar, using index
       *
       * @param i Index
       *
       * @return MatVar
       */
      MatVar get(const std::size_t i) {
        if(i >= _nelem) {
          cout << "i = " << i << ", nelem = " << _nelem << endl;
          throw matiocpp_exception("Cell's get out of bounds");
        }

        matvar_t *m = Mat_VarGetCell(_mptr, i);

        if(m == NULL) {
          throw matiocpp_exception("could not get cell");
        }

        MatVar mv(m);

        return mv;
      }

      /**
       * @brief Get matvar based on index vector
       *
       * @param idx Index vector
       *
       * @return Matvar
       */
      MatVar get(const Col<int> &idx) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly
        int* i = const_cast<int*>(_idims.memptr());
        int* ii = const_cast<int*>(idx.memptr());

        int iii = Mat_CalcSingleSubscript(_ndims, i,ii);

        return get(idx);
      }

      /**
       * @brief Get number of elements
       *
       * @return Number of elements
       */
      int nelems() const {
        return _nelem;
      }

      /**
       * @brief Get number of dimensions
       *
       * @return Number of dimensions
       */
      int ndims() const {
        return _ndims;
      }

    private:
      /**
       * @brief access matvar_ptr
       *
       * @return matvar_ptr
       */
      operator matvar_t*() const {
        return _mptr;
      }

      /**
       * @brief dereference as it was a matvar_ptr
       *
       * @return matvar_ptr
       */
      matvar_t* operator->() const {
        return _mptr;
      }

      friend class Struct;
        friend class Loader;
        friend class Saver;

    private:
      MatVar _mptr;
      int _ndims;
      Col<int> _idims;
      int _nelem;

      std::vector<std::size_t> _dims;
  };

  /**
   * @brief Struct class
   */
  class Struct {
    public:
      /**
       * @brief Struct class
       *
       * @param dims Vector of dimensions to create multi-dimensional array of structs
       * @param fields Fieldnames
       */
      Struct(const std::vector<std::size_t> &dims, const std::vector<std::string> &fields) : _fields(fields) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly
        std::size_t *d = const_cast<std::size_t*>(&dims[0]);

        // NOTE: Creating vector of dimensions with same content as
        //     dims as the API requires int* for subscript calc
        //     but std::size_t* for init.
        _ndims = dims.size();
        _idims = zeros<Col<int> >(_ndims);
        for(std::size_t i = 0; i < _ndims; ++i) {
          _idims[i] = dims[i];
        }

        _nelem = static_cast<int>(prod(_idims));
        _nfields = fields.size();

        std::vector<const char *> carr;
        carr.reserve(_nfields);
        for(std::size_t i = 0; i < _nfields; ++i) {
          carr.push_back(fields[i].c_str());
        }

        matvar_t *mm = Mat_VarCreateStruct(NULL, _ndims, d, &carr[0], _nfields);

        if(mm == NULL) {
          throw matiocpp_exception("Cant create struct");
        }

        _mptr = MatVar(mm);
      }

      /**
       * @brief Create a struct from a MatVar
       *
       * @param mv MatVar
       */
      Struct(MatVar &mv) : _mptr(mv) {
        if(mv->data_type != MAT_T_STRUCT || mv->class_type != MAT_C_STRUCT) {
          //FIXME: Should this be a regular exception?
          throw matiocpp_castexception("Cannot be cast to struct");
        }

        _ndims = _mptr->rank;

        _dims.resize(_ndims);
        _idims.set_size(_ndims);
        for(std::size_t i = 0; i < _ndims; ++i) {
          _dims[i] = _mptr->dims[i];
          _idims[i] = _mptr->dims[i];
        }
        _nelem = static_cast<int>(prod(_idims));

        _nfields = Mat_VarGetNumberOfFields(_mptr);
        if(_nfields < 0) {
          throw matiocpp_exception("Cant get number of fields");
        }

        // FIXME: Who frees this?
        char * const *nams = Mat_VarGetStructFieldnames(_mptr);

        if (nams == NULL) {
          throw matiocpp_exception("Cannot get fieldnames");
        }

        _fields.reserve(_nfields);
        for(std::size_t i = 0; i < _nfields; ++i) {
          _fields.push_back(nams[i]);
        }
      }

    public:
      /**
       * @brief Determines if struct has field
       *
       * @param fieldName Fieldname
       *
       * @return True if struct has field
       */
      bool hasfield(const char *fieldName) const {
        bool res = false;

        for(std::size_t i = 0; i < _nfields; ++i) {
          if(fieldName == _fields[i]) {
            res = true;
            break;
          }
        }

        return res;
      }

      /**
       * @brief Return number of elements
       *
       * @return Number of elements
       */
      int nelems() const {
        return _nelem;
      }

      /**
       * @brief Return number of fields
       *
       * @return Number of fields
       */
      int nfields() const {
        return _nfields;
      }

      /**
       * @brief Return vector of field names
       *
       * @return Vector of fieldnames
       */
      std::vector<std::string> fields() const {
        return _fields;
      }

      /**
       * @brief Set variable in struct
       *
       * @param fieldName Fieldname
       * @param i Index
       * @param v Matvar variable
       */
      void set(const char *fieldName, const std::size_t i, const MatVar &v) {
        if(i >= _nelem) {
          cout << "i = " << i << ", nelem = " << _nelem << endl;
          throw matiocpp_exception("Struct's set out of bounds");
        }

        matvar_t *m = Mat_VarSetStructFieldByName(_mptr, fieldName, i, v);

        // FIXME: Not checking return as it can either be: 1) first insert / error or 2) error
      }

      /**
       * @brief Get variable from struct
       *
       * @param fieldName Fieldname
       * @param i Index
       *
       * @return Matvar
       */
      MatVar get(const char *fieldName, const std::size_t i) {
        if(i >= _nelem) {
          cout << "i = " << i << ", nelem = " << _nelem << endl;
          throw matiocpp_exception("Struct's set out of bounds");
        }

        matvar_t *m = Mat_VarGetStructFieldByName(_mptr, fieldName, i);

        if (m == NULL) {
          throw matiocpp_exception("No such field");
        }

        MatVar mv(m);

        return mv;
      }

      /**
       * @brief Convert to MatVar
       *
       * @return MatVar
       */
      operator MatVar() const {
        return _mptr;
      }

    private:
      /**
       * @brief Returns matvar_t pointer
       *
       * @return matvar_t pointer
       */
      operator matvar_t*() const {
        return _mptr;
      }

      /**
       * @brief Dereference operator, to allow for using as matvar_t pointer
       *
       * @return matvar_t pointer
       */
      matvar_t* operator->() const {
        return _mptr;
      }

      friend class Cell;
      friend class Loader;
      friend class Saver;

    private:
      MatVar _mptr;
      std::vector<std::size_t> _dims;
      int _ndims;
      int _nelem;
      Col<int> _idims;
      std::vector<std::string> _fields;
      int _nfields;
  };

  MatVar::operator Cell() const {
    // NOTE: Not checking type here like for the other MatVar operators, as its checked by
    //     the constructor of Cell
    MatVar mv(_matvar);
    return Cell(mv);
  }

  MatVar::operator Struct() const {
    // NOTE: Not checking type here like for the other MatVar operators, as its checked by
    //     the constructor of Struct
    MatVar mv(_matvar);
    return Struct(mv);
  }

  /**
   * @brief Saver for matlab files
   */
  class Saver {
    public:
       /**
       * @brief Saver constructor
       *
       * @param matFilename name of file
       * @param hdr header to save
       * @param fmt format type, defaults to matio library default
       */
      Saver(const char *matFilename, const char *hdr=NULL, mat_ft fmt=MAT_FT_DEFAULT) : _matfp(0) {
        _matFilename = matFilename;
        _matfp = Mat_Open(_matFilename, MAT_ACC_RDWR);
        // _matfp = Mat_CreateVer(_matFilename, hdr, fmt);

        if(!_matfp) {
          _matfp = Mat_CreateVer(_matFilename, hdr, fmt);
          if(!_matfp) {
            throw matiocpp_exception("Could not open file");
          }
        }
      }

      /**
       * @brief Close saver and file
       */
      ~Saver() {
        if(!_matfp) {
          Mat_Close(_matfp);
        }
      }

    public:
      /**
       * @brief save variable to file
       *
       * @param varName variable name
       * @param mc matlab variable
       * @param compress if it should be compressed or not
       *
       * @return
       */
      // int save(const char *varName, const MatVar &mc, matio_compression compress = MAT_COMPRESSION_NONE) {
      int save(const char *varName, const MatVar &mc, matio_compression compress = MAT_COMPRESSION_ZLIB) {
        // NOTE: Potentially dangerous, but dealing with C-api that expects non-const pointers, despite using them readonly

        mc->name = const_cast<char*>(varName);
        // cout << "Saving variable: " << mc->name <<endl;

        if (hasvariable(mc->name)) {
          // cout << "Deleting existing variable before saving: " << mc->name <<endl;
          int resDel = Mat_VarDelete(_matfp, mc->name);
          if (resDel != 0) {
            throw matiocpp_exception("Could not delete the variable");
          }
          else {
            if (hasvariable(mc->name)) {
              cout << "Saving was inefficient: previously stored values were not deleted" <<endl;
            }
          }
        }

        int res = Mat_VarWrite(_matfp, mc, compress);
        Mat_Close(_matfp);
        _matfp = Mat_Open(_matFilename,MAT_ACC_RDWR);

        return res;
      }

    private:
      /**
       * @brief Returns if file has variable
       *
       * @param varName Variable name
       *
       * @return true if variable name exists
       */
      bool hasvariable(const char *varName) {
        matvar_t *var = Mat_VarReadInfo(_matfp, varName);
        // cout << "mat file has variable: " << varName << endl;

        return var!=NULL;
      }

    private:
      mat_t *_matfp;
      const char *_matFilename;
  };

  /**
   * @brief Loader class
   */
  class Loader {
    public:
       /**
       * @brief Load matlab file
       *
       * @param matFilename matlab filename
       */
      Loader(const char *matFilename) : _matfp(0) {
        _matFilename = matFilename;
        _matfp = Mat_Open(_matFilename, MAT_ACC_RDONLY);

        if(!_matfp) {
          throw matiocpp_exception("Could not open file");
        }
      }

      /**
       * @brief Close matlab file
       */
      ~Loader() {
        if(!_matfp) {
          Mat_Close(_matfp);
        }
      }

    public:
      /**
       * @brief Loads variable from file, throws an exception if it does not exist
       *
       * @param varName Variable name
       *
       * @return variable
       */
      MatVar load(const char *varName) {
        matvar_t *var = Mat_VarRead(_matfp, varName);

        if (var == NULL) {
          throw matiocpp_exception("No such variable");
        }

        MatVar mv(var);

        return mv;
      }

    public:
      /**
       * @brief Returns if file has variable
       *
       * @param varName Variable name
       *
       * @return true if variable name exists
       */
      bool hasvariable(const char *varName) {
        matvar_t *var = Mat_VarReadInfo(_matfp, varName);

        return var!=NULL;
      }

    private:
      mat_t *_matfp;
      const char *_matFilename;
  };
}

#endif
