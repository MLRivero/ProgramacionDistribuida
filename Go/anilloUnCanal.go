package main

import (
	"fmt"
	"strconv"
	"time"
)

type Node struct {
	ID   int
	next *Node
}

type Anillo struct {
	Nodo     Node
	numNodos int
	//Creamos el array de los canales de con la longitud dada
	// por el tamaño del anillo
}

func (a *Anillo) InitAnillo() *Anillo {
	a.Nodo.ID = 0
	a.Nodo.next = &a.Nodo
	a.numNodos = 1
	return a
}

func crearAnillo() *Anillo {
	return new(Anillo).InitAnillo()
}

func (a *Anillo) AddNode(n *Node) *Anillo {
	aux := a.Nodo.next
	for aux.next != &a.Nodo {
		aux = aux.next
	}
	n.next = &a.Nodo
	aux.next = n
	a.numNodos++
	return a
}

func (a *Anillo) imprimirAnillo() {
	aux := &a.Nodo
	i := 0
	for i < a.numNodos {
		print(aux.ID)
		print(" ")
		aux = aux.next
		i++
	}

}

func playToken(nodo Node, canal chan string) {
	//print("Nodo " + strconv.Itoa(nodo.ID) + " tiene el token\n")
	t := ""
	if nodo.ID == 0 {
		t += "- "
		time.Sleep(100 * time.Millisecond)
	} else {
		t += strconv.Itoa(nodo.ID) + " "
		time.Sleep(500 * time.Millisecond)
	}
	//print(<-canal)
	canal <- t
}

func main() {
	//creación del Anillo con el numero de Nodos que se quieran introducir
	tamAnillo := 5
	n := 10 // numero iteraciones token
	anillo := crearAnillo()
	i := 1

	for i < tamAnillo {
		anillo.AddNode(&Node{i, nil})
		i++
	}

	anillo.imprimirAnillo()
	fmt.Println(" ")

	/* Parte del canal */

	aux := &anillo.Nodo
	c := make(chan string, n) // creo canal de char
	for i := 0; i < n; i++ {

		go playToken(*aux, c)
		token := <-c
		print("Token recibido: " + token + "\n")
		aux = aux.next
	}

}
