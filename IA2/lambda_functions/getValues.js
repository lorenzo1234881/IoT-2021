const AWS = require("aws-sdk");

const dynamo = new AWS.DynamoDB.DocumentClient();
const hour = 3600000;
const tablename = "IoT_IA2";

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

const AggValPerDevice = {
  "deviceId": 0,
  "aggValTemperature": undefined,
  "aggValLight": undefined
};

const AggValPerDeviceList = {
  "list": [],
  updateAggVals: function(deviceId, temperature, light) {
    
    let aggValOfDev = 0
    let found = false;
    
    for(let aggVal of this.list) {
      if(aggVal.deviceId == deviceId) {
        aggValOfDev = aggVal;

        found = true;
        break;
      }
    }
    
    if(!found) {
      aggValOfDev = Object.create(AggValPerDevice);
      aggValOfDev.deviceId = deviceId;
      aggValOfDev.aggValTemperature = Object.create(AggregateVal);
      aggValOfDev.aggValLight = Object.create(AggregateVal);
      
      this.list.push(aggValOfDev);
    }
      aggValOfDev.aggValTemperature.updateMax(temperature);
      aggValOfDev.aggValTemperature.updateMin(temperature);
      aggValOfDev.aggValTemperature.updateSum(temperature);
      
      aggValOfDev.aggValLight.updateMax(light);
      aggValOfDev.aggValLight.updateMin(light);
      aggValOfDev.aggValLight.updateSum(light);
  },
  computeAvg: function(){
    for(let aggVal of this.list) {
      aggVal.aggValTemperature.computeAvg();
      aggVal.aggValLight.computeAvg();
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
    
    let currentTime;
    
    switch (event.routeKey) {
      case "GET /last-values":
        var data = await dynamo.scan({ TableName: tablename, limit: 1}).promise();
        
        var last_value = {
          temperature: undefined,
          light: undefined,
          led0: undefined,
          led1: undefined,
          led2: undefined
        }
        
        if(data.ScannedCount >= 1) {
            last_value = data.Items[data.ScannedCount-1];
             if(Date.now() - last_value.timestamp >  hour)
               last_value = undefined;
        }
          
        body = JSON.stringify(last_value);
        
        break;
        case "GET /last-values/{deviceId}":
          var data = await dynamo.scan({ TableName: tablename, limit: 1}).promise();
          
          var last_value = {
            temperature: undefined,
            light: undefined,
            led0: undefined,
            led1: undefined,
            led2: undefined
          }
  
          currentTime = Date.now();
          
          for (var item of data.Items)
          {        
            if(item.deviceId == event.pathParameters.deviceId && currentTime-item.timestamp < hour) {
              last_value = item;
            }
          }
          
          body = JSON.stringify(last_value);
          
      break;
      case "GET /aggregate-values":
        
        var data = await dynamo.scan({ TableName: tablename}).promise();
        var aggValTemperature = Object.create(AggregateVal);
        var aggValLight = Object.create(AggregateVal);

        currentTime = Date.now();
        
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

      case "GET /aggregate-values/{deviceId}":
        
        var data = await dynamo.scan({ TableName: tablename}).promise();
        var aggValTemperature = Object.create(AggregateVal);
        var aggValLight = Object.create(AggregateVal);

        currentTime = Date.now();
        
        for (var item of data.Items)
        {
           if(currentTime - item.timestamp >  hour)
             continue;
          if(item.deviceId != event.pathParameters.deviceId)
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
        
      case "GET /aggregate-values-device":
        var data = await dynamo.scan({ TableName: tablename}).promise();
        currentTime = Date.now();
        
        for (var item of data.Items)
        {
           if(currentTime - item.timestamp >  hour)
            continue;
          
          AggValPerDeviceList.updateAggVals(item.deviceId, item.temperature, item.light);
        }
        AggValPerDeviceList.computeAvg();

        console.log(AggValPerDeviceList.list[0].deviceId)
        console.log(AggValPerDeviceList.list[0].aggValTemperature)
        console.log(AggValPerDeviceList.list[0].aggValLight)

        body = JSON.stringify(AggValPerDeviceList);
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
