console.log('Loading function');
var AWS = require('aws-sdk');
var dynamo = new AWS.DynamoDB.DocumentClient();
var table = "IoT_IA3";

exports.handler = function(event, context) {
    console.log(event)
   
    var jsonstring = Buffer.from(event.payload, 'base64').toString('ascii');
    var payload = JSON.parse(jsonstring);
   
    var paramsDB = {
    TableName:table,
    Item:{
        "timestamp" : event.timestamp,
        "deviceId": event.deviceId,
        "temperature": payload.temperature,
        "temperatureMax": payload.temperatureMax,
        "temperatureMin": payload.temperatureMin,
        "temperatureAvg": payload.temperatureAvg,
        "humidity": payload.humidity,
        "humidityMax": payload.humidityMax,
        "humidityMin": payload.temperatureMin,
        "humidityAvg": payload.humidityAvg
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
}
