cmake_minimum_required(VERSION 3.21)


project(SeaSaltOrigami VERSION 0.1 LANGUAGES CXX)

# 强制开启Qt的MOC、UIC、RCC（核心！解决undefined reference）
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Quick QuickControls2)
qt_standard_project_setup()

qt_add_executable(SeaSaltOrigami)
target_compile_features(SeaSaltOrigami PRIVATE cxx_std_23)
target_sources(SeaSaltOrigami
    PRIVATE
        main.cpp
        SeaSaltGameController.cpp
        SeaSaltGameController.h
        Card.h
        CardPile.h
        Player.h
        GameRule.h
        Card.cpp
        CardPile.cpp
        Player.cpp
        GameRule.cpp
)
# 这个是添加qml资源
qt_add_resources(SeaSaltOrigami "app_qml"
    PREFIX "/"
    FILES
        qml/Main.qml
        qml/Theme.qml
        qml/GameCard.qml
        qml/GameTextButton.qml
        qml/PileSpot.qml
        qml/PileStrip.qml
        qml/PlayedPairStrip.qml
        qml/GameHeader.qml
        qml/GameActionBar.qml
        qml/HandStrip.qml
        qml/BuffStrip.qml
        qml/CollectStrip.qml
        qml/MainMenu.qml
        qml/MenuPrimaryButton.qml
        qml/SettingsScreen.qml
        qml/TutorialScreen.qml
        qml/GamePlay.qml
)
target_link_libraries(SeaSaltOrigami
    PRIVATE
        Qt6::Widgets
        Qt6::Quick
        Qt6::QuickControls2
)
set_target_properties(SeaSaltOrigami PROPERTIES
    WIN32_EXECUTABLE TRUE
)
# 暂时下边的开发一般不会使用

# 自动设置 bin 和 lib  路径
include(GNUInstallDirs)
set(CMAKE_INSTALL_PREFIX "/opt/SeaSaltOrigami" CACHE PATH "Install path prefix" FORCE)
install(TARGETS SeaSaltOrigami
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
# qt自动部署 ---- 后期release发布用
qt_generate_deploy_app_script(
    TARGET SeaSaltOrigami
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
