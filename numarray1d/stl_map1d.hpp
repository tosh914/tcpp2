/**
 * @file stl_map1d.hpp
 * @brief
 * @author Toshimitsu Takahashi
 * @date 2012/12/27
 *
 */

#ifndef TCPP_STL_MAP1D_HPP_
#define TCPP_STL_MAP1D_HPP_

#include "numarray1d_interface.hpp"
#include <cassert>
#include <map>

/**
 * @namespace tcpp
 */
namespace tcpp {

/**
 * @brief Adapter class of std::vector<T> for Numeric1dInterface<T>
 */
template <typename T>
class StlMap1d: public NumArray1dInterface<T> {
public:
	/********** Constructors **********/
	/**
	 * @brief Constructor
	 * @param[in] body Main body(std::vector<T>)
	 */
	StlMap1d( std::map<int, T>& body ): body_(body) {}

	/********** Accessors **********/
	/**
	 * @brief Access to the main body
	 */
	const std::map<int, T>& body() const { return body_; }

	/**
	 * @brief Access to the main body
	 */
	std::map<int, T>& bodyRef() const { return body_; }

	/**
	 * @brief Access to the coefficient
	 * @param[in] index Index of the coefficient
	 * @return const reference of the coefficient
	 */
	const T& coeff( int index ) const {
		assert( index >= 0 && index < length() );
		return body_[index];
	}

	/**
	 * @brief Access to the coefficient
	 * @param[in] index Index of the coefficient
	 * @return reference (non-const) of the coefficient
	 */
	T& coeffRef( int index ) {
		assert( index >= 0 && index < length() );
		return body_[index];
	}

	/********** Mutators **********/
	/**
	 * @brief Set the coefficient to the specified value
	 * @param[in] index Index of the coefficient
	 * @param[in] value Value the coefficient is set to
	 */
	void set_coeff( int index, T value ) {
		assert( index >= 0 && index < length() );
		body_[index] = value;
	}

	/**
	 * @brief Set the coefficient to the specified value
	 * @param[in] index Index of the coefficient
	 * @param[in] value Value the coefficient is set to
	 */
	template <typename T1>
	void SetCoeff( int index, T1 value ) {
		assert( index >= 0 && index < length() );
		body_[index] = static_cast<T>( value );
	}

	/********** Methods **********/
	/**
	 * @brief Get the number of coefficients
	 */
	int length() const {
		return static_cast<int>( body_.size() );
	}

private:
	std::map<int, T>& body_;
};

} /* namespace tcpp */

#endif /* TCPP_STL_MAP1D_HPP_ */

