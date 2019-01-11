/**
 *error code header
 */

#ifndef _ERROR_H
#define _ERROR_H


//reading a file
#define E_FAILED_TO_READ -1

//illegal vlArray access
#define E_INDEX_OUT_OF_BOUNDS -2

//invalid bitmap
#define E_INVALID_BMP -3

//not enough memory
#define E_NO_MEMORY -4

//error creating file for write
#define E_FAILED_TO_WRITE -5

//list null
#define E_LIST_NULL -6

//iteration in invalid state
#define E_INVALID_STATE -7

#endif // !_ERROR_H