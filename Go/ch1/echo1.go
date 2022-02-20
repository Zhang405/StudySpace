/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-09 20:40:55
 */
package main

import(
	"fmt"
	"os"
)

func main() {
	var s,sep string
	for i := 1; i < len(os.Args); i++ {
		s += sep + os.Args[i]
		sep = " "
	}
	fmt.Println(s)
}
