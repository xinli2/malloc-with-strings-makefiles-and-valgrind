
all:rollingStrings

rollingStrings:rollingStrings.o
	gcc rollingStrings.o -o rollingStrings -Wall -g -std=gnu99

rollingStrings.o:rollingStrings.c
	gcc rollingStrings.c -c -Wall -g -std=gnu99

clean:
	rm -fr rollingStrings rollingStrings.o
