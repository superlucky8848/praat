# Makefile of the library "speechocean"
# Shen Long, 26 June 2020

include ../makefile.defs

CPPFLAGS = -I ../kar -I ../melder -I ../sys -I ../dwsys -I ../stat -I ../dwtools -I ../fon

OBJECTS = speechocean.o

.PHONY: all clean

all: speechocean.a

clean:
	$(RM) $(OBJECTS)
	$(RM) speechocean.a

speechocean.a: $(OBJECTS)
	touch speechocean.a
	rm speechocean.a
	$(AR) cq speechocean.a $(OBJECTS)
	$(RANLIB) speechocean.a

$(OBJECTS): *.h ../kar/*.h ../melder/*.h ../sys/*.h ../dwsys/*.h ../stat/*.h ../dwtools/*.h ../fon/*.h
