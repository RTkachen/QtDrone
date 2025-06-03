cmake_minimum_required(VERSION 3.16)

project(moj_projekt VERSION 0.1 LANGUAGES CXX)
cmake_minimum_required(VERSION 3.16)

project(moj_projekt VERSION 0.1 LANGUAGES CXX)
include_directories(inc)

# Ustawienie folderu wyjść
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts Core Gui SerialPort LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts Core Gui SerialPort LinguistTools)

set(TS_FILES
  linguist/moj_projekt_pl_150.ts
  linguist/moj_projekt_en_150.ts)

set(PROJECT_SOURCES
    src/mainwindow.cpp
    inc/mainwindow.hpp
    ui/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(moj_projekt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/main.cpp
        ui/chartwindow.ui
        src/chartwindow.cpp
        inc/chartwindow.hpp
        src/SpinBoxController.cpp
        inc/SpinBoxController.hpp
        resources.qrc
        src/translator.cpp
        inc/translator.hpp
        src/accsimulator.cpp
        inc/accsimulator.hpp
        src/simulationcontroller.cpp
        inc/simulationcontroller.hpp
        src/SerialPortHandler.cpp
        inc/SerialPortHandler.hpp
        inc/D3widget.hpp
        src/D3widget.cpp
        src/D3widget.cpp
    )

    # Generator aktualizacji plików tłumaczeń (lupdate)
    find_program(LUPDATE_EXECUTABLE lupdate)
    add_custom_target(
      update-translations
      COMMAND ${LUPDATE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/ui
              -ts ${CMAKE_CURRENT_SOURCE_DIR}/linguist/moj_projekt_en_150.ts
                   ${CMAKE_CURRENT_SOURCE_DIR}/linguist/moj_projekt_pl_150.ts
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Generowanie plików .qm z plików .ts
    qt_add_translation(QM_FILES ${TS_FILES})

    # --- Dodajemy custom target wymuszający wygenerowanie plików tłumaczeń ---
    add_custom_target(generated_qm DEPENDS ${QM_FILES})

    # ---- Kopiowanie plików .qm do katalogu resources/languages ----
    # Zmieniamy DEST_QM_DIR na resources/languages wewnątrz katalogu projektu.
    set(DEST_QM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/languages)
    add_custom_target(copy_qm ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_QM_DIR}
        COMMENT "Tworzenie katalogu: ${DEST_QM_DIR}"
    )
    # Upewniamy się, że target copy_qm wykona się dopiero po wygenerowaniu plików tłumaczeń.
    add_dependencies(copy_qm generated_qm)

    foreach(qm_file IN LISTS QM_FILES)
        get_filename_component(qm_filename ${qm_file} NAME)
        add_custom_command(TARGET copy_qm POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${qm_file} ${DEST_QM_DIR}/${qm_filename}
            COMMENT "Kopiowanie ${qm_filename} do ${DEST_QM_DIR}"
            DEPENDS ${qm_file}
        )
    endforeach()
    add_dependencies(moj_projekt copy_qm)
    # ------------------------------------------------------------------------

    include(GNUInstallDirs)
    install(TARGETS moj_projekt
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
else()
    if(ANDROID)
        add_library(moj_projekt SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(moj_projekt
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(moj_projekt
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::SerialPort
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.moj_projekt)
endif()
set_target_properties(moj_projekt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(moj_projekt)
endif()
