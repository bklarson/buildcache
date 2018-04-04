//--------------------------------------------------------------------------------------------------
// Copyright (c) 2018 Marcus Geelnard
//
// This software is provided 'as-is', without any express or implied warranty. In no event will the
// authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose, including commercial
// applications, and to alter it and redistribute it freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not claim that you wrote
//     the original software. If you use this software in a product, an acknowledgment in the
//     product documentation would be appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be misrepresented as
//     being the original software.
//
//  3. This notice may not be removed or altered from any source distribution.
//--------------------------------------------------------------------------------------------------

#include "compiler_wrapper.hpp"

#include "hasher.hpp"

#include <iostream>

namespace bcache {
compiler_wrapper_t::compiler_wrapper_t(cache_t& cache) : m_cache(cache) {
}

compiler_wrapper_t::~compiler_wrapper_t() {
}

bool compiler_wrapper_t::handle_command(const arg_list_t& args, int& return_code) {
  try {
    // Start a hash.
    hasher_t hasher;

    // Hash the preprocessed file contents.
    hasher.update(preprocess_source(args));

    // Hash the (filtered) command line flags.
    hasher.update(filter_arguments(args).join(" "));

    // Hash the compiler version string.
    hasher.update(get_compiler_id(args));

    // DEBUG
    std::cout << " == HASH: " << hasher.final().as_string() << "\n";

    // Look up the entry in the cache or create a new entry.
    // TODO(m): Implement me!
    (void)return_code;
    return false;
  } catch (...) {
    return false;
  }
}

file::tmp_file_t compiler_wrapper_t::get_temp_file(const std::string& extension) const {
  // Use a suitable folder for temporary files.
  const auto tmp_path = file::append_path(m_cache.root_folder(), "tmp");
  if (!file::dir_exists(tmp_path)) {
    file::create_dir(tmp_path);
  }

  // Generate a fairly unique file name.
  return file::tmp_file_t(tmp_path, extension);
}
}  // namespace bcache