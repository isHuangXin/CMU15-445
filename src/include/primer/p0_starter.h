//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// p0_starter.h
//
// Identification: src/include/primer/p0_starter.h
//
// Copyright (c) 2015-2020, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

// PROJECT #0 - C++ PRIMER
// 您只需要修改一个文件：p0_starter.h
// 在这个头文件中，我们定义了您必须实现的三个类：Matrix，RowMatrix，RowMatrixOperations。
// 这些矩阵是简单的二维矩阵，必须支持加法、矩阵乘法和简化的通用矩阵乘法(GEMM)运算。
// Matrix抽象类定义了派生类的常用功能RowMatrix。RowMatrixOperations类使用RowMatrix对象来实现上述概览中所提到的操作。
// 函数原型和成员变量在文件中指定。项目要求您填写所有构造函数、析构函数和成员函数的实现。
// 不要加任何附加功能的原型或者成员变量。您的实施应仅包括实施我们为您定义的功能。

#pragma once

#include <memory>

namespace bustub {

/*
 * The base class defining a Matrix
 */
template <typename T>
class Matrix {
 protected:
  // TODO(P0): Add implementation
  Matrix(int r, int c) {
    rows = r;
    cols = r;
    linear = new T[r * c];
    // memset(linear, 0, sizeof(T) * r * c);
  }

  // # of rows in the matrix
  int rows;
  // # of Columns in the matrix
  int cols;
  // Flattened array containing the elements of the matrix
  // TODO(P0) : Allocate the array in the constructor. Don't forget to free up
  // the array in the destructor.
  T *linear;

 public:
  // Return the # of rows in the matrix
  virtual int GetRows() = 0;

  // Return the # of columns in the matrix
  virtual int GetColumns() = 0;

  // Return the (i,j)th  matrix element
  virtual T GetElem(int i, int j) = 0;

  // Sets the (i,j)th  matrix element to val
  virtual void SetElem(int i, int j, T val) = 0;

  // Sets the matrix elements based on the array arr
  virtual void MatImport(T *arr) = 0;

  // TODO(P0): Add implementation
  virtual ~Matrix() {  // 析构函数释放动态数组内存
    delete[] linear;   // 释放linear指向的内存，并且逐一调用数组中每个对象的destructor
    // 用 new[] 分配的内存需要用 delete[] 释放，它们是一一对应的。
  }
};

template <typename T>
class RowMatrix : public Matrix<T> {
 public:
  // TODO(P0): Add implementation
  RowMatrix(int r, int c) : Matrix<T>(r, c) {
    data_ = new T *[r](); // ()表示初始化为空指针
    for(int i = 0; i < r; i++) {
      data_[i] = &this->linear[i * c];
    }
  }

  // TODO(P0): Add implementation
  int GetRows() override { return this->rows; }

  // TODO(P0): Add implementation
  int GetColumns() override { return this->cols; }

  // TODO(P0): Add implementation
  T GetElem(int i, int j) override { 
    // if (i < 0 || j < 0 || i >= this->rows_ || j >= this->cols_) {
      // throw Exception(ExceptionType::OUT_OF_RANGE, "GetElement(i,j) out of range");
    // }
    return data_[i][j]; 
  } 

  // TODO(P0): Add implementation
  void SetElem(int i, int j, T val) override {
    // if (i < 0 || j < 0 || i >= this->rows_ || j >= this->cols_) {
      // throw Exception(ExceptionType::OUT_OF_RANGE, "SetElement(i,j) out of range");
    // }
    data_[i][j] = val;
  }

  // TODO(P0): Add implementation
  void MatImport(T *arr) override {
    for(int i = 0; i < GetRows(); i++) {
      data_[i] = arr + i * GetColumns();
    }
  }

  // TODO(P0): Add implementation
  ~RowMatrix() override {
    delete[] data_;
    // data_ = nullptr;
  };  // 是否还要释放this->lieaner呢？(我觉得不用，他们不是共用一个内存空间吧)

 private:
  // 2D array containing the elements of the matrix in row-major format
  // TODO(P0): Allocate the array of row pointers in the constructor. Use these pointers
  // to point to corresponding elements of the 'linear' array.
  // Don't forget to free up the array in the destructor.
  T **data_;
};

template <typename T>
class RowMatrixOperations {
 public:
  // Compute (mat1 + mat2) and return the result.
  // Return nullptr if dimensions mismatch for input matrices.
  static std::unique_ptr<RowMatrix<T>> AddMatrices(std::unique_ptr<RowMatrix<T>> mat1,
                                                   std::unique_ptr<RowMatrix<T>> mat2) {
    // TODO(P0): Add code
    // 注意，得到unique_ptr中的元素只能用.get()，不能用->get()；或者不用.get()
    int row1 = mat1->GetRows();
    int col1 = mat1->GetColumns();
    int row2 = mat2->GetRows();
    int col2 = mat2->GetColumns();
    if (row1 != row2 || col1 != col2) {  
      // 矩阵大小不相等
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }

    // row1==row2 && col1==col2
    std::unique_ptr<RowMatrix<T>> res_mat(new RowMatrix<T>(row1, col1)); // 创建一个指向RowMatrix<T>的unique_ptr
    for(int i = 0; i < row1; i++) {
      for(int j = 0; j < col1; j++) {
        T val = mat1->GetElem(i, j) + mat2->GetElem(i, j);
        res_mat->SetElem(i, j, val);
      }
    }
    return res_mat;
  }

  // Compute matrix multiplication (mat1 * mat2) and return the result.
  // Return nullptr if dimensions mismatch for input matrices.
  static std::unique_ptr<RowMatrix<T>> MultiplyMatrices(std::unique_ptr<RowMatrix<T>> mat1,
                                                        std::unique_ptr<RowMatrix<T>> mat2) {
    // TODO(P0): Add code
    int row1 = mat1->GetRows();
    int col1 = mat1->GetColumns();
    int row2 = mat2->GetRows();
    int col2 = mat2->GetColumns();
    if (col1 != row2) {  // 矩阵大小不满足mat1*mat2的乘法要求
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }

    // col1 == row2   (row1*col1)*(row2*col2)
    std::unique_ptr<RowMatrix<T>> res_mat(new RowMatrix<T>(row1, col2));
    for (int i = 0; i < row1; i++) {
      for (int j = 0; j < col2; j++) {
        T val = 0;
        for (int k = 0; k < col1; k++) {
          val += mat1->GetElem(i, k) * mat2->GetElem(k, j);
        }
        res_mat->SetElem(i, j, val);
      }
    }
    return res_mat;
  }

  // Simplified GEMM (general matrix multiply) operation
  // Compute (matA * matB + matC). Return nullptr if dimensions mismatch for input matrices
  static std::unique_ptr<RowMatrix<T>> GemmMatrices(std::unique_ptr<RowMatrix<T>> matA,
                                                    std::unique_ptr<RowMatrix<T>> matB,
                                                    std::unique_ptr<RowMatrix<T>> matC) {
    // TODO(P0): Add code
    std::unique_ptr<RowMatrix<T>> tmp_mat = MultiplyMatrices(matA, matB);
    if (tmp_mat == nullptr) {
      return tmp_mat;
    }
    return AddMatrices(tmp_mat, matC);
  }
};
}  // namespace bustub
