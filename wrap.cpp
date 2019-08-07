//#include <boost/numpy.hpp>
//#define ndarr_assert(x)
#include <u_bond.h>

#include <iostream>

#ifdef NUMPY_EXTRACT
template<typename T>
boost::python::handle<> array_from_python_str<T>::array_type;
#endif

BOOST_PYTHON_MODULE(libmm) {
#ifdef NUMPY_EXTRACT
    array_from_python_str<double>("numpy","ndarray");
#endif

    def("test_write", &test_write);
    def("test_read", &test_read);
}
