var myWeatherAPIKey = '1f14864b11d1bca93ee5f425431b031c';
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
      pos.coords.latitude + '&lon=' + pos.coords.longitude + '&appid=' + myWeatherAPIKey;

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
            'select%20*%20from%20yahoo.finance.quotes%20where%20symbol%20in%20(%22N,VAB.TO,SCTY%22)' +
            '&env=store://datatables.org/alltableswithkeys&format=json';
  var quotes = [], changes = [], changesPercent = [];
  var totalChange = 0, totalPercent = 0;

  // Send request to yahooapis
  xhrRequest(url, 'GET',
    function(responseText) {
      var json = JSON.parse(responseText);

      // Extract information from response JSON
      var quoteJson = json.query.results.quote;
      for (var quote in quoteJson) {
        if (quoteJson.hasOwnProperty(quote)) {
          quotes.push(quoteJson[quote].symbol);
          changes.push(quoteJson[quote].Change);
          changesPercent.push(quoteJson[quote].ChangeinPercent.substring(0,
            quoteJson[quote].ChangeinPercent.indexOf('%')));
        }
      }

      // Sum our changes and percentages for totals
      for (var i = 0; i < changes.length; i++) {
        totalChange += Number(changes[i]);
      }
      for (var i = 0; i < changesPercent.length; i++) {
        totalPercent += Number(changesPercent[i]);
      }

      var dictionary = {
        'KEY_TYPE': 1, //STOCK
        'KEY_STOCKS_TOTAL_CHANGE': totalChange.toString(),
        'KEY_STOCKS_TOTAL_PERCENT': totalPercent.toString(),
        'KEY_STOCKS_TOTAL_DIRECTION_IS_UP': ((totalChange >= 0) ? true : false)
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log('Stock info sent to Pebble successfully!');
        },
        function(e) {
          console.log('Error sending stock info to Pebble!');
        }
      );
    }
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log('PebbleKit JS ready!');

    // Get the initial weather
    getWeather();
    getStocks();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    getWeather();
  }
);

Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL('https://my-website.com/config-page.html');
});
