unit_test:
	gcc test/Testunit.c src/function.c unity/unity.c -o testunit -lm
integration_test:
	gcc test/Testintegration.c src/function.c unity/unity.c -o testintegration -lm




