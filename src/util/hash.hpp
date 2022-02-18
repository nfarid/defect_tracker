
#ifndef HPP_UTIL_HASH
#define HPP_UTIL_HASH

#include "./core.hpp"
#include "./cstring_view.hpp"

#include <string>


namespace Util
{


/**
 * \brief Creates a hash from a password string
 * \param str - the password to be hashed
 * \return the password digest
 */
std::string hash(CStringView str);

/**
 * \brief Checks if password is correct against its respective digest
 * \param digest - the stored password digest
 * \param str - the password being check
 * \return true if the password is correct, false otherwise
 */
bool verifyHash(CStringView digest, CStringView str);


}  // namespace Util

#endif  // ifndef HPP_UTIL_HASH
