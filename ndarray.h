#ifndef NDARRAY_H
#define NDARRAY_H

#include <boost/python.hpp>
#include <boost/config/select_compiler_config.hpp>
#include BOOST_COMPILER_CONFIG

#ifdef BOOST_HAS_VARIADIC_TMPL
#pragma message ("Using variadic templates")
#else
#pragma message ("NOT using variadic templates")
#endif

#ifndef ndarr_assert
#define ndarr_assert(x) assert(x)
#endif
//#define ndarr_assert(x) //no array_ptr.operator() argument check AT ALL. Use at your own risk.

template<typename T>
class array_ptr
{
private:
    struct array_struct
    {
        PyObject_HEAD
        char* data;
        int nd;
        Py_intptr_t * shape;
        Py_intptr_t * strides;
        PyObject * base;
        PyObject * descr;
        int flags;
        PyObject * weakreflist;
    };

    typedef unsigned long long int idx_t;

    array_struct* ptr;
    idx_t size;

#ifdef BOOST_HAS_VARIADIC_TMPL
    inline idx_t index(int stridesidx __attribute__((unused))) {
        ndarr_assert(stridesidx==ptr->nd);
        return 0;
    }

    template<typename ... Types>
    inline idx_t index(int stridesidx, int i, Types... rest) {
        ndarr_assert(stridesidx<ptr->nd);
        return i*ptr->strides[stridesidx]+index(stridesidx+1,rest...);
    }
#endif
public:
    array_ptr(const boost::python::object &obj) : ptr(reinterpret_cast<array_struct*>(obj.ptr())) {
        size=ptr->strides[0]*ptr->shape[0];
    }
    array_struct* operator ->() {
        return ptr;
    }

#ifndef BOOST_HAS_VARIADIC_TMPL
    inline T& operator()(int i0, ...) {
        va_list vl;
        va_start(vl,i0);
        idx_t int idx=i0*ptr->strides[0];
        for(int i=1;i<ptr->nd;++i)
            idx+=va_arg(vl,int)*ptr->strides[i];
        va_end(vl);
#else
    template<typename ... Types>
    inline T& operator()(Types... indexes) {
        idx_t idx=index(0,indexes...);
#endif
        ndarr_assert(idx<size);
        return *reinterpret_cast<T*>(ptr->data+idx);
    }
};

#ifdef NUMPY_EXTRACT
#warning "Using numpy extract, please define"
#warning "template<typename T>"
#warning "boost::python::handle<> array_from_python_str<T>::array_type;"
#warning "in a cpp file that includes ndarray.h"
//like this:
//template<typename T>
//boost::python::handle<> array_from_python_str<T>::array_type;
template<typename T>
struct array_from_python_str
{
    static boost::python::handle<> array_type;

    array_from_python_str(std::string module_name, std::string type_name)
    {
        using namespace boost::python;
        PyObject* module = ::PyImport_Import(object(module_name).ptr());
        if (module)
        {
            PyObject* type = ::PyObject_GetAttrString(module, const_cast<char*>(type_name.c_str()));

            if (type && PyType_Check(type))
            {
                array_type = handle<>(type);

                converter::registry::push_back(
                            &convertible,
                            &construct,
                            boost::python::type_id<array_ptr<T> >());
            }
        }
    }

    // Determine if obj_ptr can be converted in a QString
    static void* convertible(PyObject* obj_ptr)
    {
        if (PyObject_IsInstance(obj_ptr,array_type.get())==0) return 0;
        return obj_ptr;
    }

    // Convert obj_ptr into a QString
    static void construct(
            PyObject* obj_ptr,
            boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        // Grab pointer to memory into which to construct the new QString
        void* storage = (
                    (boost::python::converter::rvalue_from_python_storage<array_ptr<T> >*)
                    data)->storage.bytes;

        // in-place construct the new QString using the character data
        // extraced from the python object
        // Use borrowed to construct the object so that a reference
        // count will be properly handled.
        boost::python::handle<> hndl(boost::python::borrowed(obj_ptr));
        new (storage) array_ptr<T>(boost::python::object(hndl));

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};
#endif

#endif // NDARRAY_H
