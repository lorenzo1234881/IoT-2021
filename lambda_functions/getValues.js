const AWS = require("aws-sdk");

const dynamo = new AWS.DynamoDB.DocumentClient();

const hour = 3600000;

const AggregateVal = {
    max: 0,
    min: Number.MAX_VALUE,
    avg: 0,
    sum: 0,
    n: 0,
    updateMin: function(value) {
        if(value < this.min){
            this.min = value;
        }
    },
    updateMax: function(value){
        if(value > this.max) {
            this.max = value;
        } 
    },
    updateSum: function(value){
        this.sum += value;
        this.n += 1;
    },
    computeAvg: function(){
        if(this.n > 0) {
          this.avg = this.sum/this.n;
        }
    }
};


exports.handler = async (event, context) => {
  let body;
  let statusCode = 200;
  const headers = {
    "Content-Type": "application/json",
    'Access-Control-Allow-Headers': '*',
    'Access-Control-Allow-Method': '*',
    "Access-Control-Allow-Origin": "*"
  };

  try {
    switch (event.routeKey) {
      case "GET /last-values":
        var data = await dynamo.scan({ TableName: "IoT_IA1", limit: 1}).promise();
        
        var last_value = {}
        
        if(data.ScannedCount >= 1) {
            last_value = data.Items[data.ScannedCount-1];
            if(Date.now() - last_value.timestamp >  hour)
              last_value = undefined;
        }
          
        body = JSON.stringify(last_value);
        
        break;
      case "GET /aggregate-values":
        
        var data = await dynamo.scan({ TableName: "IoT_IA1"}).promise();
        var aggValTemperature = Object.create(AggregateVal);
        var aggValLight = Object.create(AggregateVal);

        const currentTime = Date.now();
        
        for (var item of data.Items)
        {
          if(currentTime - item.timestamp >  hour)
            continue;
          
          aggValTemperature.updateMax(item.temperature);
          aggValTemperature.updateMin(item.temperature);
          aggValTemperature.updateSum(item.temperature);
          
          aggValLight.updateMax(item.light);
          aggValLight.updateMin(item.light);
          aggValLight.updateSum(item.light);
        }
        aggValTemperature.computeAvg();
        aggValLight.computeAvg();
        
        var jsonMsg = {
            light:aggValLight,
            temperature: aggValTemperature
        };
          
        body = JSON.stringify(jsonMsg);
        break;
        
      case "OPTIONS /{proxy+}":
        break;
      default:
        throw new Error(`Unsupported route: "${event.routeKey}"`);
    }
  } catch (err) {
    statusCode = 400;
    body = err.message;
  } finally {
    body = JSON.stringify(body);
  }

  return {
    statusCode,
    body,
    headers
  };
};
