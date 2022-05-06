all: USA PAL JAP KOR TWN
	
Syati:
	git clone https://github.com/Lord-Giganticus/Syati.git
	rm Syati/source/ExtendedActorFactory.cpp
	cp -R source/. Syati/source
	cp -R include/. Syati/include
	
USA: Syati
	cd Syati; make $@
	mv Syati/CustomCode_$@.bin CustomCode_$@.bin

PAL: Syati
	cd Syati; make $@
	mv Syati/CustomCode_$@.bin CustomCode_$@.bin

JAP: Syati
	cd Syati; make $@
	mv Syati/CustomCode_$@.bin CustomCode_$@.bin

KOR: Syati
	cd Syati; make $@
	mv Syati/CustomCode_$@.bin CustomCode_$@.bin

TWN: Syati
	cd Syati; make $@
	mv Syati/CustomCode_$@.bin CustomCode_$@.bin

clean:
	rm -rf Syati CustomCode*