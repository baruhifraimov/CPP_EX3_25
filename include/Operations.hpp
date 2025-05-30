// baruh.ifraimov@gmail.com
#pragma once
#include <iostream>

namespace coup{

// Defines if the operation is blocked (1) or unblocked (0)
// Stores each enum value using 1 byte — exactly 8 bits.
enum class Operation : u_int8_t {
	NONE   = 0x00,  // 00000000
    GATHER   = 0x01,  // 00000001
    TAX      = 0x02,  // 00000010
    // BRIBE    = 0x04,  // 00000100 [ARCHIVED, it got handled by the judge]
    ARREST   = 0x08,  // 00001000
    SANCTION = 0x10,  // 00010000
    // COUP     = 0x20,  // 00100000 [ARCHIVED, it got handled by the judge]
	EXTRA_TURN = 0x40, // 01000000
	DISABLE_ARREST = 0x80 // 10000000

};

// inline to declare the operator& same across all files
/**
 * @brief Convert to unsigned int to work with bit operations and reconvert to the enum class

 * 
 * @param a 
 * @param b 
 * @return Operation 
 */
inline Operation operator&(Operation a, Operation b){
	return static_cast<Operation>(static_cast<u_int8_t>(a) & static_cast<u_int8_t>(b));
}

/**
 * @brief bitwise OR operation between the OPERATION enum
 * 
 * @param a 
 * @param b 
 * @return Operation 
 */
inline Operation operator|(Operation a, Operation b){
	return static_cast<Operation>(static_cast<u_int8_t>(a) | static_cast<u_int8_t>(b));
}

/**
 * @brief bitwise |= operation between the OPERATION enum
 * 
 * @param a 
 * @param b 
 * @return Operation& 
 */
inline Operation& operator|=(Operation& a, Operation b){
	a = a | b;
	return a;
}

/**
 * @brief bitwise AND operation between the OPERATION enum
 * 
 * @param a 
 * @param b 
 * @return Operation& 
 */
inline Operation& operator&=(Operation& a, Operation b){
	a = a & b;
	return a;
}

/**
 * @brief bitwise NOT operation on a single OPERATION
 * 
 * @param a 
 * @return Operation 
 */
inline Operation operator~(Operation a){
	return static_cast<Operation>(~static_cast<u_int8_t>(a));
}
}