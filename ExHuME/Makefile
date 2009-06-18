EXHUMEDIR = ${HOME}/scratch0/ExHuME/ExHuME-1.3.4/ExHuME-1.3.4
CLHEPDIR = /afs/cern.ch/cms/sw/slc4_amd64_gcc345/external/clhep/1.9.4.2-cms
LHAPDFDIR = /afs/cern.ch/cms/sw/slc4_amd64_gcc345/external/lhapdf/5.6.0-cms2
PYTHIA6DIR = /afs/cern.ch/cms/sw/slc4_amd64_gcc345/external/pythia6/419
HEPMCDIR = /afs/cern.ch/cms/sw/slc4_amd64_gcc345/external/hepmc/2.03.06-cms3

EXHUMELIB = -L$(EXHUMEDIR)/lib -lExHuME
CLHEPLIB = -L$(CLHEPDIR)/lib -lCLHEP
LHAPDFLIB = -L$(LHAPDFDIR)/lib -lLHAPDF
PYTHIA6LIB = -L$(PYTHIA6DIR)/lib -lpythia6 -lpythia6_dummy $(PYTHIA6DIR)/lib/pydata.o
HEPMCLIB = -L$(HEPMCDIR)/lib -lHepMCfio -lHepMC

LIBS = $(EXHUMELIB) $(CLHEPLIB) $(LHAPDFLIB) $(PYTHIA6LIB) $(HEPMCLIB)
 
INCLUDES = -I$(EXHUMEDIR)/include/ExHuME -I$(CLHEPDIR)/include -I$(HEPMCDIR)/include

OPT = -funroll-loops -O3 -Wall

FSOURCES  = $(shell ls ./src/*.f)
CPPSOURCES = $(shell ls ./src/*.cpp)
FOBJS     = $(FSOURCES:.f=.o)
CPPOBJS   = $(CPPSOURCES:.cpp=.o)
CC	  = gcc	
FF	  = g77

%.o:		%.cpp
		$(CC) -c $(OPT) \
		$(INCLUDES) \
		$^ -o $@

%.o:		%.f
		$(FF) -c $(OPT) $^ -o $@

GGMain: GGMain.o FortranTools.o
	g++ $(OPT) $^ $(LIBS) -lg2c -o $@

GGMain_HepMC: GGMain_HepMC.o
	g++ $(OPT) $^ $(LIBS) -lg2c -o $@

GGMain_HepMC_ascii: GGMain_HepMC_ascii.o
	g++ $(OPT) $^ $(LIBS) -lg2c -o $@

DiPhotonMain:   DiPhotonMain.o FortranTools.o
		g++ $(OPT) $^ $(LIBS) -lg2c -o $@

readHepMC: readHepMC.o
	g++ $(OPT) $^ $(LIBS) -lg2c -o $@

all: GGMain GGMain_HepMC GGMain_HepMC_ascii DiPhotonMain readHepMC

PDFsets:	
		ln -s $(LHAPDFDIR)/PDFsets ./PDFsets

clean:
		rm *.o
