/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-09 20:53:20
 */
 package main

 import(
	 "fmt"
	 "os"
	 "strings"
 )
 
 func main(){
	 fmt.Println(strings.Join(os.Args[1:]," "))
 }