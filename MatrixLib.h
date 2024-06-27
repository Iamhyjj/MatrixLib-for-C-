#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
namespace MatrixLib{

template<typename T>
class Matrix
{
    public:
        Matrix(size_t rows, size_t cols, std::vector<std::vector<T>> data): 
        rows_(rows),cols_(cols),data_(std::make_unique<std::vector<std::vector<T>>>(data)){}

        //create a matrix whose elements are default value
        Matrix(size_t rows=0,size_t cols=0):rows_(rows),cols_(cols){
            std::vector<std::vector<T>> data(rows_,std::vector<T>(cols_));
            data_ = std::make_unique<std::vector<std::vector<T>>>(data);
        }

        
        size_t rows()const {return rows_;}

        size_t cols()const {return cols_;}

        const std::vector<std::vector<T>>& data() const {return *data_;}
        
        Matrix operator+(const Matrix& other) const{
            if(rows_ != other.rows() || cols_ != other.cols()){
                throw std::invalid_argument("Matrices dimensions mismatch");
            }
            Matrix result(rows_, cols_);
            for(size_t i = 0; i < rows_; ++i){
                for(size_t j = 0; j < cols_; ++j){
                    (*result.data_)[i][j] = (*data_)[i][j] + (*other.data_)[i][j];
                }
            }
            return result;
        }

        Matrix operator-(){
            Matrix result(rows_,cols_);
            for(size_t i = 0; i < rows_; ++i){
                for(size_t j = 0; j < cols_; ++j){
                    (*result.data_)[i][j] = -(*data_)[i][j];
                }
            }
            return result;
        }

        Matrix operator-(const Matrix& other){

        }

        Matrix operator*(const Matrix& other){

        }

        Matrix operator=(const Matrix& other){

        }

        bool operator==(const Matrix& other) const{
            if(rows_ != other.rows() || cols_ != other.cols_){
                return 0;
            }
            for(auto i{0}; i < rows_ ; ++i){
			    for(auto j{0} ; j<cols_ ; ++j){
                    if(*data_[i][j] != *(other.data()[i][j])){
                        return 0;
                    }
				}
			}
            return 1;
		}

    private:
        std::unique_ptr<std::vector<std::vector<T>>> data_;
        size_t rows_;
        size_t cols_;

};

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