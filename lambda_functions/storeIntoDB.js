console.log('Loading function');
var AWS = require('aws-sdk');
var dynamo = new AWS.DynamoDB.DocumentClient();
var table = "IoT_IA1";

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
            "led": 1,
            "buzzer": 0,
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
            "led": 1,
            "buzzer": 1,
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
        "led": event.led,
        "buzzer": event.buzzer,
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
        "led": 0,
        "buzzer": 0,
    };
    
    for(let threshold of thresholds)
    {
        if(threshold.trigger(event.temperature, event.light))
            msgToPublish = threshold.msgToPublish;
        else
            break;
    }
    
    var paramsiot = {
        topic: "awsiot_to_localgateway",
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
