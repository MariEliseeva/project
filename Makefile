main: main.o avr.o readValues.o doAction.o getValue.o setValue.o expression.o
	g++ -g -std=c++11 -pedantic -Wall -Wextra -Werror bin/main.o bin/avr.o bin/readValues.o bin/doAction.o bin/getValue.o bin/setValue.o bin/expression.o -o avr
main.o: src/main.cpp
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/main.cpp -o bin/main.o
avr.o: src/avr.cpp src/readValues.cpp
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/avr.cpp -o bin/avr.o
readValues.o: src/readValues.cpp
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/readValues.cpp -o bin/readValues.o
doAction.o: src/doAction.cpp
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/doAction.cpp -o bin/doAction.o
getValue.o: src/getValue.cpp
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/getValue.cpp -o bin/getValue.o
setValue.o: src/setValue.cpp
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/setValue.cpp -o bin/setValue.o
expression.o: src/expression.cpp
	g++ -c -g -std=c++11 -pedantic -Wall -Wextra -Werror -Iinclude src/expression.cpp -o bin/expression.o
clean:
	rm *.o ./avr
