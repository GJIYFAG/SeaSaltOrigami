include("/root/下载/SeaSaltOrigami_refit/build/C_C_2026_05_13-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/SeaSaltOrigami-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "/root/下载/SeaSaltOrigami_refit/build/C_C_2026_05_13-Debug/SeaSaltOrigami"
    GENERATE_QT_CONF
)
