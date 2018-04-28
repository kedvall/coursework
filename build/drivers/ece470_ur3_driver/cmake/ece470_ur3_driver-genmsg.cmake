# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "ece470_ur3_driver: 2 messages, 0 services")

set(MSG_I_FLAGS "-Iece470_ur3_driver:/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(ece470_ur3_driver_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/positions.msg" NAME_WE)
add_custom_target(_ece470_ur3_driver_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "ece470_ur3_driver" "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/positions.msg" ""
)

get_filename_component(_filename "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/command.msg" NAME_WE)
add_custom_target(_ece470_ur3_driver_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "ece470_ur3_driver" "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/command.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(ece470_ur3_driver
  "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/positions.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ece470_ur3_driver
)
_generate_msg_cpp(ece470_ur3_driver
  "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/command.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ece470_ur3_driver
)

### Generating Services

### Generating Module File
_generate_module_cpp(ece470_ur3_driver
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ece470_ur3_driver
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(ece470_ur3_driver_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(ece470_ur3_driver_generate_messages ece470_ur3_driver_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/positions.msg" NAME_WE)
add_dependencies(ece470_ur3_driver_generate_messages_cpp _ece470_ur3_driver_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/command.msg" NAME_WE)
add_dependencies(ece470_ur3_driver_generate_messages_cpp _ece470_ur3_driver_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(ece470_ur3_driver_gencpp)
add_dependencies(ece470_ur3_driver_gencpp ece470_ur3_driver_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS ece470_ur3_driver_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(ece470_ur3_driver
  "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/positions.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ece470_ur3_driver
)
_generate_msg_lisp(ece470_ur3_driver
  "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/command.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ece470_ur3_driver
)

### Generating Services

### Generating Module File
_generate_module_lisp(ece470_ur3_driver
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ece470_ur3_driver
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(ece470_ur3_driver_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(ece470_ur3_driver_generate_messages ece470_ur3_driver_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/positions.msg" NAME_WE)
add_dependencies(ece470_ur3_driver_generate_messages_lisp _ece470_ur3_driver_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/command.msg" NAME_WE)
add_dependencies(ece470_ur3_driver_generate_messages_lisp _ece470_ur3_driver_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(ece470_ur3_driver_genlisp)
add_dependencies(ece470_ur3_driver_genlisp ece470_ur3_driver_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS ece470_ur3_driver_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(ece470_ur3_driver
  "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/positions.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ece470_ur3_driver
)
_generate_msg_py(ece470_ur3_driver
  "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/command.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ece470_ur3_driver
)

### Generating Services

### Generating Module File
_generate_module_py(ece470_ur3_driver
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ece470_ur3_driver
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(ece470_ur3_driver_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(ece470_ur3_driver_generate_messages ece470_ur3_driver_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/positions.msg" NAME_WE)
add_dependencies(ece470_ur3_driver_generate_messages_py _ece470_ur3_driver_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/youbot/edvall2_catkin/src/drivers/ece470_ur3_driver/msg/command.msg" NAME_WE)
add_dependencies(ece470_ur3_driver_generate_messages_py _ece470_ur3_driver_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(ece470_ur3_driver_genpy)
add_dependencies(ece470_ur3_driver_genpy ece470_ur3_driver_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS ece470_ur3_driver_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ece470_ur3_driver)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ece470_ur3_driver
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(ece470_ur3_driver_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ece470_ur3_driver)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ece470_ur3_driver
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(ece470_ur3_driver_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ece470_ur3_driver)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ece470_ur3_driver\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ece470_ur3_driver
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(ece470_ur3_driver_generate_messages_py std_msgs_generate_messages_py)
