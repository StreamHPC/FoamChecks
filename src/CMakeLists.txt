add_library(foamchecks SHARED MinMaxNamespace.cpp)
target_compile_definitions(foamchecks PRIVATE ${LLVM_DEFINITIONS})
target_include_directories(foamchecks PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(foamchecks PRIVATE
  common_options
  clangASTMatchers
  clangTidy
  clangTidyUtils
  clangTransformer)

# Copy clang resource headers into the build tree
if (NOT TARGET clang-resource-headers)
  # Use only major.minor.patch for the resource directory structure; some
  # platforms include suffix in LLVM_VERSION.
  set(llvm_ver
    "${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.${LLVM_VERSION_PATCH}"
  )
  set(clang_headers_src "${LLVM_LIBRARY_DIR}/clang/${llvm_ver}/include")
  set(clang_headers_dst "${CMAKE_BINARY_DIR}/lib/clang/${llvm_ver}/include")

  file(GLOB_RECURSE in_files RELATIVE "${clang_headers_src}"
    "${clang_headers_src}/*"
  )

  set(out_files)
  foreach (file ${in_files})
    set(src "${clang_headers_src}/${file}")
    set(dst "${clang_headers_dst}/${file}")

    add_custom_command(OUTPUT "${dst}"
      DEPENDS "${src}"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${src}" "${dst}"
      COMMENT "Copying clang's ${file}..."
    )
    list(APPEND out_files "${dst}")
  endforeach()

  add_custom_target(clang-resource-headers ALL DEPENDS ${out_files})
endif()
