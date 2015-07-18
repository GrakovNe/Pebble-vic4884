var Location = "Omsk";
var Vibe_Hourly = 1;
var Vibe_BT = 1;

var Location_key = 1;
var Vibe_Hourly_key = 2;
var Vibe_BT_key = 3;

function HTTPGET(url) {
    var req = new XMLHttpRequest();
    req.open("GET", url, false);
    req.send(null);
    return req.responseText;
}

function ReadSettings(){
	Location = localStorage.getItem(Location_key);
	if (!Location)
		Location = "London";
	
	Vibe_Hourly = localStorage.getItem(Vibe_Hourly_key);
	if (!Vibe_Hourly)
		Vibe_Hourly = 1;
	
	Vibe_BT = localStorage.getItem(Vibe_BT_key);
	if (!Vibe_BT)
		Vibe_BT = 1;
}

function GetWeather(){
	var response = HTTPGET("http://api.openweathermap.org/data/2.5/weather?q=" + Location);
	var json = JSON.parse(response);
	var temperature = (json.main.temp - 273.15) ;
	var conditions = json.weather[0].main;
	
	/*Just adding "+" sign for good temperature*/
	var sign = "";
	if (temperature > 0)
		sign = "+";
	
	temperature = temperature.toFixed(0);	
	
	/*Sending All data to Pebble*/
	var CurrentWeather = sign + temperature ;
	console.log("Weahter updated!" + CurrentWeather);
	
	var Num_Condition = 0;
	
	console.log(conditions);
	
	if (conditions == "Rain")
		Num_Condition = 1;
	if (conditions == "Snow")
		Num_Condition = 2;
	if (conditions == "Thunderstorm")
		Num_Condition = 3;
	if (conditions == "Clear")
		Num_Condition = 4;
	
	Pebble.sendAppMessage( {"WEATHER_TEMPERATURE" : CurrentWeather, "WEATHER_CONDITIONS" : Num_Condition} );
}

function SendSettings(){
	Pebble.sendAppMessage({
							'SETTINGS_HOURLY_VIBE': parseInt(Vibe_Hourly),
							'SETTINGS_BT_VIBE': parseInt(Vibe_BT)
							});
}

Pebble.addEventListener("showConfiguration",
  function(e) {  
    Pebble.openURL("http://grakovne.org/pebble/vic4884/AppConfig.php?Location=" + Location + "&Hourly_Vibe=" + Vibe_Hourly + "&BT_Vibe=" + Vibe_BT);
  }
);

Pebble.addEventListener("ready",
  function(e) {
	console.log("Connection established!");
	ReadSettings();
	SendSettings();
	GetWeather();
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
  
	localStorage.setItem(Location_key, configuration.Location);
	localStorage.setItem(Vibe_Hourly_key, configuration.Hourly_Vibe);
	localStorage.setItem(Vibe_BT_key, configuration.BT_Vibe);  

	ReadSettings();
	SendSettings();
	GetWeather();
	
  }
);

Pebble.addEventListener("appmessage",
  function(e) {
	GetWeather();
  }
);

