/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-04 22:50:19
 */
fn main() {
    println!("Hello, world!");
    another_fn();
    echo_value(6);
    println!("{}",five());
    println!("{}",add_one(five()));

}

fn another_fn()
{
    println!("Another Function");
}

fn echo_value(x:i32)
{
    println!("The Value is {}",x);
}

fn five()->i32
{
    5
}

fn add_one(x:i32)->i32{
    x+1
}