//
// Autogenerated by Thrift Compiler (0.9.0)
//
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
//

RobotStatus = {
'OFFLINE' : 1,
'ONLINE' : 2
};
Command = {
'STOP' : 1,
'MOVE_FORWARD' : 2,
'MOVE_BACKWARD' : 3,
'TURN_LEFT' : 4,
'TURN_RIGHT' : 5,
'TILT_UP' : 6,
'TILT_DOWN' : 7
};
CommandStatus = {
'OK' : 1,
'ERROR' : 2,
'OFFLINE' : 3
};
SecurityError = {
'USER_NOT_LOGGED_IN' : 1,
'INSUFFICIENT_RIGHTS' : 2
};
UserLoginRequest = function(args) {
  this.username = null;
  this.password = null;
  if (args) {
    if (args.username !== undefined) {
      this.username = args.username;
    }
    if (args.password !== undefined) {
      this.password = args.password;
    }
  }
};
UserLoginRequest.prototype = {};
UserLoginRequest.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
      if (ftype == Thrift.Type.STRING) {
        this.username = input.readString().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 2:
      if (ftype == Thrift.Type.STRING) {
        this.password = input.readString().value;
      } else {
        input.skip(ftype);
      }
      break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

UserLoginRequest.prototype.write = function(output) {
  output.writeStructBegin('UserLoginRequest');
  if (this.username !== null && this.username !== undefined) {
    output.writeFieldBegin('username', Thrift.Type.STRING, 1);
    output.writeString(this.username);
    output.writeFieldEnd();
  }
  if (this.password !== null && this.password !== undefined) {
    output.writeFieldBegin('password', Thrift.Type.STRING, 2);
    output.writeString(this.password);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

RobotInfo = function(args) {
  this.robot_id = null;
  this.robot_name = null;
  this.robot_status = null;
  if (args) {
    if (args.robot_id !== undefined) {
      this.robot_id = args.robot_id;
    }
    if (args.robot_name !== undefined) {
      this.robot_name = args.robot_name;
    }
    if (args.robot_status !== undefined) {
      this.robot_status = args.robot_status;
    }
  }
};
RobotInfo.prototype = {};
RobotInfo.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
      if (ftype == Thrift.Type.STRING) {
        this.robot_id = input.readString().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 2:
      if (ftype == Thrift.Type.STRING) {
        this.robot_name = input.readString().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 3:
      if (ftype == Thrift.Type.I32) {
        this.robot_status = input.readI32().value;
      } else {
        input.skip(ftype);
      }
      break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

RobotInfo.prototype.write = function(output) {
  output.writeStructBegin('RobotInfo');
  if (this.robot_id !== null && this.robot_id !== undefined) {
    output.writeFieldBegin('robot_id', Thrift.Type.STRING, 1);
    output.writeString(this.robot_id);
    output.writeFieldEnd();
  }
  if (this.robot_name !== null && this.robot_name !== undefined) {
    output.writeFieldBegin('robot_name', Thrift.Type.STRING, 2);
    output.writeString(this.robot_name);
    output.writeFieldEnd();
  }
  if (this.robot_status !== null && this.robot_status !== undefined) {
    output.writeFieldBegin('robot_status', Thrift.Type.I32, 3);
    output.writeI32(this.robot_status);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

UserRobots = function(args) {
  this.robots = null;
  if (args) {
    if (args.robots !== undefined) {
      this.robots = args.robots;
    }
  }
};
UserRobots.prototype = {};
UserRobots.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
      if (ftype == Thrift.Type.LIST) {
        var _size0 = 0;
        var _rtmp34;
        this.robots = [];
        var _etype3 = 0;
        _rtmp34 = input.readListBegin();
        _etype3 = _rtmp34.etype;
        _size0 = _rtmp34.size;
        for (var _i5 = 0; _i5 < _size0; ++_i5)
        {
          var elem6 = null;
          elem6 = new RobotInfo();
          elem6.read(input);
          this.robots.push(elem6);
        }
        input.readListEnd();
      } else {
        input.skip(ftype);
      }
      break;
      case 0:
        input.skip(ftype);
        break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

UserRobots.prototype.write = function(output) {
  output.writeStructBegin('UserRobots');
  if (this.robots !== null && this.robots !== undefined) {
    output.writeFieldBegin('robots', Thrift.Type.LIST, 1);
    output.writeListBegin(Thrift.Type.STRUCT, this.robots.length);
    for (var iter7 in this.robots)
    {
      if (this.robots.hasOwnProperty(iter7))
      {
        iter7 = this.robots[iter7];
        iter7.write(output);
      }
    }
    output.writeListEnd();
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

CommandRequest = function(args) {
  this.from = null;
  this.to = null;
  this.command = null;
  if (args) {
    if (args.from !== undefined) {
      this.from = args.from;
    }
    if (args.to !== undefined) {
      this.to = args.to;
    }
    if (args.command !== undefined) {
      this.command = args.command;
    }
  }
};
CommandRequest.prototype = {};
CommandRequest.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
      if (ftype == Thrift.Type.STRING) {
        this.from = input.readString().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 2:
      if (ftype == Thrift.Type.STRING) {
        this.to = input.readString().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 3:
      if (ftype == Thrift.Type.I32) {
        this.command = input.readI32().value;
      } else {
        input.skip(ftype);
      }
      break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

CommandRequest.prototype.write = function(output) {
  output.writeStructBegin('CommandRequest');
  if (this.from !== null && this.from !== undefined) {
    output.writeFieldBegin('from', Thrift.Type.STRING, 1);
    output.writeString(this.from);
    output.writeFieldEnd();
  }
  if (this.to !== null && this.to !== undefined) {
    output.writeFieldBegin('to', Thrift.Type.STRING, 2);
    output.writeString(this.to);
    output.writeFieldEnd();
  }
  if (this.command !== null && this.command !== undefined) {
    output.writeFieldBegin('command', Thrift.Type.I32, 3);
    output.writeI32(this.command);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

CommandResponse = function(args) {
  this.status = null;
  if (args) {
    if (args.status !== undefined) {
      this.status = args.status;
    }
  }
};
CommandResponse.prototype = {};
CommandResponse.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
      if (ftype == Thrift.Type.I32) {
        this.status = input.readI32().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 0:
        input.skip(ftype);
        break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

CommandResponse.prototype.write = function(output) {
  output.writeStructBegin('CommandResponse');
  if (this.status !== null && this.status !== undefined) {
    output.writeFieldBegin('status', Thrift.Type.I32, 1);
    output.writeI32(this.status);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

AccessDeniedException = function(args) {
  this.error = null;
  if (args) {
    if (args.error !== undefined) {
      this.error = args.error;
    }
  }
};
Thrift.inherits(AccessDeniedException, Thrift.TException);
AccessDeniedException.prototype.name = 'AccessDeniedException';
AccessDeniedException.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
      if (ftype == Thrift.Type.I32) {
        this.error = input.readI32().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 0:
        input.skip(ftype);
        break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

AccessDeniedException.prototype.write = function(output) {
  output.writeStructBegin('AccessDeniedException');
  if (this.error !== null && this.error !== undefined) {
    output.writeFieldBegin('error', Thrift.Type.I32, 1);
    output.writeI32(this.error);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

