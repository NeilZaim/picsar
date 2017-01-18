# **Installation of the PICSAR Python module on MacOS**

The Python version of PICSAR has been developed for the PIC code WARP.

## **1. Installing WARP**

In order to install WARP, you can use the WARP installation documentation.
Download WARP from the [bitbucket repository](https://bitbucket.org/berkeleylab/warp).
Instructions are located in the sources in the folder `doc`.

## **2. Installing python and packages for PICSAR**

If you have already installed Warp, 
this step (installing python and useful package) is already done.
If not, we simply recommend to first install Warp.

Install python 2.7.
We recommend python from Anaconda (`http://docs.continuum.io/anaconda/install`)

Then, install numpy

```
pip install numpy
```

Finally, install mpi4py to be able to use MPI with Python.

```
pip install mpi4py
```

## **3. Installing Forthon**

If you have already installed Warp, this step is already done.

Before creating the python module picsarpy for picsar, 
you must install the Forthon compiler. 
To do so: 

* Copy the last stable version of Forthon by typing:
```
git clone https://github.com/dpgrote/Forthon.git
```

* Then `cd` into the directory `Forthon` and run python `setup.py install --home=$PATH`

Here, `PATH` is where you want the `bin` and `lib` folder to be created 
and the Forthon files to be located.

NB: **On the cluster Edison at NERSC**:  
Simply type `pip install Forthon --user`

## **4. Makefile_Forthon configuration**

Clone `picsar` where you want to install it.

In order to automatically configure the installation, type
```
./configure
```

Then type
```
make -f Makefile_Forthon
```

**If the configure step failed:**   
You will need to edit the file `Makefile_Forthon` and indicate the following environment variables:

- FCOMP: your fortran compiler (e.g gfortran),

- FCOMPEXEC: your MPI Fortran wrapper (e.g mpif90),

- FARGS: arguments of the $(FCOMP) compiler. To get OpenMP version of PICSAR use the flag -fopenmp (with gfortran) and -openmp (Cray, Intel). NB: this version of PICSAR requires at least **OpenMP 4.0**.  

- LIBDIR: your library folder containing MPI libraries (e.g /usr/local/Cellar/open-mpi/1.8.6/lib/ for an Homebrew install of open-mpi on MACOSX, /opt/local/lib/mpich-mp/ for a Macports install of mpich),

- LIBS: required libraries for the install. With Open-MPI, the compilation of picsar requires the following libraries: -lmpi, -lmpi_usempi, -lmpi_mpifh, -lgomp. For open-mpi>1.8.x, you should use -lmpi_usempif08 instead of -lmpi_usempi. For a Macports install of mpich, you should use -lmpifort -lmpi -lpmpi.   



## **5. Compiling and installing**


To compile, invoke the rule "all": 
```
make -f Makefile_Forthon all
```
Then make sure that the folders `python_libs` and `python_bin` are in
your `$PYTHONPATH`.

On Edison and Cori, this is ensured by the setup of your `~/.bashrc.ext`.

## **6. Testing**

Testing the code after compilation is highly recommended.

To test the compilation/execution, you can use the makefile (py.test is required):

  For all test:
  - `make -f Makefile_Forthon test`

  For each test one by one
  - Propagating beams:       `make -f Makefile_Forthon test1`
  - Langmuir wave:           `make -f Makefile_Forthon test2`
