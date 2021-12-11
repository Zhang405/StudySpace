/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-09 20:49:36
 */
package main

import(
	"fmt"
	"os"
)

func main(){
	s,sep := " "," "
	for _,arg := range os.Args[1:]{
		s+=sep+arg
		//sep = " "
	}
	fmt.Println(s)
}