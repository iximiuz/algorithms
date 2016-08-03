package main

import (
	"container/heap"
	"fmt"
	"math/rand"
	"reflect"
	"sort"
)

type BinHeap interface {
	sort.Interface // Less(i, j int) bool, Swap(i, j int), Len() int
	Push(x interface{})
	Pop() interface{}
}

func HeapPush(h BinHeap, x interface{}) {
}

func HeapPop(h BinHeap) interface{} {
	return nil
}

func Heapify(h BinHeap) {
	for i := h.Len() / 2; i > 0; i-- {
		bubbleDown(h, i)
	}
}

func HeapReplace(h BinHeap, x interface{}) interface{} {
	return nil
}

func bubbleUp(h BinHeap, idx int) {
	for idx > 1 && h.Less(idx-1, idx/2-1) {
		h.Swap(idx-1, idx/2-1)
		idx /= 2
	}
}

func bubbleDown(h BinHeap, idx int) {
	smallest := idx
	left := 2 * idx
	right := left + 1

	if left <= h.Len() && h.Less(left-1, smallest-1) {
		smallest = left
	}
	if right <= h.Len() && h.Less(right-1, smallest-1) {
		smallest = right
	}

	if idx != smallest {
		h.Swap(idx-1, smallest-1)
		bubbleDown(h, smallest)
	}
}

type IntArray []int

func (a IntArray) Less(i, j int) bool {
	return a[i] < a[j]
}

func (a IntArray) Swap(i, j int) {
	a[i], a[j] = a[j], a[i]
}

func (a IntArray) Len() int {
	return len(a)
}

func (a *IntArray) Push(x interface{}) {
	*a = append(*a, x.(int))
}

func (a *IntArray) Pop() interface{} {
	last, tmp := (*a)[len(*a)-1], (*a)[:len(*a)-1]
	*a = tmp
	return last
}

func TestIntHeap(n int) {
	fmt.Printf("Testing integers heap...\n")

	aHeap := make(IntArray, 0)
	for i := 0; i < n; i++ {
		aHeap.Push(rand.Intn(10 * n))
	}

	eHeap := make(IntArray, n)
	copy(eHeap, aHeap)

	Heapify(&aHeap)
	heap.Init(&eHeap)

	if !reflect.DeepEqual(aHeap, eHeap) {
		panic(fmt.Sprintf("Heapify faield. %v != %v", aHeap, eHeap))
	}

	for aHeap.Len() != 0 && eHeap.Len() != 0 {
		if rand.Intn(5) < 4 {
			a := HeapPop(&aHeap)
			b := heap.Pop(&eHeap)
			if a != b {
				panic(fmt.Sprintf("HeapPop failed. %v != %v, %v != %v", a, b, aHeap, eHeap))
			}
		} else {
		}
	}

	fmt.Printf("Success!")
}

func TestUserHeap() {
}

func main() {
	TestIntHeap(20)
	TestUserHeap()
}
