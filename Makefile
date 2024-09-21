unit_test:
	gcc -fprofile-arcs -ftest-coverage test/Testunit.c src/function.c unity/unity.c -o testunit -lm
integration_test:
	gcc -fprofile-arcs -ftest-coverage test/Testintegration.c src/function.c unity/unity.c -o testintegration -lm
property_test:
	gcc -fprofile-arcs -ftest-coverage test/Testproperty.c src/function.c unity/unity.c -o testproperty -lm
install: 
	gcc src/main.c src/function.c -o main -lm



