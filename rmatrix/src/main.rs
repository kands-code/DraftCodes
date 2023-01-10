use rmatrix::rmatrix::matrix::Matrix;
use rmatrix::rmatrix::numtype::NumI32;

fn main() {
    let m: Matrix<NumI32> =
        Matrix::init_from_vector(vec![1, 2, 3, 4, 5, 6, 7, 8, 9], 2, 3).unwrap();
    println!("{}", m);
}
