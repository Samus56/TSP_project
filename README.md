# TSP_project v01

'TSP_project' è un'applicazione in C che, dato una matrice di pesi di un grafo simmetrico risolve il 
problema del commesso viaggiatore (TSP). Il nome nasce dalla sua più tipica rappresentazione: dato un insieme di città, e note le distanze tra ciascuna coppia di esse, trovare il tragitto di minima percorrenza che un commesso viaggiatore deve seguire per visitare tutte le città una ed una sola volta e ritornare alla città di partenza.
Nell'applicazione sono stati usati 2 algoritmi, il Nearest Neighbour e il Simulated Annealing.

## Installazione
Da dove è stato scaricato il progetto:
```
cd progetto
make install
```

## Uso
```
./main
```
## Esempio
```
$ cd progetto
$ make install
$ ./main

Solution Nearest Neighbor: 
0 14 12 10 5 8 4 11 3 9 13 6 2 7 1 0 
Il costo della soluzione e' 2964 
Solution Simulated Annealing: 
0 4 8 5 10 12 11 3 6 2 13 9 7 1 14 0 
Il costo della soluzione e' 2898
```