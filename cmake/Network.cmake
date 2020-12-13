 # Downloads a file from a URL to a local file, raising any errors.
 function(download_file url file)
 if(EXISTS "${file}")
   message(STATUS "${url} already downloaded!")
 else()
   file(DOWNLOAD "${url}" "${file}.tmp" STATUS _stat SHOW_PROGRESS)
   file(RENAME ${file}.tmp ${file})
 endif(EXISTS "${file}")

 list(GET _stat 0 _retval)
 if(_retval)
   # Don't leave corrupt/empty downloads
   list(GET _stat 0 _errcode)
   list(GET _stat 1 _message)
   message(FATAL_ERROR "Error downloading ${url}: ${_message} (${_errcode})")
 endif(_retval)
endfunction(download_file)

function(download_and_extract url file dir)
 download_file(${url} ${file})
 file(ARCHIVE_EXTRACT 
   INPUT ${file} DESTINATION ${dir}
 )
endfunction()