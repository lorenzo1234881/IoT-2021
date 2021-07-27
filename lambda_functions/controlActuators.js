var AWS = require('aws-sdk');
var iotdata = new AWS.IotData({ endpoint: 'aezhc7emqzhxh-ats.iot.us-east-1.amazonaws.com' });

var statusCode = 200;

exports.handler = async(event) => {
    
    console.log("Event => " + JSON.stringify(event));
    var params = {
        topic: "aws_to_local_2",
        payload: event.body,
        qos: 0
    };

return new Promise((resolve, reject) => {
    iotdata.publish(params, function(err, data) {
        if (err) {
            console.log("ERROR => " + JSON.stringify(err));
        }
        else {
            console.log("Success");
        }
    })
});

};
