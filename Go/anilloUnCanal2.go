package main

import (
	"fmt"
	"strconv"
	"time"
)

type Node struct { //creamos estructura Node en la que guardamos un ID de tipo entero y una referencia al siguiente (para topología anillo)
	ID   int
	next *Node
}

type Anillo struct {  //Estructura de anillo en la que se guarda el primer nodo y el número total de nodos.
	Nodo     Node
	numNodos int
}

func (a *Anillo) InitAnillo() *Anillo { //Inicializamos el primer nodo con ID 0 y el siguiente es él mismo (ya que solo hay un nodo)
	a.Nodo.ID = 0
	a.Nodo.next = &a.Nodo
	a.numNodos = 1
	return a
}

func crearAnillo() *Anillo {
	return new(Anillo).InitAnillo()
}

func (a *Anillo) AddNode(n *Node) *Anillo { //insertar nodo
	aux := a.Nodo.next
	for aux.next != &a.Nodo { // recorremos anillo hasta llegar al "último" nodo para insertar al final (no se han hecho comprobaciones de insertar dos nodos iguales ni insertar nodos menores...
		aux = aux.next    // en principio inserta los nodos por orden de llamada a la AddNode.
	}
	n.next = &a.Nodo //enlazamos último nodo (nodo a insertar) con el primer nodo
	aux.next = n //enlazamos nodo anterior con el nuevo
	a.numNodos++ //incrementamos número de nodos
	return a
}

func (a *Anillo) imprimirAnillo() { //imprime el ID de todos los nodos del anillo
	aux := &a.Nodo
	i := 0
	for i < a.numNodos {
		print(aux.ID)
		print(" ")
		aux = aux.next
		i++
	}

}

func playToken(nodo Node, canal chan string, token *string) {            //pasamos nodo, canal y referencia a token (ya que va a ser modificado)
	//print("Nodo " + strconv.Itoa(nodo.ID) + " tiene el token\n")
	if nodo.ID == 0 {      //solo nodo 0
		*token += "- " //añadimos -
		time.Sleep(100 * time.Millisecond) // duerme 100ms
	} else {
		*token += strconv.Itoa(nodo.ID) + " " //añadimos ID del nodo correspondiente al token
		time.Sleep(500 * time.Millisecond) //duerme 500ms
	}
	//print(<-canal)
	canal <- *token //metemos el token en el canal
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
	token := ""
	for i := 0; i < n; i++ {
		go playToken(*aux, c, &token) //llamada rutina de modificación de canal y token
		print("Token recibido: " + <-c + "\n") // imprimimos lo que hay dentro del canal en ese momento (el canal se bloquea hasta que no hay una rutina que lo recibe)
		aux = aux.next
	}
	print("Token final: " + token) //imprimimos token final modificado.

}
