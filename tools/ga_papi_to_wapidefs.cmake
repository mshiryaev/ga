if (INPUT)
  if (OUTPUT)
    message(STATUS "Reading ${INPUT} ...")
    file(READ "${INPUT}" in0)
    string(REGEX REPLACE "\r?\n" ";" lines "${in0}")
    set(out "")
    foreach (l ${lines})
      string(REGEX MATCH "p(nga_.*) *\\(" found "${l}")
      if (found) 
        set(l "#define w${CMAKE_MATCH_1} p${CMAKE_MATCH_1}")
      endif (found)
      string(REGEX MATCH "PAPI" found "${l}")
      if (found)
        string(REPLACE "PAPI" "WAPIDEFS" l "${l}")
      endif (found)
      # remove comments
      string(REGEX REPLACE "/\\*..*\\*/" "" l "${l}")
      # ignore lines with leading whitespace
      string(REGEX MATCH "^  *" found "${l}")
      if (found) 
        set(l "")
      endif ()
      # ignore typedef lines
      string(REGEX MATCH "typedef" found "${l}")
      if (found) 
        set(l "")
      endif ()
      if (l)
        set(out "${out}${l}\n")
      endif (l)
    endforeach ()
    message(STATUS "Writing ${OUTPUT} ...")
    file(WRITE "${OUTPUT}" "${out}")
  else (OUTPUT)
    message(ERROR "OUTPUT variable must be set")
  endif (OUTPUT)
else (INPUT)
  message(ERROR "INPUT variable must be set")
endif (INPUT)
if (INPUT)
  if (OUTPUT)
    message(STATUS "Reading ${INPUT} ...")
    file(READ "${INPUT}" in0)
    string(REGEX REPLACE "\r?\n" ";" lines "${in0}")
    set(out "")
    foreach (l ${lines})
      string(REGEX MATCH "p(nga_.*) *\\(" found "${l}")
      if (found) 
        set(l "#define w${CMAKE_MATCH_1} p${CMAKE_MATCH_1}")
      endif (found)
      string(REGEX MATCH "PAPI" found "${l}")
      if (found)
        string(REPLACE "PAPI" "WAPIDEFS" l "${l}")
      endif (found)
      # remove comments
      string(REGEX REPLACE "/\\*..*\\*/" "" l "${l}")
      # ignore lines with leading whitespace
      string(REGEX MATCH "^  *" found "${l}")
      if (found) 
        set(l "")
      endif ()
      # ignore typedef lines
      string(REGEX MATCH "typedef" found "${l}")
      if (found) 
        set(l "")
      endif ()
      if (l)
        set(out "${out}${l}\n")
      endif (l)
    endforeach ()
    message(STATUS "Writing ${OUTPUT} ...")
    file(WRITE "${OUTPUT}" "${out}")
  else (OUTPUT)
    message(ERROR "OUTPUT variable must be set")
  endif (OUTPUT)
else (INPUT)
  message(ERROR "INPUT variable must be set")
endif (INPUT)