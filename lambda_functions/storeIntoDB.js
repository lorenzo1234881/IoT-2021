console.log('Loading function');
var AWS = require('aws-sdk');
var dynamo = new AWS.DynamoDB.DocumentClient();
var table = "IoT_IA2";

var iotdata = new AWS.IotData({ endpoint: 'aezhc7emqzhxh-ats.iot.us-east-1.amazonaws.com' });

const thresholds = [
    {
        "temperature": 70,
        "light": 30,
        "trigger": function(temperature, light) {
            
            if(temperature <= this.temperature && light <= this.light)
                return true;
            else
                return false;
        },
        "msgToPublish": {
            "deviceId": 0,
            "led0": 1,
            "led1": 0,
            "led2": 0
        }
    },
    {
        "temperature": 60,
        "light": 10,
        "trigger": function(temperature, light) {
            
            if(temperature <= this.temperature && light <= this.light)
                return true;
            else
                return false;
        },
        "msgToPublish": {
            "deviceId": 0,
            "led0": 0,
            "led1": 1,
            "led2": 0
        }
    },
]

exports.handler = function(event, context) {
   console.log(event)
   var paramsDB = {
    TableName:table,
    Item:{
        "deviceId": event.deviceId,
        "timestamp": event.timestamp,
        "temperature": event.temperature,
        "light": event.light,
        "led0": event.led0,
        "led1": event.led1,
        "led2": event.led2
        }
    };
    
    console.log("Adding a new IoT device...");
    dynamo.put(paramsDB, function(err, data) {
        if (err) {
            console.log(err)
            context.fail();
        } else {
            console.log("Data inserted");
            context.succeed();
        }
    });
    
    var msgToPublish = {
        "deviceId": 0,
        "led0": 0,
        "led1": 0,
        "led2":0
    };
    
    for(let threshold of thresholds)
    {
        if(threshold.trigger(event.temperature, event.light))
            msgToPublish = threshold.msgToPublish;
        else
            break;
    }
    
    msgToPublish.deviceId = event.deviceId;
    
    var paramsiot = {
        topic: "aws_to_local_2",
        payload: JSON.stringify(msgToPublish),
        qos: 0
    };

return new Promise((resolve, reject) => {
    iotdata.publish(paramsiot, function(err, data) {
        if (err) {
            console.log("ERROR => " + JSON.stringify(err));
        }
        else {
            console.log("Success");
        }
    })
});

}
