var myAPIKey = '1f14864b11d1bca93ee5f425431b031c';
var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  // Construct URL
  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
      pos.coords.latitude + '&lon=' + pos.coords.longitude + '&appid=' + myAPIKey;

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET',
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.main.temp - 273.15);
      console.log('Temperature is ' + temperature);

      // Conditions
      var conditions = json.weather[0].main;
      console.log('Conditions are ' + conditions);

      // Assemble dictionary using our keys
      var dictionary = {
        'KEY_TYPE': 0, //WEATHER
        'KEY_TEMPERATURE': temperature,
        'KEY_CONDITIONS': conditions
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log('Weather info sent to Pebble successfully!');
        },
        function(e) {
          console.log('Error sending weather info to Pebble!');
        }
      );
    }
  );
}

function locationError(err) {
  console.log('Error requesting location!');
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

function getStocks() {
  var url = 'http://query.yahooapis.com/v1/public/yql?q=' +
            'select%20*%20from%20yahoo.finance.quotes%20where%20symbol%20in%20(%22N,APPL%22)' +
            '&env=store://datatables.org/alltableswithkeys&format=json';

  // Send request to yahooapis
  xhrRequest(url, 'GET',
    function(responseText) {
      var json = JSON.parse(responseText);

      // TODO: Take all stocks and sum them up to send over as one value

      // Assemble dictionary using our keys
      var dictionary = {
        'KEY_TYPE': 1, //STOCK
        'KEY_STOCKS': 0,
      };
    }
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log('PebbleKit JS ready!');

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    getWeather();
  }
);
