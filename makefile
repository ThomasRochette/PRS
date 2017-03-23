CC = gcc
flag = -Wall


all : client serveur clientUDP

clientUDP : clientUDP.o
	$(CC) $(flag) $< -o $@

client : client.o
	$(CC) $(flag) $< -o $@

serveur : serveur.o fenetreGlissante.o
	$(CC) $(flag) $^ -o $@

%.o: %.c
	$(CC) $(flag) -c $< -o $@

clean:
	rm -rf *.o
