(function() {
  var Connection = function(url) {
    var ths = this;

    this.handlers = new Array();

    this.marshall = function(data) {return data};
    if (typeof JSON.stringify != 'undefined') {
      this.marshall = function(data) {
        if (typeof data == 'string') return data;
        return JSON.stringify(data);
      }
    }

    var parse = function(data) {
      var call = '';
      eval('call = ' + data);
      return call;
    }
    if (typeof JSON.parse != 'undefined') parse = JSON.parse;

    this.onmessage = null;
    this.onopen = null;
    this.onclose = null;
    this.onerror = null;

    var WS = require('websocket').client;
    var client = new WS();
    this.connection = null;
    client.on('connect', function(con) {
      con.on('message', function(message) {
        var e = {data: message.utf8Data};

        if(ths.onmessage) {
          try {
            ths.onmessage(e);
          } catch(err) {
            ros_debug(err);
          }
        }

        var call = ''; 
        try {
          call = parse(e.data);
        } catch(err) {
          return;
        }

        for (var i in ths.handlers[call.receiver]) {
          var handler = ths.handlers[call.receiver][i]
          handler(call.msg);
        }
      });

      con.on('close', function() {if (ths.onclose != null) ths.onclose();});
      con.on('error', function() {if (ths.onerror != null) ths.onerror();});

      ths.connection = con;

      if (ths.onopen != null) {
        ths.onopen();
      }

    });

    this.magicServices = new Array('/rosbridge/topics','/rosbridge/services','/rosbridge/typeStringFromTopic','/rosbridge/typeStringFromService','/rosbridge/msgClassFromTypeString','/rosbridge/reqClassFromTypeString','/rosbridge/rspClassFromTypeString','/rosbridge/classFromTopic','/rosbridge/classesFromService');

    client.connect(url);
  }

  Connection.prototype.callService = function(service, payload, callback) {
    this.handlers[service] = new Array(callback);
    var call = '{"receiver":"' + service + '"';
    call += ',"msg":' + this.marshall(payload) + '}';
    this.connection.send(call);
  }

  Connection.prototype.publish = function(topic, typeStr, payload) {
    typeStr.replace(/^\//,'');
    var call = '{"receiver":"' + topic + '"';
    call += ',"msg":' + this.marshall(payload);
    call += ',"type":"' + typeStr + '"}';
    this.connection.send(call);
  }

  Connection.prototype.addHandler = function(topic, func) {
    if (!(topic in this.handlers)) {
      this.handlers[topic] = new Array();
    }
    this.handlers[topic].push(func);
  }

  Connection.prototype.setOnError = function(func) {
    this.onerror = func;
  }

  Connection.prototype.setOnClose = function(func) {
    this.onclose = func;
  }

  Connection.prototype.setOnOpen = function(func) {
    this.onopen = func;
    if (this.connection != null) this.onopen();
  }

  Connection.prototype.setOnMessage = function(func) {
    this.onmessage = func;
  }

  this.Connection = Connection;

}).call(this);
