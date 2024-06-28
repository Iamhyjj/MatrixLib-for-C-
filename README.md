# <center> Matrix Lib for C++(Unfinished yet)
## Intruduction
This is a simple Matrix Lib for C++ which is based on my former Matrix Lib for C(https://github.com/Iamhyjj/Matrix-Lib-for-C)( but better :) ).  
It supports basic matrix caculations and operations:  
(1)+ (+=) , - (-=) , * (*=) , = , <<  
(2)transpose  
(3)determinant calculation   
(4)Gaussion elinimation   
## Basic features of Class Matrix
(1)Template Class  
The element type of matrix is up to you, you can declare it when creating  
e.g. MatrixLib::Matrix<double> m();  
(2)Using unique_ptr to store data    
(3)Error check  
Invalid operations will be thrown,for example , in operator+:  
    *if(rows_ != other.rows() || cols_ != other.cols()){*  
        *throw std::invalid_argument("Matrixes dimensions mismatch");*  
    *}*  
