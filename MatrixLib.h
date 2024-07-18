#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
#include <algorithm>
namespace MatrixLib{

template<typename T>
class Matrix
{
    public:
        Matrix(std::vector<std::vector<T>> data): data_(std::make_unique<std::vector<std::vector<T>>>(data))
        {
            rows_ = data.size();
            cols_ = data[0].size();
        }

        //create a matrix whose elements are default value
        Matrix(size_t rows=0,size_t cols=0):rows_(rows),cols_(cols){
            std::vector<std::vector<T>> data(rows_,std::vector<T>(cols_));
            data_ = std::make_unique<std::vector<std::vector<T>>>(data);
        }

        //cpoy constructor
        Matrix(const Matrix& other){
            std::vector<std::vector<T>> dst(other.data().size(),std::vector<T>(other.data()[0].size()));
            rows_ = other.rows();
            cols_ = other.cols();
            for(auto i{0};i<dst.size();i++){
                std::copy(other.data()[i].begin(),other.data()[i].end(),dst[i].begin());
            }
            data_ = std::make_unique<std::vector<std::vector<T>>>(dst);
        }
        
        //move constructor
        Matrix(Matrix&& other):rows_(other.rows()),cols_(other.cols()),data_(std::move(other.data_)){}

        size_t rows()const {return rows_;}

        size_t cols()const {return cols_;}

        const std::vector<std::vector<T>> data() const {return *data_;}
        
        Matrix operator+(const Matrix& other) const{
            if(rows_ != other.rows() || cols_ != other.cols()){
                throw std::invalid_argument("Matrixes dimensions mismatch");
            }
            Matrix result(rows_, cols_);
            for(size_t i = 0; i < rows_; ++i){
                for(size_t j = 0; j < cols_; ++j){
                    (*result.data_)[i][j] = (*data_)[i][j] + (*other.data_)[i][j];
                }
            }
            return result;
        }

        Matrix operator+=(const Matrix& other){
            if(rows_ != other.rows() || cols_ != other.cols()){
                throw std::invalid_argument("Matrixes dimensions mismatch");
            }
            for(size_t i = 0; i < rows_; ++i){
                for(size_t j = 0; j < cols_; ++j){
                    (*data_)[i][j] += (*other.data_)[i][j];
                }
            }
            return *this;
        }

        Matrix operator-() const{
            Matrix result(*this);
            for(size_t i = 0; i < rows_; ++i){
                for(size_t j = 0; j < cols_; ++j){
                    (*result.data_)[i][j] = -(*data_)[i][j];
                }
            }
            return result;
        }

        Matrix operator-(const Matrix& other) const{
            return *this + (-other);
        }

        Matrix operator-=(const Matrix& other){
            return (*this) += (-other);
        }

        Matrix operator*(const Matrix& other) const{
            if(cols_ != other.rows()){
                throw std::invalid_argument("Matrixes dimensions mismatch");
            }
            Matrix result(rows_,other.cols());
            for(auto i{0}; i<rows_ ; ++i){
			    for(auto j{0}; j<other.cols() ;++j){
				    for(auto k{0}; k<cols_ ; ++k){
					    (*result.data_)[i][j] += this->data()[i][k] * other.data()[k][j];
				}
			    }
		    }
            return result;
        }

        Matrix operator*=(const Matrix& other){
            *this = (*this) * other;
            return *this;
        }

        Matrix operator*=(double k){
            for(auto& v:(*data_)){
                for(auto& element: v){
                    element *= k;
                }
            }
            return *this;
        }

        Matrix operator*(double k) const{
            Matrix result(*this);
            return result *= k;
        }


        Matrix pow(unsigned int n){
		    if(n==1){
			    return *this;
		    }
		    if(n==2){
			    return (*this) * (*this);
		    }
		    return n%2 ? (*this) * ((this->pow((n-1)/2)).pow(2)) : (this->pow(n/2)).pow(2);
	    }

        //assgin operator
        Matrix& operator=(const Matrix& other){
            std::vector<std::vector<T>> dst(other.data().size(),std::vector<T>(other.data()[0].size()));
            rows_ = other.rows();
            cols_ = other.cols();
            for(auto i{0};i<dst.size();i++){
                std::copy(other.data()[i].begin(),other.data()[i].end(),dst[i].begin());
            }
            data_ = std::make_unique<std::vector<std::vector<T>>>(dst);
            return *this;
        }
        
        //move assign operator
        Matrix& operator=(Matrix&& other){
            if(this != &other){
                rows_ = other.rows();
                cols_ = other.cols();
                data_ = std::move(other.data_);
            }
            return *this;
        }


        bool operator==(const Matrix& other) const{
            if(rows_ != other.rows() || cols_ != other.cols_){
                return 0;
            }
            for(auto i{0}; i < rows_ ; ++i){
			    for(auto j{0} ; j<cols_ ; ++j){
                    if((*data_)[i][j] != (other.data())[i][j]){
                        return 0;
                    }
				}
			}
            return 1;
		}

        bool operator!=(const Matrix& other) const{
            return !(*this == other);
        }

        Matrix tranpose(){
            Matrix result(cols_,rows_);
	        for(auto i{0} ; i<rows_ ; ++i){
		        for(auto j{0} ; j<cols_ ; ++j){
			        (*result.data_)[j][i] = (*data_)[i][j];
		        }
	        }
	        return result;
        }

        Matrix gaussionElimination(){
            Matrix result(*this);
            for(auto i{0}; i<rows_&&i<cols_; ++i){
                auto j=i;
                while(result.data()[i][i]==0&&j<rows_){
                    if(result.data()[j][i]){
                        std::swap((*result.data_)[i],(*result.data_)[j]);      //make mat[i][i]!=0
                        for(auto& element : (*result.data_)[i]){
                            element = -element;                               //guarding the determinant of the matrix
                        }
                    }
                    j++;
                }
                if(j==rows_){
                    continue;                //every element in coloumn number i is 0,move to next coloumn
                }
                for(j=0;j<rows_;++j){
                    if(j!=i&&(*result.data_)[j][i]){
                        auto t = -(*result.data_)[j][i]/(*result.data_)[i][i];
                        for(auto k{0};k<cols_;k++){
                            (*result.data_)[j][k] += (*result.data_)[i][k] * t;
                        }
                    }
                }
            }
            return result;
        }

        T determinant(){
            if(rows_ != cols_){
                throw std::invalid_argument("The matrix isn't square.");
            }
            Matrix temp(this->gaussionElimination());
            T result = 1;
            for(auto i{0};i<rows_;++i){
                result *= temp.data()[i][i];
            }
            return result;
        }


        unsigned int rank(){
            Matrix temp(this -> gaussionElimination());
            unsigned int result = 0;
            for(auto i{0}; i<rows_&&i<cols_ ; ++i){
                if((*temp.data_)[i][i]){
                    result++;
                }
	        }
	        return result;
        }

        Matrix inverse(){
            if(rows_ != cols_){
                throw std::invalid_argument("The matrix isn't square.");
            }
            else if(this->rank() < rows_){
                throw std::invalid_argument("The matrix is a singular matrix.");
            }
            else{
                Matrix temp(rows_,2*cols_),result(rows_,cols_);;
                for(auto i{0};i<rows_;++i){
                    for(auto j{0};j<cols_;++j){
                        (*temp.data_)[i][j] = (*data_)[i][j];
                    }
                }
                for(auto i{cols_};i<temp.cols();++i){
                    (*temp.data_)[i-cols_][i] = 1;
                }
                temp = std::move(temp.gaussionElimination());
                //unitization
                for(auto i{0}; i< rows_; ++i){
                    for(auto& element : (*temp.data_)[i]){
                        element /= (*temp.data_)[i][i];
                    }
                }
                for(auto i{0};i<temp.rows();++i){
                    for(auto j{cols_};j<temp.cols();++j){
                        (*result.data_)[i][j-cols_]=temp.data()[i][j];
                    }
                }
                return result;
            }
            
        }

    protected:
        std::unique_ptr<std::vector<std::vector<T>>> data_;
        size_t rows_;
        size_t cols_;


};

template<typename T>
class IdentityMatrix : public Matrix<T>
{
    public:
        IdentityMatrix(size_t rows = 0) : Matrix<T>(rows,rows)
        {
            for(auto i{0} ; i < rows ; ++i){
                (*Matrix<T>::data_)[i][i] = 1;
            }
        }
};

template<typename T>
Matrix<T> operator*(double k, Matrix<T> m){
    return m*k;
}


template<typename T>
std::ostream& operator<<(std::ostream& out,const Matrix<T>& m){
    const auto& matrix = m.data();
    for(const auto& row:matrix){
        for(const auto& element:row){
            out << element << " ";
        }
        out << "\n";
    }
    return out;
}




}//namespace MatrixLib
