#/****************************************************************************
#** Authors: Dario Menasce, Stefano Terzo
#**
#** I.N.F.N. Milan-Bicocca
#** Piazza  della Scienza 3, Edificio U2
#** Milano, 20126
#**
#****************************************************************************/

PROGRAMS = ReaderTemplate Converter

all: $(PROGRAMS)

obj/Data.o:     src/Data.cpp                         	           \
                inc/Data.h			    	           
	@echo ' '
	@echo '------------- Compiling Data.cpp  '
	g++ -c                                                     \
            -Iinc                                                  \
            -o obj/Data.o                                          \
               src/Data.cpp                                        \
               `root-config --cflags`      
                       
obj/EventReader.o:     src/EventReader.cpp			   \
                       inc/EventReader.h			   \
                       ${MonicelliDir}/include/Event.h             \
                       ${MonicelliDir}/include/EventHeader.h       \
                       ${MonicelliDir}/include/Geometry.h          \
                       ${MonicelliDir}/include/MessageTools.h   
	@echo ' '
	@echo '------------- Compiling EventReader.cpp  '
	g++ -c                                                     \
            -I${MonicelliDir}/include                              \
            -Iinc                                                  \
            -o obj/EventReader.o				   \
               src/EventReader.cpp				   \
               `root-config --cflags`      
                       
obj/EventAnalyzer.o:   src/EventAnalyzer.cpp			   \
                       inc/EventAnalyzer.h			   \
                       inc/EventReader.h			   \
                       ${MonicelliDir}/include/Event.h             \
                       ${MonicelliDir}/include/EventHeader.h       \
                       ${MonicelliDir}/include/Geometry.h          \
                       ${MonicelliDir}/include/MessageTools.h   
	@echo ' '
	@echo '------------- Compiling EventAnalyzer.cpp  '
	g++ -c                                                     \
            -I${MonicelliDir}/include                              \
            -Iinc                                                  \
            -o obj/EventAnalyzer.o				   \
               src/EventAnalyzer.cpp				   \
               `root-config --cflags`      
                       
obj/EventConverter.o:  src/EventConverter.cpp                      \
                       inc/EventConverter.h                        \
                       inc/EventReader.h                           \
                       inc/Data.h                                  \
                       ${MonicelliDir}/include/Event.h             \
                       ${MonicelliDir}/include/EventHeader.h       \
                       ${MonicelliDir}/include/Geometry.h          \
                       ${MonicelliDir}/include/MessageTools.h   
	@echo ' '
	@echo '------------- Compiling EventConverter.cpp  '
	g++ -c                                                     \
            -I${MonicelliDir}/include                              \
            -Iinc                                                  \
            -o obj/EventConverter.o				   \
               src/EventConverter.cpp				   \
               `root-config --cflags`      
                       
ReaderTemplate:        obj/ReaderTemplate.o			   \
                       obj/EventReader.o			   \
                       obj/EventAnalyzer.o			   \
                       obj/Detector.o                              \
                       obj/ROC.o                                   \
                       obj/Event.o                                 \
                       obj/EventDict.o                             \
                       obj/EventHeader.o                           \
                       obj/EventHeaderDict.o                       \
                       obj/Geometry.o
	@echo ' '
	@echo '------------- Creating ReaderTemplate executable '
	g++ -o ReaderTemplate                                      \
               obj/ReaderTemplate.o				   \
               obj/EventReader.o				   \
               obj/EventAnalyzer.o				   \
               obj/Detector.o                                      \
               obj/Geometry.o                                      \
               obj/ROC.o                                           \
               obj/Event.o                                         \
               obj/EventDict.o                                     \
               obj/EventHeader.o                                   \
               obj/EventHeaderDict.o                               \
               `root-config --libs`                         


Converter:       obj/Converter.o                        	   \
                 obj/EventReader.o			     	   \
                 obj/EventConverter.o			     	   \
                 obj/Detector.o 			     	   \
                 obj/Data.o 	    		     	           \
                 obj/ROC.o				     	   \
                 obj/Event.o				     	   \
                 obj/EventDict.o			     	   \
                 obj/EventHeader.o			     	   \
                 obj/EventHeaderDict.o  		     	   \
                 obj/Geometry.o
	@echo ' '
	@echo '------------- Creating ReaderTemplate executable '
	g++ -o Converter                                           \
               obj/Converter.o  				   \
               obj/EventReader.o				   \
               obj/EventConverter.o				   \
               obj/Data.o					   \
               obj/Detector.o                                      \
               obj/Geometry.o                                      \
               obj/ROC.o                                           \
               obj/Event.o                                         \
               obj/EventDict.o                                     \
               obj/EventHeader.o                                   \
               obj/EventHeaderDict.o                               \
               `root-config --libs`                         


obj/ReaderTemplate.o:  src/ReaderTemplate.cpp			   \
                       inc/EventReader.h			   \
                       ${MonicelliDir}/include/Event.h             \
                       ${MonicelliDir}/include/EventHeader.h       \
                       ${MonicelliDir}/include/Geometry.h          \
                       ${MonicelliDir}/include/MessageTools.h   
	@echo ' '
	@echo '------------- Compiling ReaderTemplate.cpp  '
	g++ -c                                                     \
            -I${MonicelliDir}/include                              \
            -Iinc                                                  \
            -o obj/ReaderTemplate.o				   \
               src/ReaderTemplate.cpp				   \
               `root-config --cflags`      
        
obj/Converter.o:  src/Converter.cpp                           	   \
                  inc/Data.h			            	   \
                  inc/EventReader.h			      	   \
                  ${MonicelliDir}/include/Event.h	      	   \
                  ${MonicelliDir}/include/EventHeader.h       	   \
                  ${MonicelliDir}/include/Geometry.h	      	   \
                  ${MonicelliDir}/include/MessageTools.h   
	@echo ' '
	@echo '------------- Compiling Converter.cpp  '
	g++ -c                                                     \
            -I${MonicelliDir}/include                              \
            -Iinc                                                  \
            -o obj/Converter.o  			      \
               src/Converter.cpp			      \
               `root-config --cflags`      
        
obj/Event.o:           ${MonicelliDir}/src/Event.cpp               \
	               ${MonicelliDir}/include/MessageTools.h      \
	               ${MonicelliDir}/include/ANSIColors.h
	@echo ' '
	@echo '------------- Compiling Event '
	g++ -c                                                     \
            -I${MonicelliDir}/include                              \
            -o obj/Event.o                                         \
               ${MonicelliDir}/src/Event.cpp                       \
               `root-config --cflags`
                                       
obj/Geometry.o:        ${MonicelliDir}/include/Detector.h          \
                       ${MonicelliDir}/include/MessageTools.h      \
                       ${MonicelliDir}/include/ANSIColors.h
	@echo ' '
	@echo '------------- Compiling Geometry '
	g++ -c                                                     \
            -I${MonicelliDir}/include                              \
            -o obj/Geometry.o                                      \
               ${MonicelliDir}/src/Geometry.cpp                    \
               `root-config --cflags`        
                                       
obj/Detector.o:        ${MonicelliDir}/src/Detector.cpp            \
                       ${MonicelliDir}/include/ROC.h               \
                       ${MonicelliDir}/include/MessageTools.h      \
                       ${MonicelliDir}/include/ANSIColors.h
	@echo ' '
	@echo '------------- Compiling Detector '
	g++ -c                                                     \
            -I${MonicelliDir}/include                              \
            -o obj/Detector.o                                      \
               ${MonicelliDir}/src/Detector.cpp                    \
               `root-config --cflags`      
                                       
obj/ROC.o:             ${MonicelliDir}/src/ROC.cpp                 \
                       ${MonicelliDir}/include/MessageTools.h      \
                       ${MonicelliDir}/include/ANSIColors.h
	@echo ' '
	@echo '------------- Compiling ROC '
	g++ -c                                                     \
            -I${MonicelliDir}/include                              \
            -o obj/ROC.o                                           \
               ${MonicelliDir}/src/ROC.cpp                         \
               `root-config --cflags`                                                                                                  
                                       
obj/EventHeader.o:     ${MonicelliDir}/src/EventHeader.cpp
	@echo ' '
	@echo '------------- Compiling EventHeader.cpp '
	g++ -c                                                     \
            -I${MonicelliDir}/include                              \
            -o obj/EventHeader.o                                   \
               ${MonicelliDir}/src/EventHeader.cpp                 \
               `root-config --cflags`   

obj/EventHeaderDict.o: obj/EventHeaderDict.C
	@echo ' '
	@echo '------------- Compiling EventHeader Dictionary '
	g++ -c                                                     \
            -o obj/EventHeaderDict.o                               \
               obj/EventHeaderDict.C                               \
               `root-config --cflags`
        
obj/EventHeaderDict.C: ${MonicelliDir}/include/EventHeader.h             
                       
	@echo ' '
	@echo '------------- Building EventHeader Dictionary '
	rm -f     obj/EventHeaderDict.* &&                         \
        rootcint  obj/EventHeaderDict.C                            \
                  -c ${MonicelliDir}/include/EventHeader.h+        

obj/EventDict.o: obj/EventDict.C
	@echo ' '
	@echo '------------- Compiling Event Dictionary '
	g++ -c                                                     \
            -o obj/EventDict.o                                     \
               obj/EventDict.C                                     \
               `root-config --cflags`
        
obj/EventDict.C:       ${MonicelliDir}/include/Event.h             \
                       ${MonicelliDir}/include/Geometry.h          \
                       ${MonicelliDir}/include/Detector.h          \
                       ${MonicelliDir}/include/ROC.h
	@echo ' '
	@echo '------------- Building Event Dictionary '
	rm -f     obj/EventDict.* &&                               \
        rootcint  obj/EventDict.C                                  \
                  -c ${MonicelliDir}/include/Event.h+              \
                     ${MonicelliDir}/include/Geometry.h+           \
                     ${MonicelliDir}/include/Detector.h+           \
                     ${MonicelliDir}/include/ROC.h+
        
clean: 
	@echo ' '
	@echo '------------- Clean up '
	rm -f obj/* $(PROGRAMS) 
