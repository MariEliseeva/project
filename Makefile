main: main.o avr.o readValues.o doAction.o getValue.o setValue.o expression.o
	mkdir -p bin
	g++ -g -std=c++11 -pedantic -Wall -Wextra -Werror bin/main.o bin/avr.o bin/readValues.o bin/doAction.o bin/getValue.o bin/setValue.o bin/expression.o -o avr
main.o: src/main.cpp
	mkdir -p bin
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/main.cpp -o bin/main.o
avr.o: src/avr.cpp src/readValues.cpp
	mkdir -p bin
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/avr.cpp -o bin/avr.o
readValues.o: src/readValues.cpp
	mkdir -p bin
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/readValues.cpp -o bin/readValues.o
doAction.o: src/doAction.cpp
	mkdir -p bin
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/doAction.cpp -o bin/doAction.o
getValue.o: src/getValue.cpp
	mkdir -p bin
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/getValue.cpp -o bin/getValue.o
setValue.o: src/setValue.cpp
	mkdir -p bin
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/setValue.cpp -o bin/setValue.o
expression.o: src/expression.cpp
	mkdir -p bin
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/expression.cpp -o bin/expression.o

test: test.o avr.o readValues.o doAction.o getValue.o setValue.o expression.o Test.o avr_test.o
	mkdir -p bin
	g++ -std=c++11 -Iinclude -pedantic -Wall -Wextra -Werror bin/avr_test.o bin/Test.o bin/test.o bin/avr.o bin/readValues.o bin/doAction.o bin/getValue.o bin/setValue.o bin/expression.o -o bin/test
avr_test.o: test/avr_test.cpp
	mkdir -p bin
	g++ -c -std=c++11 -Iinclude -pedantic -Wall -Wextra -Werror test/avr_test.cpp -o bin/avr_test.o
Test.o: test/Test.cpp
	mkdir -p bin
	g++ -c -std=c++11 -Iinclude -pedantic -Wall -Wextra -Werror test/Test.cpp -o bin/Test.o
test.o: test/test.cpp
	mkdir -p bin
	g++ -c -std=c++11 -Iinclude -pedantic -Wall -Wextra -Werror test/test.cpp -o ./bin/test.o

clean:
	rm bin/*.o ./avr ./bin/test



