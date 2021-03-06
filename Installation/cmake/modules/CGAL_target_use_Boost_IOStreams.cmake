if (CGAL_target_use_Boost_IOStreams_included)
  return()
endif()
set(CGAL_target_use_Boost_IOStreams_included TRUE)

function(CGAL_target_use_Boost_IOStreams target)

  if( WIN32 )
    # to avoid a warning with old cmake 
    set(_Boost_BZIP2_HEADERS             "boost/iostreams/filter/bzip2.hpp")
    set(_Boost_ZLIB_HEADERS              "boost/iostreams/filter/zlib.hpp")
    find_package( Boost OPTIONAL_COMPONENTS bzip2 zlib)
  else()
    find_package(ZLIB QUIET)
  endif()

  if(TARGET Boost::iostreams)
    target_link_libraries(${target} PUBLIC Boost::iostreams)
  else()
    target_link_libraries(${target} PUBLIC ${Boost_IOSTREAMS_LIBRARY})
  endif()
  
  if( WIN32 )
    if (Boost_ZLIB_FOUND AND Boost_BZIP2_FOUND)
      target_link_libraries(${target} PUBLIC ${Boost_ZLIB_LIBRARY} ${Boost_BZIP2_LIBRARY})
    else()
      message(STATUS "NOTICE: This project requires Boost ZLIB and Boost BZIP2, and will not be compiled.")
      return()
    endif()
  else()
    if(ZLIB_FOUND)
      target_link_libraries(${target} PUBLIC ZLIB::ZLIB)
    else()
      message(STATUS "NOTICE: This project requires ZLIB, and will not be compiled.")
      return()
    endif()
  endif()

endfunction()
