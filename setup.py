from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

ext_modules = [
    Extension( "invasion",
               sources=["invasion-wrapper.pyx", "invasion.c"],
               include_dirs = ["/opt/vc/include"],
               libraries = [ "bcm_host" ],
               library_dirs = ["/opt/vc/lib"]
    )
]


    
setup(
    name = "invasion",
    ext_modules = cythonize( ext_modules )
)
