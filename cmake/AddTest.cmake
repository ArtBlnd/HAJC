macro(AddTest testname)
    project(testname)
    add_executable(testname ${ARGV})
endmacro(AddTest)