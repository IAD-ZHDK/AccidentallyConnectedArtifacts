var mqtt = require('mqtt');
var _ = require('underscore');

var client = mqtt.connect("mqtt://grafik16:grafik16@SERVER:1337", {
  clientId: "router"
});

var inputs = [
  "breath",
  "brush",
  "exo-glove",
  "e-landscape",
];

var outputs = [
  "scissomat",
  "surface",
  "vest",
  "sphere"
];

var map = {};

client.on('connect', function(){
  client.subscribe('update');
  client.subscribe('input/#');

  client.on('message', function(topic, payload){
    var str = payload.toString();

    if(topic == 'update') {
      var seg = str.split(" => ");

      if(seg.length == 2) {
        var nIn = parseInt(seg[0]);
        var nOut = parseInt(seg[1]);

        var inName = inputs[nIn-1];
        var outName = outputs[nOut-1];

        if(nIn == 0 && nOut > 0) {
          delete map[outName];

          client.publish("output/" + outName + "/value1", "0");
          client.publish("output/" + outName + "/value2", "0");
        } else if(nIn > 0 && nOut > 0) {
          map[outName] = inName;
        }

        console.log("---")
        _.each(map, function(_in, _out){
          console.log(_in, '=>', _out)
        })
      }
    } else {
      var seg = topic.split('/');

      if(seg.length == 3) {
        _.each(map, function(_in, _out){
          if(map[_out] == _in) {
            client.publish("output/" + _out + "/" + seg[2], payload);
          }
        });
      }
    }
  });
});
