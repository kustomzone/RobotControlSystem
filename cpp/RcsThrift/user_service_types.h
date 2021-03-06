/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef user_service_TYPES_H
#define user_service_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace robot_control_system {

struct RobotStatus {
  enum type {
    OFFLINE = 1,
    ONLINE = 2
  };
};

extern const std::map<int, const char*> _RobotStatus_VALUES_TO_NAMES;

struct Command {
  enum type {
    STOP = 1,
    MOVE_FORWARD = 2,
    MOVE_BACKWARD = 3,
    TURN_LEFT = 4,
    TURN_RIGHT = 5,
    TILT_UP = 6,
    TILT_DOWN = 7
  };
};

extern const std::map<int, const char*> _Command_VALUES_TO_NAMES;

struct CommandStatus {
  enum type {
    OK = 1,
    ERROR = 2,
    OFFLINE = 3
  };
};

extern const std::map<int, const char*> _CommandStatus_VALUES_TO_NAMES;

struct SecurityError {
  enum type {
    USER_NOT_LOGGED_IN = 1,
    INSUFFICIENT_RIGHTS = 2
  };
};

extern const std::map<int, const char*> _SecurityError_VALUES_TO_NAMES;

typedef struct _UserLoginRequest__isset {
  _UserLoginRequest__isset() : username(false), password(false) {}
  bool username;
  bool password;
} _UserLoginRequest__isset;

class UserLoginRequest {
 public:

  static const char* ascii_fingerprint; // = "07A9615F837F7D0A952B595DD3020972";
  static const uint8_t binary_fingerprint[16]; // = {0x07,0xA9,0x61,0x5F,0x83,0x7F,0x7D,0x0A,0x95,0x2B,0x59,0x5D,0xD3,0x02,0x09,0x72};

  UserLoginRequest() : username(), password() {
  }

  virtual ~UserLoginRequest() throw() {}

  std::string username;
  std::string password;

  _UserLoginRequest__isset __isset;

  void __set_username(const std::string& val) {
    username = val;
  }

  void __set_password(const std::string& val) {
    password = val;
  }

  bool operator == (const UserLoginRequest & rhs) const
  {
    if (!(username == rhs.username))
      return false;
    if (!(password == rhs.password))
      return false;
    return true;
  }
  bool operator != (const UserLoginRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UserLoginRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(UserLoginRequest &a, UserLoginRequest &b);

typedef struct _RobotInfo__isset {
  _RobotInfo__isset() : robot_id(false), robot_name(false), robot_status(false) {}
  bool robot_id;
  bool robot_name;
  bool robot_status;
} _RobotInfo__isset;

class RobotInfo {
 public:

  static const char* ascii_fingerprint; // = "4F8ABE747D9A2DF14F7E84DDBE60D55C";
  static const uint8_t binary_fingerprint[16]; // = {0x4F,0x8A,0xBE,0x74,0x7D,0x9A,0x2D,0xF1,0x4F,0x7E,0x84,0xDD,0xBE,0x60,0xD5,0x5C};

  RobotInfo() : robot_id(), robot_name(), robot_status((RobotStatus::type)0) {
  }

  virtual ~RobotInfo() throw() {}

  std::string robot_id;
  std::string robot_name;
  RobotStatus::type robot_status;

  _RobotInfo__isset __isset;

  void __set_robot_id(const std::string& val) {
    robot_id = val;
  }

  void __set_robot_name(const std::string& val) {
    robot_name = val;
  }

  void __set_robot_status(const RobotStatus::type val) {
    robot_status = val;
  }

  bool operator == (const RobotInfo & rhs) const
  {
    if (!(robot_id == rhs.robot_id))
      return false;
    if (!(robot_name == rhs.robot_name))
      return false;
    if (!(robot_status == rhs.robot_status))
      return false;
    return true;
  }
  bool operator != (const RobotInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RobotInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RobotInfo &a, RobotInfo &b);

typedef struct _UserRobots__isset {
  _UserRobots__isset() : robots(false) {}
  bool robots;
} _UserRobots__isset;

class UserRobots {
 public:

  static const char* ascii_fingerprint; // = "2B34C86D2A169708F9BF5CF5C7C9D6F4";
  static const uint8_t binary_fingerprint[16]; // = {0x2B,0x34,0xC8,0x6D,0x2A,0x16,0x97,0x08,0xF9,0xBF,0x5C,0xF5,0xC7,0xC9,0xD6,0xF4};

  UserRobots() {
  }

  virtual ~UserRobots() throw() {}

  std::vector<RobotInfo>  robots;

  _UserRobots__isset __isset;

  void __set_robots(const std::vector<RobotInfo> & val) {
    robots = val;
  }

  bool operator == (const UserRobots & rhs) const
  {
    if (!(robots == rhs.robots))
      return false;
    return true;
  }
  bool operator != (const UserRobots &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UserRobots & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(UserRobots &a, UserRobots &b);

typedef struct _CommandRequest__isset {
  _CommandRequest__isset() : from(false), to(false), command(false) {}
  bool from;
  bool to;
  bool command;
} _CommandRequest__isset;

class CommandRequest {
 public:

  static const char* ascii_fingerprint; // = "4F8ABE747D9A2DF14F7E84DDBE60D55C";
  static const uint8_t binary_fingerprint[16]; // = {0x4F,0x8A,0xBE,0x74,0x7D,0x9A,0x2D,0xF1,0x4F,0x7E,0x84,0xDD,0xBE,0x60,0xD5,0x5C};

  CommandRequest() : from(), to(), command((Command::type)0) {
  }

  virtual ~CommandRequest() throw() {}

  std::string from;
  std::string to;
  Command::type command;

  _CommandRequest__isset __isset;

  void __set_from(const std::string& val) {
    from = val;
  }

  void __set_to(const std::string& val) {
    to = val;
  }

  void __set_command(const Command::type val) {
    command = val;
  }

  bool operator == (const CommandRequest & rhs) const
  {
    if (!(from == rhs.from))
      return false;
    if (!(to == rhs.to))
      return false;
    if (!(command == rhs.command))
      return false;
    return true;
  }
  bool operator != (const CommandRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CommandRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(CommandRequest &a, CommandRequest &b);

typedef struct _CommandResponse__isset {
  _CommandResponse__isset() : status(false) {}
  bool status;
} _CommandResponse__isset;

class CommandResponse {
 public:

  static const char* ascii_fingerprint; // = "8BBB3D0C3B370CB38F2D1340BB79F0AA";
  static const uint8_t binary_fingerprint[16]; // = {0x8B,0xBB,0x3D,0x0C,0x3B,0x37,0x0C,0xB3,0x8F,0x2D,0x13,0x40,0xBB,0x79,0xF0,0xAA};

  CommandResponse() : status((CommandStatus::type)0) {
  }

  virtual ~CommandResponse() throw() {}

  CommandStatus::type status;

  _CommandResponse__isset __isset;

  void __set_status(const CommandStatus::type val) {
    status = val;
  }

  bool operator == (const CommandResponse & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    return true;
  }
  bool operator != (const CommandResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CommandResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(CommandResponse &a, CommandResponse &b);

typedef struct _AccessDeniedException__isset {
  _AccessDeniedException__isset() : error(false) {}
  bool error;
} _AccessDeniedException__isset;

class AccessDeniedException : public ::apache::thrift::TException {
 public:

  static const char* ascii_fingerprint; // = "8BBB3D0C3B370CB38F2D1340BB79F0AA";
  static const uint8_t binary_fingerprint[16]; // = {0x8B,0xBB,0x3D,0x0C,0x3B,0x37,0x0C,0xB3,0x8F,0x2D,0x13,0x40,0xBB,0x79,0xF0,0xAA};

  AccessDeniedException() : error((SecurityError::type)0) {
  }

  virtual ~AccessDeniedException() throw() {}

  SecurityError::type error;

  _AccessDeniedException__isset __isset;

  void __set_error(const SecurityError::type val) {
    error = val;
  }

  bool operator == (const AccessDeniedException & rhs) const
  {
    if (!(error == rhs.error))
      return false;
    return true;
  }
  bool operator != (const AccessDeniedException &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AccessDeniedException & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(AccessDeniedException &a, AccessDeniedException &b);

} // namespace

#endif
