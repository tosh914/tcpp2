/**
 * @file eigen_stl_convert.hpp
 * @brief Convert STL containers <-> Eigen::Array/Matrix
 * @author Toshimitsu Takahashi
 * @date 2012/12/22
 *
 */

#ifndef TCPP2_EIGEN_STL_CONVERT_HPP_
#define TCPP2_EIGEN_STL_CONVERT_HPP_

#include "earray2vector.hpp"
#include "emat2vector.hpp"

#include <cassert>

#ifdef USING_TBB
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#endif

/**
 * @namespace tcpp
 */
namespace tcpp {

/**
 * @brief Convert Eigen::Array\<T, size, 1\> to std::vector\<T\>
 * @note When size == -1(= Eigen::Dynamic), eigen_array will be treated as dynamic array
 * @param[in] eigen_array Eigen::Array\<T, size, 1\> (1-dimensional array)
 * @param[out] std_vector std::vector\<T\> Asserted to be empty.
 */
template <typename T, int size>
void Convert( const Eigen::Array<T, size, 1> &eigen_array, std::vector<T> &std_vector ) {
	EArray2Vector<T, size> earray2vector( eigen_array, std_vector );
#ifdef USING_TBB
	tbb::task_scheduler_init init;
	tbb::blocked_range<int> range(0, eigen_array.rows(), 1000);
	tbb::parallel_for( range, earray2vector );
#else
	earray2vector();
#endif
}

/**
 * @brief Convert std::vector\<T\> to Eigen::Array\<T, size, 1\>
 * @note When size == -1(= Eigen::Dynamic), eigen_array will be treated as dynamic array
 * @param[in] std_vector std::vector\<T\> Asserted to have {size} values except when size == -1
 * @param[out] eigen_array Eigen::Array\<T, size, 1\> (1-dimensional array)
 */
template <typename T, int size>
void Convert( const std::vector<T>& std_vector, Eigen::Array<T, size, 1>& eigen_array ) {
	int element_num;
	if( size > 0 ) {
		element_num = size;
	} else {
		element_num = static_cast<int>( std_vector.size() );
	}
	assert( element_num == static_cast<int>( std_vector.size() ) );
	
	eigen_array = Eigen::Array<T, size, 1>::Zero( element_num, 1 );
	for( int i = 0; i < element_num; ++i )
		eigen_array.coeffRef(i) = std_vector[i];
}

/**
 * @breif Convert Eigen::Array\<T, row_num, col_num\> to std::vector\<std::vector\<T\> \>
 * @param[in] eigen_array Eigen::Array\<T, row_num, col_num\> (2-dimensional array)
 * @param[out] std_vector std::vector\<std::vector\<T\> \>, asserted to be empty
 */
template <typename T, int row_num, int col_num>
void Convert( const Eigen::Array<T, row_num, col_num> &eigen_array, std::vector<std::vector<T> > &std_vector ) {
	assert( std_vector.empty() );
	int rows, cols;
	if( row_num > 0 ) {
		rows = row_num;
	} else {
		rows = eigen_array.rows();
	}
	
	if( col_num > 0 ) {
		cols = col_num;
	} else {
		cols = eigen_array.cols();
	}

	std_vector.reserve(rows);
	for( int row = 0; row < rows; ++row ) {
		std::vector<T> row_vector;
		row_vector.reserve(cols);
		for( int col = 0; col < cols; ++col ) {
			row_vector.push_back( eigen_array( row, col ) );
		}
		std_vector.push_back( row_vector );
	}
}

/**
 * @breif Convert Eigen::Array\<T, row_num, col_num\> to std::vector\<std::vector\<T\> \>
 * @param[in] std_vector std::vector\<std::vector\<T\> \>, std_vector.size() == row_num and std_vector[0].size() == col_num
 * @param[out] eigen_array Eigen::Array\<T, row_num, col_num\> (2-dimensional array)
 */
template <typename T, int row_num, int col_num>
void Convert( const std::vector<std::vector<T> > &std_vector, Eigen::Array<T, row_num, col_num> &eigen_array ) {
	int size_of_std_vector = static_cast<int>( std_vector.size() );
	int size_of_std_vector_0 = static_cast<int>( std_vector[0].size() );

	assert( row_num < 0 || row_num == size_of_std_vector );
	assert( col_num < 0 || col_num == size_of_std_vector_0 );

	int rows = size_of_std_vector;
	int cols = size_of_std_vector_0;

	eigen_array = Eigen::Array<T, row_num, col_num>::Zero( rows, cols );
	for( int row = 0; row < rows; ++row )
		for( int col = 0; col < cols; ++col )
			eigen_array.coeffRef( row, col ) = std_vector[row][col];
}

/**
 * @breif Convert Eigen::Matrix\<T, row_num, col_num\> to std::vector\<std::vector\<T\> \>
 * @param[in] eigen_matrix Eigen::Matrix\<T, row_num, col_num\>
 * @param[out] std_vector std::vector\<std::vector\<T\> \>, asserted to be empty
 */
template <typename T, int row_num, int col_num>
void Convert( const Eigen::Matrix<T, row_num, col_num> &eigen_matrix, std::vector<std::vector<T> > &std_vector ) {
	EMat2Vector<T, row_num, col_num> emat2vector( eigen_matrix, std_vector );
#ifdef USING_TBB
	tbb::task_scheduler_init init;
	tbb::blocked_range2d<int> range(0, eigen_matrix.rows(), 100, 0, eigen_matrix.cols(), 100);
	tbb::parallel_for( range, emat2vector );
#else
	emat2vector();
#endif
}

/**
 * @breif Convert std::vector\<std::vector\<T\> \> to Eigen::Array\<T, row_num, col_num\>
 * @param[in] eigen_matrix Eigen::Matrix\<T, row_num, col_num\>
 * @param[out] std_vector std::vector\<std::vector\<T\> \>, std_vector.size() == row_num and std_vector[0].size() == col_num
 */
template <typename T, const int row_num, const int col_num>
void Convert( const std::vector<std::vector<T> > &std_vector, Eigen::Matrix<T, row_num, col_num> &eigen_matrix ) {
	int size_of_std_vector = static_cast<int>( std_vector.size() );
	int size_of_std_vector_0 = static_cast<int>( std_vector[0].size() );
	
	assert( row_num < 0 || row_num == size_of_std_vector );
	assert( col_num < 0 || col_num == size_of_std_vector_0 );
	
	int rows = size_of_std_vector;
	int cols = size_of_std_vector_0;

	eigen_matrix = Eigen::Matrix<T, row_num, col_num>::Zero( rows, cols );
	for( int row = 0; row < rows; ++row)
		for( int col = 0; col < cols ; ++col )
			eigen_matrix.coeffRef( row, col ) = std_vector[row][col];
}

} /* namespace tcpp */

#endif /* TCPP_EIGEN_STL_CONVERT_HPP_ */

