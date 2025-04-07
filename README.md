# Parallel-Matrix-mult

## 1. Git Clone this repository using 
`git clone git@github.com:Diatbbin/Parallel-Matrix-mult.git`

## 2. Create a Build Directory 
`mkdir build`

## 3. Navigate into the Build Directory
`cd build`

## 4. Configure the Project
`cmake -DCMAKE_PREFIX_PATH={Path where HPX is installed} ..`

## 5. Build the Project
`make`

## 6. Run the Executable 
`./matrix_mul --n=100 --m=100 --k=100 --l=0 --u=100 --seed=123`

## Parameter Explanation
n refers to the number of rows of the first matrix<br>
m refers to the number of cols of the first matrix and the number of rows for the second matrix<br>
k refers to the number of cols of the second matrix<br>
l refers to the lower limit of range of value<br>
u refers to the upper limit of range of value