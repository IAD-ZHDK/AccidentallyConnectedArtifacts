var mqtt = require('mqtt');
var _ = require('underscore');

var client = mqtt.connect("mqtt://grafik16:grafik16@SERVER:1337", {
  clientId: "router"
});

var inputs = [
  "a",
  "b",
  "c",
  "d",
];

var outputs = [
  "e",
  "f",
  "g",
  "h"
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
          console.log('rem:', map[outName], nOut);
          delete map[outName];
        } else if(nIn > 0 && nOut > 0) {
          map[outName] = inName;
          console.log('add:', inName, outName)
        }
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
