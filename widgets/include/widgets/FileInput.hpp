#ifndef FILE_INPUT_HPP
#define FILE_INPUT_HPP

#include <touchgfx/hal/types.hpp>

namespace touchgfx
{
namespace fileinput
{
//type used for file handles
typedef void* FileHdl;

//Function for reading files
int readFile(FileHdl hdl, uint8_t* const buffer, uint32_t length);
}
}

#endif
