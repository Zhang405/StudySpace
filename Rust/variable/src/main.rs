/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-04 17:46:11
 */
fn main() {
    let f:bool = false;
    let _guess:u32 = "42".parse().expect("Not a Number");   
    let c = 'z';
    let z = 'ℤ';
    let heart_eyed_cat:char = '😻';//rust的char为四字节，代表一个unicode标量值

    let tup:(i32,f64,u8) = (500,6.4,1);
    let a:[i32,5] = {1,2,3,4,5};
    println!("{}",heart_eyed_cat);
    println!("{}",tup.0);
}
