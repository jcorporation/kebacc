# SPDX-License-Identifier: GPL-3.0-or-later
# (c) 2025 Juergen Mang <mail@jcgames.de>
# https://github.com/jcorporation/kebacc

add_executable(kebacc "")

if(CMAKE_BUILD_TYPE MATCHES "(Release|Debug)")
  target_compile_options(kebacc
    PRIVATE
      "-Wconversion"
      "-Wformat=2"
  )
endif()

target_include_directories(kebacc
  PRIVATE 
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}
)

target_sources(kebacc
  PRIVATE
    lib/config.c
    lib/filehandler.c
    lib/log.c
    lib/mg_user_data.c
    lib/options.c
    data_handler.c
    main.c
    rest.c
    server.c
    wallbox.c
)
