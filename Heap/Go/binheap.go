package main

import (
	"fmt"
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
}

func HeapReplace(h BinHeap, x interface{}) interface{} {
	return nil
}

func main() {

}
