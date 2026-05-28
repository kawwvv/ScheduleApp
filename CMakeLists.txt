cmake_minimum_required(VERSION 3.19)
project(ScheduleApp LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(ScheduleApp
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    data/schedule.json
    scheduleloader.h scheduleloader.cpp
    teacherschedulewindow.h teacherschedulewindow.cpp teacherschedulewindow.ui
    studentschedulewindow.h studentschedulewindow.cpp studentschedulewindow.ui
    resources.qrc



)

target_link_libraries(ScheduleApp
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS ScheduleApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ScheduleApp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

