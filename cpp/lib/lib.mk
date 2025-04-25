.PHONY: all clean

all:
	$(MAKE) -C lib/math -f math.mk
	$(MAKE) -C lib/shape -f shape.mk

clean:
	$(MAKE) -C lib/math -f math.mk clean
	$(MAKE) -C lib/shape -f shape.mk clean