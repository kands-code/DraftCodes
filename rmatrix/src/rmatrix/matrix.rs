use crate::rmatrix::numtype::Num;

#[derive(Debug)]
pub struct Matrix<T>
where
    T: Num,
{
    data: Vec<T::NumType>,
    size: (usize, usize),
}

impl<T> Matrix<T>
where
    T: Num,
    <T as Num>::NumType: Copy,
{
    pub fn new(row: usize, col: usize) -> Option<Self> {
        if row == 0 || col == 0 {
            None
        } else {
            let default_val = T::zero();
            Some(Matrix {
                data: vec![default_val; row * col],
                size: (row, col),
            })
        }
    }

    pub fn init_from_vector(v: Vec<T::NumType>, row: usize, col: usize) -> Option<Matrix<T>> {
        if row * col > v.len() {
            None
        } else {
            Some(Matrix {
                data: v.split_at(row * col).0.to_owned(),
                size: (row, col),
            })
        }
    }

    pub fn size(&self) -> (usize, usize) {
        self.size
    }

    pub fn get<'a>(&'a self, row: usize, col: usize) -> Option<&'a T::NumType> {
        self.data.get((row - 1) * self.size.0 + (col - 1))
    }

    pub fn set<'a>(&'a mut self, row: usize, col: usize, elem: &'a T::NumType) -> bool {
        if row <= self.size.0 && row >= 1 && col <= self.size.1 && col >= 1 {
            eprintln!("Error: Out of Boundary! ({}, {})", row, col);
            false
        } else {
            self.data[(row - 1) * self.size.0 + (col - 1)] = elem.to_owned();
            true
        }
    }
}

impl<T> Clone for Matrix<T>
where
    T: Num,
    <T as Num>::NumType: Copy,
{
    fn clone(&self) -> Self {
        Self {
            data: self.data.clone(),
            size: self.size.clone(),
        }
    }
}

impl<T> std::ops::Add for Matrix<T>
where
    T: Num,
    <T as Num>::NumType: Copy + std::ops::Add<Output = T::NumType>,
{
    type Output = Matrix<T>;

    fn add(self, rhs: Self) -> Self::Output {
        Matrix {
            data: self
                .data
                .iter()
                .zip(&rhs.data)
                .map(|(&a, &b)| a + b)
                .collect(),
            size: self.size,
        }
    }
}

impl<T> std::fmt::Display for Matrix<T>
where
    T: Num,
    <T as Num>::NumType: Copy + std::fmt::Display,
{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "mat>>\n")?;
        for i in 1..=self.size.0 {
            write!(f, " ")?;
            for j in 1..=self.size.1 {
                write!(f, "{:<3}", self.get(i, j).unwrap())?;
            }
            write!(f, "\n")?;
        }
        write!(f, "<<mat")
    }
}
