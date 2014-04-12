#ifndef U_BOND_H
#define U_BOND_H

#include "ndarray.h"

#include <boost/python.hpp>
//#include "boost/python/numeric.hpp"
//#include "boost/python/extract.hpp"

using namespace boost::python;

#include <iostream>

double test_write(object arr1) {
#ifdef NUMPY_EXTRACT
    array_ptr<double> arr = extract<array_ptr<double> >(arr1);
#else
    array_ptr<double> arr(arr1);
#endif
    if(arr->nd==1) {
        for(int i=0; i<arr->shape[0];++i)
            arr(i)=i;
        return arr(0);
    }
    else if(arr->nd==2) {
        for(int i=0; i<arr->shape[0];++i)
            for(int j=0;j<arr->shape[1];++j)
                arr(i,j)=100*j+i;
        return arr(0,0);
    } else if(arr->nd==3) {
        for(int i=0; i<arr->shape[0];++i)
            for(int j=0;j<arr->shape[1];++j)
                for(int k=0;k<arr->shape[2];++k)
                    arr(i,j,k)=10000*k+100*j+i;
        return arr(0,0,0);
    } else
        return -1;
}

#ifdef NUMPY_EXTRACT
double test_read(array_ptr<double> arr, int i, int j=0, int k=0) {
#else
double test_read(object arr1, int i, int j=0, int k=0) {
    array_ptr<double> arr(arr1);
#endif
    if(arr->nd==1) {
        return arr(i);
    }
    else if(arr->nd==2) {
        return arr(i,j);
    } else if(arr->nd==3) {
        return arr(i,j,k);
    } else
        return -1;
}




#endif // U_BOND_H
