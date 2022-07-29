.PHONY: all burn dump verify clean 

PROGRAM := hello-world

EEPROM := AT28C256

all: burn

# assemble the source
output/rom.bin: src/$(PROGRAM).s
	vasm6502_oldstyle -Fbin -dotdir "$<" -o "$@"

# burn the assembled program onto the EEPROM
burn: output/rom.bin verify
	minipro -p $(EEPROM) -w $<

# read the data from the EEPROM into a file
dump: verify
	minipro -p $(EEPROM) -r output/dump.bin

# ensure that the EEPROM is working and connected properly
verify:
	minipro -p $(EEPROM) -z

# clean the output directory
clean:
	rm -f output/*.bin
