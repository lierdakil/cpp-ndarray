import sys
import os
from numpy.random.mtrand import random_integers, rand

from time import clock

sys.path.append(os.path.dirname(os.path.abspath(__file__)))


import math

import libmm
import numpy

I=random_integers(1,10)
J=random_integers(1,10)
K=random_integers(1,10)
I=J=K=100

start=clock()
# write test
a=numpy.require(numpy.empty([I]),requirements=['A','W','C'])
assert(libmm.test_write(a)==0)
for i in range(a.shape[0]):
    assert a[i]==i

a=numpy.require(numpy.empty([I,J]),requirements=['A','W','C'])
assert(libmm.test_write(a)==0)
for i in range(a.shape[0]):
    for j in range(a.shape[1]):
        assert a[i,j]==j*100+i

a=numpy.require(numpy.empty([I,J,K]),requirements=['A','W','C'])
assert(libmm.test_write(a)==0)
for i in range(a.shape[0]):
    for j in range(a.shape[1]):
        for k in range(a.shape[2]):
            assert a[i,j,k]==10000*k+j*100+i

print(clock() - start)

start=clock()
# read test
a=rand(I)
b=numpy.copy(a)
for i in range(a.shape[0]):
            assert libmm.test_read(b,i,0,0)==a[i]

a=rand(I,J)
b=numpy.copy(a)
for i in range(a.shape[0]):
    for j in range(a.shape[1]):
            assert libmm.test_read(b,i,j,0)==a[i,j]

a=rand(I,J,K)
b=numpy.copy(a)
for i in range(a.shape[0]):
    for j in range(a.shape[1]):
        for k in range(a.shape[2]):
            assert libmm.test_read(b,i,j,k)==a[i,j,k]

print(clock() - start)
