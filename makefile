cipher: cipher.c
	gcc cipher.c -o cipher
cipher.o: cipher.c
breaker: Cipherbreaker.c
	gcc Cipherbreaker.c -o breaker -std=c99

test: test1

test1:
	./cipher 1 5 Testdoc.txt EncryptTest.txt
	./breaker EncryptTest.txt output.txt
	diff -s Testdoc.txt output.txt
