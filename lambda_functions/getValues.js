const AWS = require("aws-sdk");

const dynamo = new AWS.DynamoDB.DocumentClient();
const hour = 3600000;
const tablename = "IoT_IA3";

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
        case "GET /last-values/{deviceId}":
          var data = await dynamo.scan({ TableName: tablename, limit: 1}).promise();
          
          var last_value = {
            temperature: undefined,
            humidity: undefined,
            temperatureMax: undefined,
            temperatureMin: undefined,
            temperatureAvg: undefined,
            humidityMax: undefined,
            humidityMin: undefined,
            humidityAvg: undefined

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
