cpp-ndarray
===========
This is a thin wrapper around numpy.ndarray for boost.python

Multidimensional array indexing uses operator() with variadic templates for C++11 mode or with va\_args for C++03 mode.

C++11 mode is detected via boost/config/compiler framework.

With variadic templates, some runtime checking is preformed on array size and number of dimensions. This can be disabled by defining ndarr\_assert(x) before including ndarray.h, which can yield a small performance benefit, but increase chance of memory corruption due to programmer errors.

Code for using boost::python::extract for numpy.ndarray import can be enabled by defining NUMPY\_EXTRACT, but one should define

```c++
template<typename T>
boost::python::handle<> array_from_python_str<T>::array_type;
```

in some cpp file and initialize converter by calling

```c++
array_from_python_str<T>("numpy","ndarray");
```
