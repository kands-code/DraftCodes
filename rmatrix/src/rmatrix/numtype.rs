pub trait Num {
    type NumType;
    fn zero() -> Self::NumType;
}

#[derive(Debug, Clone, Copy)]
pub struct NumChar;
impl Num for NumChar {
    type NumType = char;

    fn zero() -> Self::NumType {
        0 as char
    }
}

#[derive(Debug, Clone, Copy)]
pub struct NumI32;
impl Num for NumI32 {
    type NumType = i32;

    fn zero() -> Self::NumType {
        0i32
    }
}

#[derive(Debug, Clone, Copy)]
pub struct NumI64;
impl Num for NumI64 {
    type NumType = i64;

    fn zero() -> Self::NumType {
        0i64
    }
}

#[derive(Debug, Clone, Copy)]
pub struct NumI128;
impl Num for NumI128 {
    type NumType = i128;

    fn zero() -> Self::NumType {
        0i128
    }
}

#[derive(Debug, Clone, Copy)]
pub struct NumF32;
impl Num for NumF32 {
    type NumType = f32;

    fn zero() -> Self::NumType {
        0.0f32
    }
}

#[derive(Debug, Clone, Copy)]
pub struct NumF64;
impl Num for NumF64 {
    type NumType = f64;

    fn zero() -> Self::NumType {
        0.0f64
    }
}
