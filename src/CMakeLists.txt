file(GLOB SOURCES
    *.hpp
    *.cpp
)
set(INCLUDECOUNTER_SOURCES
    ${INCLUDECOUNTER_SOURCES} 
    ${SOURCES}
    PARENT_SCOPE
)