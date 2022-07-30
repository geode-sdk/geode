// Copyright (c) 2020-2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_FILESYSTEM_HPP
#define TAO_PEGTL_INTERNAL_FILESYSTEM_HPP

#include "../config.hpp"

#include <fs/filesystem.hpp>

namespace TAO_PEGTL_NAMESPACE::internal
{
   namespace filesystem = ghc::filesystem;

   using error_code = std::error_code;

   inline const auto& system_category() noexcept
   {
      return std::system_category();
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
