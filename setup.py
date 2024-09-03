from setuptools import Extension, setup 
import numpy


module = Extension('mykmeanssp',
                   sources=['kmeansmodule.c'],
                   include_dirs=[numpy.get_include()])

setup(name='mykmeanssp',
      version='1.0',
      description='K-means clustering algorithm with K-means++ initialization',
      ext_modules=[module])