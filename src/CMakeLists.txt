Set(Headers
        parser.h
        )
Set(Source
        parser.c
        )

# Add the source files for the math_tools component
add_library(src STATIC ${Source} ${Headers})