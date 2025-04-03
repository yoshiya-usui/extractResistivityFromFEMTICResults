CXX           = icpc
CC            = icpc
CXXFLAGS      = -O3 \
                -D_LINUX \
                -DNDEBUG 
DEST          = ./
OBJS          = ExtractResistivityFromPointsList.o \
                MeshData.o \
                MeshDataTetraElement.o \
                MeshDataBrickElement.o \
                MeshDataNonConformingHexaElement.o \
                ResistivityBlock.o \
                Util.o
PROGRAM       = extractResistivityFromFEMTICResults

all:            $(PROGRAM)

$(PROGRAM):     $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

clean:;		rm -f *.o *~ $(PROGRAM)
